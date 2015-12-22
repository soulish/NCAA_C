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
    std::string histogramsFileName = "";
    bool useHistogramsFile = false;

    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:H:h")) != -1) {
        switch (c) {
            case 'y':
                year = atoi(optarg);
                break;
            case 'H':
                histogramsFileName.assign(optarg);
                useHistogramsFile = true;
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
    if (year == 0) {
        std::cout << "You must set the year using the -y switch" << std::endl;
        printOptions();
        return 0;
    }
    if (!useHistogramsFile){
        std::cout << "You must supply a histograms file with the -H switch" << std:: endl;
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

    sprintf(path, "%s/cpp/NCAA_C/constants/game_function_weights.d", homePath);
    ConstantGameFunction *gameFunction = ConstantGameFunction::Instance();
    gameFunction->initialize(path);

    //read in the game and waverage info
    sprintf(path, "%s/cpp/NCAA_C/teams/%d/", homePath, year);
    std::cout << "Reading in waverages for " << year << std::endl;
    readTeamsFromDir(path, "waverages");

    sprintf(path, "%s/cpp/NCAA_C/%s", homePath, histogramsFileName.c_str());
    TFile *histsFile = new TFile(path);
    std::unordered_map<int, TH1F*> probs_by_year, probs_err_by_year;

    sprintf(path, "probs_by_year_%d", year);
    probs_by_year.emplace(year, (TH1F*)gROOT->FindObject(path));
    sprintf(path, "probs_err_by_year_%d", year);
    probs_err_by_year.emplace(year, (TH1F*)gROOT->FindObject(path));

    Tournament tournament(year);
    sprintf(path, "%s/cpp/NCAA_C/tournamentInfo/tournament_%d.d", homePath, year);
    tournament.readTournamentInfo(path);
    tournament.play(probs_by_year[year]);

    return 0;
}

void printOptions(){
    std::cout << std::endl;
    std::cout << "Usage options:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y year of the tournament to simulate (no default)[Required]" << std::endl;
    std::cout << "\t-H histogramsFileName (no default)[Optional]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: $CLION/tournamentSimulation -y 2015 -H rootFiles/gameFunctionHistograms.root" << std::endl;
    std::cout << std::endl;
    std::cout << "This program will simulate the NCAA tournament for a given year (must be" << std::endl;
    std::cout << "between 2007 and 2015 (currently)), using a histograms file to calculate" << std::endl;
    std::cout << "the winning percentages.  The results will be output on the screen." << std::endl;
    std::cout << std::endl;
}
