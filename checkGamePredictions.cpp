#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <boost/foreach.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <TFile.h>
#include <TMinuit.h>
#include "src/ConstantTeamNeutralRatios.h"
#include "src/Team.h"
#include "src/readTeams.h"
#include "helpers/doubleFormatter.h"
#include "src/ConstantSeasonInfo.h"
#include "src/ConstantStandardDeviations.h"
#include "src/ConstantWAverageFunctions.h"
#include "TH1F.h"
#include "TRandom3.h"
#include "src/ConstantSRSadditions.h"
#include "helpers/vectorMinMax.h"
#include "src/ConstantGameFunction.h"

int main(int argc,char *argv[]) {
    int c;
    bool writeOutput = false;
    std::string outFileName = "";
    std::vector<std::string> years;
    std::string inYears = "";
    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:o:")) != -1) {
        switch (c) {
            case 'y':
                inYears.assign(optarg);
                boost::split(years, inYears, boost::is_any_of(","));
                break;
            case 'o':
                outFileName.assign(optarg);
                writeOutput = true;
                break;
            default:
                // not an option
                break;
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

    int gameScoreWins = 0, gameScoreTotal = 0;
    int gameScoreWinsSpread = 0, gameScoreTotalSpread = 0;
    int vegasWinsSpread = 0, vegasTotalSpread = 0;

    for (auto &team : teams) {
        games = team.second->getGamesByDate();
        for (auto &game : games) {
            if (team.first < game.second->getOpp()) continue; //look at each game only once
            opp = Team::findTeam(game.second->getOpp());
            if (!opp) continue;
            if (game.second->getDate() >= seasonInfo->get(team.second->getYear(), "tournament start")) continue;
            if (game.second->getDate().month().as_number() == 11) continue;

            bool win = game.second->getWin() == 1;
            double spread = game.second->getSpread();

            wa1 = team.second->WAverageOnDate(game.second->getDate());
            wa2 = opp->WAverageOnDate(game.second->getDate());

            std::string loc = game.second->getLoc();
            std::string oppLoc = game.second->getOppLoc();

            double gameScore = gameFunction->predictGame(wa1,wa2,team.second->getYear(),loc,oppLoc);

            gameScoreTotal++;
            if (gameScore > 0 && win) gameScoreWins++;
            if (gameScore < 0 && !win) gameScoreWins++;

            if (spread != 0){
                gameScoreTotalSpread++;
                if (gameScore > 0 && win) gameScoreWinsSpread++;
                if (gameScore < 0 && !win) gameScoreWinsSpread++;

                vegasTotalSpread++;
                if (spread < 0 && win) vegasWinsSpread++;
                if (spread > 0 && !win) vegasWinsSpread++;
            }
        }
    }

    std::cout << "All games:\t\t" << gameScoreWins << " / " << gameScoreTotal << " = " <<
    doubleFormatter(gameScoreWins / (double) gameScoreTotal, 3) << std::endl;

    std::cout << "Games with Spread:\t" << gameScoreWinsSpread << " / " << gameScoreTotalSpread << " = " <<
    doubleFormatter(gameScoreWinsSpread / (double) gameScoreTotalSpread, 3) << std::endl;

    std::cout << "Vegas precitions:\t" << vegasWinsSpread << " / " << vegasTotalSpread << " = " <<
    doubleFormatter(vegasWinsSpread / (double) vegasTotalSpread,3) << std::endl;

    return 0;
}