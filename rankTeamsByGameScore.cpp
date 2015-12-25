#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <boost/foreach.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <TFile.h>
#include <TROOT.h>
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
#include "src/ConstantTeam5YearAverages.h"

void printOptions();

int main(int argc,char *argv[]) {
    int c;
    std::vector<std::string> years;
    std::string inYears = "";
    bool useHistogramsFile = false;
    std::string srsValue = "free";
    int chosenYear = 0;

    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:Hs:Y:h")) != -1) {
        switch (c) {
            case 'y':
                inYears.assign(optarg);
                boost::split(years, inYears, boost::is_any_of(","));
                break;
            case 'H':
                useHistogramsFile = true;
                break;
            case 's':
                srsValue.assign(optarg);
                break;
            case 'Y':
                chosenYear = atoi(optarg);
                break;
            case 'h':
                printOptions();
                return 0;
            default:
                std::cout << "You provided an unknown option.  Please try again." << std::endl;
                printOptions();
                return 0;
        }
    }

    //ensure years and outYear are set
    if (years.empty()) {
        std::cout << "You must set the input years using the -y switch and a comma-separated list of years" <<
        std::endl;
        return 0;
    }

    char *homePath, path[256];
    homePath = getenv("HOME");

    //read in the necessary constants
    sprintf(path, "%s/cpp/NCAA_C/constants/team_5yr_averages.d", homePath);
    ConstantTeam5YearAverages *averages = ConstantTeam5YearAverages::Instance();
    averages->initialize(path);

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
        std::cout << "Reading in waverages for " << year << std::endl;
        readTeamsFromDir(path, "waverages");
    }

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

    std::unordered_map<std::string, Team *> teams = Team::getTeams();
    TeamWAverage *wa1;

    std::unordered_map<double, std::string> gameScores;
    std::unordered_map<std::string, double> gameScorePcts;

    for (auto &team : teams) {
        wa1 = team.second->WAverageOnDate(seasonInfo->get(team.second->getYear(), "tournament start"));
        double gameScore = gameFunction->predictGame(wa1,
                                                     chosenYear == 0 ? team.second->getYear() : chosenYear,
                                                     "neutral","neutral");

        if (gameScores.find(gameScore) != gameScores.end())
            std::cout << gameScore << "\t" << team.first << std::endl;
        else
            gameScores.emplace(gameScore, team.first);

        if (useHistogramsFile) {
            double gameScorePct = -1;
            if (gameScore >= 3)
                gameScorePct = 1;
            else if (gameScore <= -3)
                gameScorePct = 0;
            else
                gameScorePct = probs_by_year[chosenYear == 0 ? team.second->getYear() : chosenYear]->
                        GetBinContent((gameScore + 3) * 1600 / 6.0);

            gameScorePcts.emplace(team.first, gameScorePct);
        }
    }

    std::map<double, std::string> sortedGameScores(gameScores.begin(), gameScores.end());

    if (useHistogramsFile) {
        int i = 1;
        for (std::map<double, std::string>::reverse_iterator it = sortedGameScores.rbegin();
             it != sortedGameScores.rend(); ++it) {
            std::cout << i << "\t" << doubleFormatter(it->first, 4) << "\t\t" <<
                    doubleFormatter(gameScorePcts[it->second],3) << "\t\t" <<
                    it->second << '\n';
            i++;
        }
    }
    else{
        int i = 1;
        for (std::map<double, std::string>::reverse_iterator it = sortedGameScores.rbegin();
             it != sortedGameScores.rend(); ++it) {
            std::cout << i << "\t" << doubleFormatter(it->first, 4) << "\t\t" << it->second << '\n';
            i++;
        }
    }

    return 0;
}

void printOptions(){
    std::cout << std::endl;
    std::cout << "rankTeamsByGameScore Usage options:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y (int,int,...) comma-separated list of years to use (no default)[Required]" << std::endl;
    std::cout << "\t-Y (int) year to use for calculations (no default)[Optional]" << std::endl;
    std::cout << "\t-s (double) srs value to use for weights and histograms (default: \"fixed\")[Optional]" << std::endl;
    std::cout << "\t-H calculate game Score percentages using histograms file (no default)[Optional]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: $CLION/rankTeamsByGameScore -y 2014,2015 -H -s 0.5 -Y 2007" << std::endl;
    std::cout << std::endl;
    std::cout << "This program ranks all of the teams in the list of years provided by their gameScore" << std::endl;
    std::cout << "as it is calculated on the day the NCAA tournament began in that year" << std::endl;
    std::cout << "using either the constants appropriate for the year of the team, or a specific year" << std::endl;
    std::cout << "which can be provided using the -Y switch.  In the example, all the teams from " << std::endl;
    std::cout << "2014 and 2015 will be ranked using 2015's constants.  If a histogram file is provided" << std::endl;
    std::cout << "then the winning percentage will also be output." << std::endl;
    std::cout << std::endl;
}