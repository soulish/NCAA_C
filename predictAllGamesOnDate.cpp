#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TH1F.h"
#include <boost/algorithm/string.hpp>
#include <TROOT.h>
#include "src/Team.h"
#include "src/ConstantTeam5YearAverages.h"
#include "src/ConstantSeasonInfo.h"
#include "src/ConstantGameFunction.h"
#include "src/ConstantSRSadditions.h"
#include "src/ConstantTeamNeutralRatios.h"
#include "src/ConstantWAverageFunctions.h"
#include "src/ConstantStandardDeviations.h"
#include "src/readTeams.h"
#include "helpers/doubleFormatter.h"
#include "helpers/vegasConversion.h"

void printOptions();

int main(int argc,char *argv[]){
    int c;
    int year = 0;
    std::string teamNameA = "", teamNameB = "", outFileName = "";
    bool useHistogramsFile = false;
    std::string srsValue = "free";
    std::string evaluationDay = "";
    std::string predictionDay = "";

    /*____________________________Parse Command Line___________________________*/
    while((c = getopt(argc,argv,"y:hHs:d:D:")) != -1){
        switch(c){
            case 'y':
                year = atoi(optarg);
                break;
            case 'H':
                useHistogramsFile = true;
                break;
            case 's':
                srsValue.assign(optarg);
                break;
            case 'd':
                predictionDay.assign(optarg);
                break;
            case 'D':
                evaluationDay.assign(optarg);
                break;
            case 'h':
                printOptions();
                return 0;
            default:
                // not an option
                break;
        }
    }

    if (year == 0){
        std::cout << "You must specify the year with the -y switch" << std::endl;
        printOptions();
        return 0;
    }
    if (predictionDay == ""){
        std::cout << "You must specify the day for which you want to make predictions with the -d switch" << std::endl;
        printOptions();
        return 0;
    }
    if (evaluationDay == ""){
        std::cout << "You did not specify an evaluation day with the -D switch, therefore" << std::endl;
        std::cout << "the evaluation day will be set to the prediction day." << std::endl;
        evaluationDay = predictionDay;
    }

    char *homePath, path[256], histName[256];
    homePath = getenv("HOME");

    sprintf(path, "%s/cpp/NCAA_C/constants/team_5yr_averages.d", homePath);
    ConstantTeam5YearAverages *averages = ConstantTeam5YearAverages::Instance();
    averages->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo *seasonInfo = ConstantSeasonInfo::Instance();
    seasonInfo->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/waverage_functions.d", homePath);
    ConstantWAverageFunctions *functions = ConstantWAverageFunctions::Instance();
    functions->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/team_neutral_ratios.d", homePath);
    ConstantTeamNeutralRatios *ratios = ConstantTeamNeutralRatios::Instance();
    ratios->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/srs_additions.d", homePath);
    ConstantSRSadditions *additions = ConstantSRSadditions::Instance();
    additions->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/game_function_weights.%s.d", homePath,srsValue.c_str());
    ConstantGameFunction *gameFunction = ConstantGameFunction::Instance();
    gameFunction->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/waverage_standard_deviations.d", homePath);
    ConstantStandardDeviations *stdDevs = ConstantStandardDeviations::Instance();
    stdDevs->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/teams/%i/", homePath, year);
    std::cout << "Reading in games and scheduled games for " << year << std::endl;
    readTeamsFromDir(path,"games");
    readTeamsFromDir(path,"schedule");
    readTeamsFromDir(path,"waverages");

    std::map<std::string, Team *> orderedTeams(Team::getTeams().begin(), Team::getTeams().end());

    TFile *histsFile;
    std::unordered_map<int, TH1F*> probs_by_year, probs_err_by_year;
    if (useHistogramsFile){
        sprintf(path, "%s/cpp/NCAA_C/rootFiles/gameFunctionHistograms.%s.root", homePath, srsValue.c_str());
        histsFile = new TFile(path);

        for (int y = 2007; y <= 2016; y++){
            sprintf(path, "probs_by_year_%d", y);
            probs_by_year.emplace(y, (TH1F*)gROOT->FindObject(path));
            sprintf(path, "probs_err_by_year_%d", y);
            probs_err_by_year.emplace(y, (TH1F*)gROOT->FindObject(path));
        }
    }

    std::vector<TeamScheduledGame*> *scheduledGamesVec;
    TeamScheduledGame *scheduledGame, *oppScheduledGame;
    Team *opp;
    for (auto &team : orderedTeams) {
        scheduledGamesVec = team.second->ScheduledGamesOnDate(
                boost::gregorian::date(boost::gregorian::from_string(predictionDay)));
        if (!scheduledGamesVec) continue;

        for (int i = 0; i < scheduledGamesVec->size(); i++){
            scheduledGame = scheduledGamesVec->at(i);
            if (team.first > scheduledGame->getOppname()) continue;  //process games only once.
            opp = Team::findTeam(scheduledGame->getOppname());
            if (!opp){
                //std::cout << std::endl;
                continue;
            }

            TeamWAverage *waA = team.second->WAverageOnDate(boost::gregorian::date(boost::gregorian::from_string(evaluationDay)));
            TeamWAverage *waB = opp->WAverageOnDate(boost::gregorian::date(boost::gregorian::from_string(evaluationDay)));

            std::string loc = scheduledGame->getLocation();
            std::string oppLoc;
            if (loc == "home") oppLoc = "away";
            else if (loc == "away") oppLoc = "home";
            else if (loc == "neutral") oppLoc = "neutral";

            double gameScoreA = gameFunction->predictGame(waA, waB, team.second->getYear(), loc, oppLoc);
            double gameScoreB = gameFunction->predictGame(waB, waA, opp->getYear(), oppLoc, loc);

            if (useHistogramsFile) {
                double gamePctA = probs_by_year[team.second->getYear()]->GetBinContent((gameScoreA + 3) * 1600 / 6.0);
                double gamePctB = probs_by_year[opp->getYear()]->GetBinContent((gameScoreB + 3) * 1600 / 6.0);
                double gamePctA_err = probs_err_by_year[team.second->getYear()]->GetBinContent((gameScoreA + 3) * 1600 / 6.0);
                double gamePctB_err = probs_err_by_year[opp->getYear()]->GetBinContent((gameScoreB + 3) * 1600 / 6.0);

                if (gameScoreA >= 0) {
                    std::cout << team.first << " is " << doubleFormatter(gamePctA * 100, 1) <<
                    "% likely to win the game against " << opp->getName() << std::endl;
                }
                else {
                    std::cout << opp->getName() << " is " << doubleFormatter(gamePctB * 100, 1) <<
                    "% likely to win the game against " << team.first << std::endl;
                }
                double moneylineA = pctToMoneyline(gamePctA,true);
                double moneylineB = pctToMoneyline(gamePctB,true);
                double spreadA = pctToSpread(gamePctA);
                double spreadB = pctToSpread(gamePctB);
                std::cout << "\t\t" << team.first << " moneyline should be: " << doubleFormatter(moneylineA,0) << "\t" <<
                        "corresponding to a spread of: " << doubleFormatter(spreadA,1) << std::endl;
                std::cout << "\t\t" << opp->getName() << " moneyline should be: " << doubleFormatter(moneylineB,0) << "\t" <<
                        "corresponding to a spread of: " << doubleFormatter(spreadB,1) << std::endl;
            }
            else {
                if (gameScoreA >= 0)
                    std::cout << team.first << " is favored to win the game with a gameScore of " <<
                    doubleFormatter(gameScoreA, 3) << " against " << opp->getName() << std::endl;
                else
                    std::cout << opp->getName() << " is favored to win the game with a gameScore of " <<
                    doubleFormatter(gameScoreB, 3) << " against " << team.first << std::endl;
            }
        }
    }

    return 0;
}

