#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <boost/foreach.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <TFile.h>
#include "src/ConstantTeamNeutralRatios.h"
#include "src/Team.h"
#include "src/readTeams.h"
#include "helpers/doubleFormatter.h"
#include "src/ConstantSeasonInfo.h"
#include "src/ConstantStandardDeviations.h"
#include "src/ConstantWAverageFunctions.h"
#include "TH1F.h"
#include "src/ConstantSRSadditions.h"
#include "src/ConstantGameFunction.h"

void printOptions();

int main(int argc,char *argv[]) {
    int c;
    std::string outFileName = "";
    std::vector<std::string> years;
    std::string inYears = "";
    std::string srsValue = "free";
    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:o:s:h")) != -1) {
        switch (c) {
            case 'y':
                inYears.assign(optarg);
                boost::split(years, inYears, boost::is_any_of(","));
                break;
            case 'o':
                outFileName.assign(optarg);
                break;
            case 's':
                srsValue.assign(optarg);
                break;
            case 'h':
                printOptions();
                return 0;
            default:
                // not an option
                break;
        }
    }

    //ensure years and outYear are set
    if (years.empty()) {
        std::cout << "You must set the input years using the -y switch and a comma-separated list of years" <<
        std::endl;
        printOptions();
        return 0;
    }
    if (outFileName == ""){
        std::cout << "You must set the output file name using the -o switch" << std::endl;
        printOptions();
        return 0;
    }

    char *homePath, path[256];
    homePath = getenv("HOME");

    //read in the necessary constants
    sprintf(path, "%s/cpp/NCAA_C/constants/waverage_standard_deviations.d", homePath);
    ConstantStandardDeviations *stdDevs = ConstantStandardDeviations::Instance();
    stdDevs->initialize(path);

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

    //read in the game and waverage info
    for (std::string &year : years) {
        sprintf(path, "%s/cpp/NCAA_C/teams/%s/", homePath, year.c_str());
        std::cout << "Reading in games and waverages for " << year << std::endl;
        readTeamsFromDir(path);
        readTeamsFromDir(path, "waverages");
    }

    std::unordered_map<std::string, Team *> teams = Team::getTeams();
    std::unordered_map<std::string, TeamGame *> games;
    Team *opp;
    TeamWAverage *wa1, *wa2;

    TFile file(("rootFiles/"+outFileName).c_str(),"RECREATE");

    std::unordered_map<int, TH1F*> hists_gameScore_wins, hists_gameScore_losses;
    for (int year = 2007; year <= 2016; year++) {
        sprintf(path,"hist_gameScore_wins_%d",year);
        hists_gameScore_wins.emplace(year, new TH1F(path,"",200,-3,3));
        sprintf(path,"hist_gameScore_losses_%d",year);
        hists_gameScore_losses.emplace(year, new TH1F(path,"",200,-3,3));
    }

    for (auto &team : teams) {
        games = team.second->getGamesByDate();
        for (auto &game : games) {
            if (team.first < game.second->getOpp()) continue; //look at each game only once
            opp = Team::findTeam(game.second->getOpp());
            if (!opp) continue;
            if (game.second->getDate() >= seasonInfo->get(team.second->getYear(), "tournament start")) continue;
            if (game.second->getDate().month().as_number() == 11) continue;

            bool win = game.second->getWin() == 1;
            int gameYear = team.second->getYear();

            wa1 = team.second->WAverageOnDate(game.second->getDate());
            wa2 = opp->WAverageOnDate(game.second->getDate());

            std::string loc = game.second->getLoc();
            std::string oppLoc = game.second->getOppLoc();

            std::unordered_map<int, double> gameScoreByYear;

            switch (gameYear){
                case 2005:
                    for (int y = 2007; y <= 2010; y++)
                        gameScoreByYear.emplace(y, gameFunction->predictGame(wa1,wa2,y,loc,oppLoc));
                    break;
                case 2006:
                    for (int y = 2007; y <= 2011; y++)
                        gameScoreByYear.emplace(y, gameFunction->predictGame(wa1,wa2,y,loc,oppLoc));
                    break;
                case 2007:
                    for (int y = 2008; y <= 2012; y++)
                        gameScoreByYear.emplace(y, gameFunction->predictGame(wa1,wa2,y,loc,oppLoc));
                    break;
                case 2008:
                    for (int y = 2009; y <= 2013; y++)
                        gameScoreByYear.emplace(y, gameFunction->predictGame(wa1,wa2,y,loc,oppLoc));
                    break;
                case 2009:
                    for (int y = 2010; y <= 2014; y++)
                        gameScoreByYear.emplace(y, gameFunction->predictGame(wa1,wa2,y,loc,oppLoc));
                    break;
                case 2010:
                    for (int y = 2011; y <= 2015; y++)
                        gameScoreByYear.emplace(y, gameFunction->predictGame(wa1,wa2,y,loc,oppLoc));
                    break;
                case 2011:
                    for (int y = 2012; y <= 2016; y++)
                        gameScoreByYear.emplace(y, gameFunction->predictGame(wa1,wa2,y,loc,oppLoc));
                    break;
                case 2012:
                    for (int y = 2013; y <= 2016; y++)
                        gameScoreByYear.emplace(y, gameFunction->predictGame(wa1,wa2,y,loc,oppLoc));
                    break;
                case 2013:
                    for (int y = 2014; y <= 2016; y++)
                        gameScoreByYear.emplace(y, gameFunction->predictGame(wa1,wa2,y,loc,oppLoc));
                    break;
                case 2014:
                    for (int y = 2015; y <= 2016; y++)
                        gameScoreByYear.emplace(y, gameFunction->predictGame(wa1,wa2,y,loc,oppLoc));
                    break;
                case 2015:
                    for (int y = 2016; y <= 2016; y++)
                        gameScoreByYear.emplace(y, gameFunction->predictGame(wa1,wa2,y,loc,oppLoc));
                    break;
            }
            if (win){
                for (int y = 2007; y <= 2016; y++){
                    if (gameScoreByYear.find(y) == gameScoreByYear.end()) continue;
                    hists_gameScore_wins[y]->Fill(gameScoreByYear[y]);
                    hists_gameScore_losses[y]->Fill(-gameScoreByYear[y]);
                }
            }
            else{
                for (int y = 2007; y <= 2016; y++){
                    if (gameScoreByYear.find(y) == gameScoreByYear.end()) continue;
                    hists_gameScore_wins[y]->Fill(-gameScoreByYear[y]);
                    hists_gameScore_losses[y]->Fill(gameScoreByYear[y]);
                }
            }
        }
    }

    TF1 *fnWins = new TF1("fnWins","gaus");
    TF1 *fnLosses = new TF1("fnLosses","gaus");

    std::unordered_map<int, TH1F*> hists_probs;
    std::unordered_map<int, TH1F*> hists_probs_err;

    for (int y = 2007; y <= 2016; y++){
        if (hists_gameScore_wins[y]->Integral() == 0) continue;
        hists_gameScore_wins[y]->Fit(fnWins,"q");
        hists_gameScore_losses[y]->Fit(fnLosses,"q");

        sprintf(path,"probs_by_year_%d",y);
        hists_probs.emplace(y, new TH1F(path,"",1600,-3,3));
        sprintf(path,"probs_err_by_year_%d",y);
        hists_probs_err.emplace(y, new TH1F(path,"",1600,-3,3));

        for (int i = 1; i < 1600; i++){
            double w = fnWins->Eval(-3+i*6/1600.0);
            double l = fnLosses->Eval(-3+i*6/1600.0);
            double p = 0;
            double e = 0;
            if (w != 0 && l != 0){
                p = w / (double)(w+l);
                e = sqrt(p*(1-p) / (w+l));
            }
            else if (w != 0 && l == 0){
                p = 1;
                e = 0.01;
            }
            else if (w == 0){
                p = 0;
                e = 0.01;
            }

            hists_probs[y]->SetBinContent(i,p);
            hists_probs_err[y]->SetBinContent(i,e);
        }
    }

    file.Write();
    file.Close();

    return 0;
}

void printOptions(){
    std::cout << std::endl;
    std::cout << "generateGameFunctionHistograms Usage options:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y (int) comma-separated list of intput years (no default)[Required]" << std::endl;
    std::cout << "\t-o (string) output ROOT file name created in rootFiles directory (no default)[Required]" << std::endl;
    std::cout << "\t-s (double) SRS value (default: \"free\")[Optional]" << std::endl;
    std::cout << "\t-h print this message" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: $CLION/generateGameFunctionHistograms -y 2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015" << std::endl;
    std::cout << "          -s 0.5 -o gameFunctionHistograms.0.5.root" << std::endl;
    std::cout << std::endl;
    std::cout << "This program generates the histograms which are used to determine the" << std::endl;
    std::cout << "likely winning percentage of a team based on their game score.  It requires" << std::endl;
    std::cout << "all of the years for which standard deviations have been defined" << std::endl;
    std::cout << "and the name of the output ROOT file, which will be created in the " << std::endl;
    std::cout << "rootFiles/ directory." << std::endl;
    std::cout << std::endl;
}
