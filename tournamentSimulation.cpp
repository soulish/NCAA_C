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
#include "src/ConstantSeasonInfo.h"
#include "src/ConstantStandardDeviations.h"
#include "src/ConstantWAverageFunctions.h"
#include "TH1F.h"
#include "src/ConstantSRSadditions.h"
#include "src/ConstantGameFunction.h"
#include "src/ConstantTeam5YearAverages.h"
#include "src/Tournament.h"

void printOptions();

int main(int argc,char *argv[]) {
    int c;
    int year = 0;
    bool calcTotalPercentages = false;
    std::string srsValue = "free";

    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:H:ts:h")) != -1) {
        switch (c) {
            case 'y':
                year = atoi(optarg);
                break;
            case 'h':
                printOptions();
                return 0;
            case 't':
                calcTotalPercentages = true;
                break;
            case 's':
                srsValue.assign(optarg);
                break;
            default:
                std::cout << "You provided an unknown option.  Please try again." << std::endl;
                printOptions();
                return 0;
        }
    }

    //ensure years and outYear are set
    if (year == 0) {
        std::cout << "You must set the year using the -y switch" << std::endl;
        printOptions();
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
    sprintf(path, "%s/cpp/NCAA_C/teams/%d/", homePath, year);
    std::cout << "Reading in waverages for " << year << std::endl;
    readTeamsFromDir(path, "waverages");

    sprintf(path, "%s/cpp/NCAA_C/rootFiles/gameFunctionHistograms.%s.root", homePath, srsValue.c_str());
    TFile *histsFile = new TFile(path);
    std::unordered_map<int, TH1F*> probs_by_year, probs_err_by_year;

    sprintf(path, "probs_by_year_%d", year);
    probs_by_year.emplace(year, (TH1F*)gROOT->FindObject(path));
    sprintf(path, "probs_err_by_year_%d", year);
    probs_err_by_year.emplace(year, (TH1F*)gROOT->FindObject(path));

    Tournament tournament(year);
    sprintf(path, "%s/cpp/NCAA_C/tournamentInfo/tournament_%d.d", homePath, year);
    tournament.readTournamentInfo(path);
    if (calcTotalPercentages)
        tournament.calculateTotalPercentages(probs_by_year[year]);
    else
        tournament.play(probs_by_year[year]);

    return 0;
}

void printOptions(){
    std::cout << std::endl;
    std::cout << "tournamentSimulation Usage options:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y (int) year of the tournament to simulate (no default)[Required]" << std::endl;
    std::cout << "\t-s (double) srsValue which determines which weights file to use (default: \"free\")[Optional]" << std::endl;
    std::cout << "\t-t calculate percentages for each team throughout the tournament [Optional]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: $CLION/tournamentSimulation -y 2015 -s 0.5" << std::endl;
    std::cout << std::endl;
    std::cout << "This program will simulate the NCAA tournament for a given year (must be" << std::endl;
    std::cout << "between 2007 and 2015 (currently)), using a histograms file to calculate" << std::endl;
    std::cout << "the winning percentages.  The results will be output on the screen." << std::endl;
    std::cout << "The -t option allows the user to calculate the odds of each team reaching" << std::endl;
    std::cout << "each round of the tournament as well as winning the whole tournament." << std::endl;
    std::cout << std::endl;
}