void printOptions(){
    std::cout << std::endl;
    std::cout << "predictAllGamesOnDate Usage options" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y (int) year of evaluation (no default)[Required]" << std::endl;
    std::cout << "\t-d (date string: yyyy-mm-dd) date on which to predict games (no default)[Required]" << std::endl;
    std::cout << "\t-D (date string: yyyy-mm-dd) date of the waverages used for prediction (default: day of predictions)[Optional]" << std::endl;
    std::cout << "\t-H use histograms file to calculate percentages [Optional]" << std::endl;
    std::cout << "\t-s (double) srs value to use for weights and histograms (default: \tfree\")[Optional]" << std::endl;
    std::cout << "\t-h print this message" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: predictAllGamesOnDate -y 2016 -H -d 2016-01-01 -D 2016-12-31" << std::endl;
    std::cout << std::endl;
    std::cout << "This program will calculate the odds of each team winning for every game to be" << std::endl;
    std::cout << "played on the date specified by the -d switch.  The games to be played on the " << std::endl;
    std::cout << "date are determined by the schedules files.  Every team is cycled through, and" << std::endl;
    std::cout << "if they have a game (or games) on the date, then the program will produce a prediction" << std::endl;
    std::cout << "for them using the weights and histograms file (if desired) associated with the " << std::endl;
    std::cout << "SRS indicated with the -s switch.  The TeamWAverage to use for the predictions" << std::endl;
    std::cout << "defaults to the day of the game, but can be set with the -D option; this is so" << std::endl;
    std::cout << "that you can make predictions for a day even if you don't have completely up-to-date" << std::endl;
    std::cout << "statistics, and haven't calculated the waverage for the game date yet, which" << std::endl;
    std::cout << "will be common since you probably won't be able to instantly download all of the" << std::endl;
    std::cout << "game information immediately for every day, so the stats lag behind by a few days." << std::endl;
    std::cout << "" << std::endl;
    std::cout << std::endl;
}