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
#include "TH2F.h"
#include "src/ConstantSRSadditions.h"
#include "src/ConstantGameFunction.h"
#include "helpers/vegasConversion.h"

void printOptions();

int main(int argc,char *argv[]) {
    int c;
    bool writeOutput = false;
    std::vector<std::string> years;
    std::string inYears = "";
    bool useHistogramsFile = false;
    std::string srsValue = "free";
    std::string outFileName = "";
    double sigmas = 3;
    int nbins = 33;
    std::string predictionDay = "";
    std::string endRangeDay = "";
    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:HS:o:s:n:d:D:h")) != -1) {
        switch (c) {
            case 'y':
                inYears.assign(optarg);
                boost::split(years, inYears, boost::is_any_of(","));
                break;
            case 's':
                srsValue.assign(optarg);
                break;
            case 'H':
                useHistogramsFile = true;
                break;
            case 'o':
                outFileName = optarg;
                break;
            case 'S':
                sigmas = atof(optarg);
                break;
            case 'n':
                nbins = atoi(optarg);
                break;
            case 'd':
                predictionDay.assign(optarg);
                break;
            case 'D':
                endRangeDay.assign(optarg);
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

    boost::gregorian::date predictionDate;
    if (predictionDay != "")
        predictionDate = boost::gregorian::date(boost::gregorian::from_string(predictionDay));

    boost::gregorian::date endRangeDate;
    if (endRangeDay != ""){
        endRangeDate = boost::gregorian::date(boost::gregorian::from_string(endRangeDay));
        if (predictionDay == ""){
            std::cout << "If you enter an end date, you must also specify a start date" << std::endl;
            return 0;
        }
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
    std::unordered_map<std::string, TeamGame *> games;
    Team *opp;
    TeamWAverage *wa1, *wa2;

    int gameScoreWins = 0, gameScoreTotal = 0;
    int gameScoreWinsSpread = 0, gameScoreTotalSpread = 0;
    int vegasWinsSpread = 0, vegasTotalSpread = 0;
    int adjGameScoreWins = 0, adjGameScoreTotal = 0;
    int unadjustedSrsWins = 0, unadjustedSrsTotal = 0;
    int srsWins = 0, srsTotal = 0;
    int srsWinsSpread = 0, srsTotalSpread = 0;

    int srsGameScoreWins = 0, srsGameScoreTotal = 0;
    int srsGameScoreWinsSpread = 0, srsGameScoreTotalSpread = 0;

    TFile *outFile;
    if (outFileName != "") {
        sprintf(path, "%s/cpp/NCAA_C/rootFiles/%s",homePath,outFileName.c_str());
        outFile = new TFile(path, "RECREATE");
    }

    TH1F *h_wins = new TH1F("h_wins","",nbins,0,1);
    TH1F *h_total = new TH1F("h_total","",nbins,0,1);
    TH2F *spread_vs_spread = new TH2F("spread_vs_spread","",80,-20,20,80,-20,20);
    TH2F *myspread_vs_pct = new TH2F("myspread_vs_pct","",100,0,1,80,-20,20);
    TH2F *myPct_vs_VegasPct = new TH2F("myPct_vs_VegasPct","",nbins,0,1,nbins,0,1);

    for (auto &team : teams) {
        games = team.second->getGamesByDate();
        for (auto &game : games) {
            //look only at games within a certain range
            if (predictionDay != ""){
                if (endRangeDay == ""){
                    if (predictionDate != game.second->getDate()) continue;
                }
                else{
                    if (game.second->getDate() < predictionDate || game.second->getDate() > endRangeDate) continue;
                }
            }
            else{
                if (game.second->getDate() >= seasonInfo->get(team.second->getYear(), "tournament start")) continue;
                if (game.second->getDate().month().as_number() == 11) continue;
            }
            if (team.first < game.second->getOpp()) continue; //look at each game only once
            opp = Team::findTeam(game.second->getOpp());
            if (!opp) continue;

            TeamGame *oppgame = opp->GameOnDate(game.second->getDate(),team.second->getName());

            bool win = game.second->getWin() == 1;
            double spread = game.second->getSpread();
            double mySpread;

            wa1 = team.second->WAverageOnDate(game.second->getDate());
            wa2 = opp->WAverageOnDate(game.second->getDate());

            std::string loc = game.second->getLoc();
            std::string oppLoc = game.second->getOppLoc();

            double gameScore = gameFunction->predictGame(wa1,wa2,team.second->getYear(),loc,oppLoc);
            if (gameScore >= 0 && win) gameScoreWins++;
            if (gameScore < 0 && !win) gameScoreWins++;
            gameScoreTotal++;

            double gameScorePct = -1;
            double gameScorePct_err = -1;

            double unadjustedSrsVal = wa1->getSrs() - wa2->getSrs();
            if (win && unadjustedSrsVal >= 0) unadjustedSrsWins++;
            if (!win && unadjustedSrsVal < 0) unadjustedSrsWins++;
            unadjustedSrsTotal++;

            double srsVal = wa1->getSrs() - wa2->getSrs() + additions->get(team.second->getYear(),loc);
            if (win && srsVal >= 0) srsWins++;
            if (!win && srsVal < 0) srsWins++;
            srsTotal++;

            if (gameScore >= 0 && srsVal >= 0){
                if (win) srsGameScoreWins++;
                srsGameScoreTotal++;
            }
            if (gameScore < 0 && srsVal < 0){
                if (!win) srsGameScoreWins++;
                srsGameScoreTotal++;
            }

            if (useHistogramsFile){
                if (gameScore >= 3){
                    gameScorePct = 1;
                    gameScorePct_err = 1;
                }
                else if (gameScore <= -3){
                    gameScorePct = 0;
                    gameScorePct_err = 1;
                }
                else{
                    gameScorePct = probs_by_year[team.second->getYear()]->GetBinContent((gameScore+3)*1600/6.0);
                    gameScorePct_err = probs_err_by_year[team.second->getYear()]->GetBinContent((gameScore+3)*1600/6.0);
                }

                if (gameScorePct >= 0.5 && gameScorePct - sigmas*gameScorePct_err >= 0.5 && win) adjGameScoreWins++;
                if (gameScorePct < 0.5 && gameScorePct + sigmas*gameScorePct_err < 0.5 && !win) adjGameScoreWins++;
                if ((gameScorePct >= 0.5 && gameScorePct - sigmas*gameScorePct_err >= 0.5) ||
                    (gameScorePct < 0.5 && gameScorePct + sigmas*gameScorePct_err < 0.5)) adjGameScoreTotal++;

                if (win) h_wins->Fill(gameScorePct);
                h_total->Fill(gameScorePct);

                if (spread != 0 && spread == -1*oppgame->getSpread()){
                    mySpread = pctToSpread(gameScorePct);
                    myspread_vs_pct->Fill(gameScorePct,mySpread);
                    spread_vs_spread->Fill(spread, mySpread);
                    myPct_vs_VegasPct->Fill(spreadToPct(spread),gameScorePct);
                }
            }

            if (spread != -1*oppgame->getSpread()) continue; //if the spreads are somehow messed up

            if (spread != 0){
                gameScoreTotalSpread++;
                if (gameScore > 0 && win) gameScoreWinsSpread++;
                if (gameScore < 0 && !win) gameScoreWinsSpread++;

                srsTotalSpread++;
                if (win && srsVal >= 0) srsWinsSpread++;
                if (!win && srsVal < 0) srsWinsSpread++;

                if (gameScore >= 0 && srsVal >= 0){
                    if (win) srsGameScoreWinsSpread++;
                    srsGameScoreTotalSpread++;
                }
                if (gameScore < 0 && srsVal < 0){
                    if (!win) srsGameScoreWinsSpread++;
                    srsGameScoreTotalSpread++;
                }

                vegasTotalSpread++;
                if (spread < 0 && win) vegasWinsSpread++;
                if (spread > 0 && !win) vegasWinsSpread++;
            }
        }
    }

    std::cout << "All Games" << std::endl;
    std::cout << "gameScore:\t\t" << gameScoreWins << " / " << gameScoreTotal << " = " <<
    doubleFormatter(gameScoreWins / (double) gameScoreTotal, 3) << std::endl;

    if (useHistogramsFile) {
        std::cout << "Games above sigma:\t" << adjGameScoreWins << " / " << adjGameScoreTotal << " = " <<
        doubleFormatter(adjGameScoreWins / (double) adjGameScoreTotal, 3) << std::endl;
    }

    std::cout << "Unadjusted SRS:\t\t" << unadjustedSrsWins << " / " << unadjustedSrsTotal << " = " <<
    doubleFormatter(unadjustedSrsWins / (double) unadjustedSrsTotal, 3) << std::endl;

    std::cout << "Adjusted SRS:\t\t" << srsWins << " / " << srsTotal << " = " <<
    doubleFormatter(srsWins / (double) srsTotal, 3) << std::endl;

    std::cout << "SRS, GameScore agree :\t" << srsGameScoreWins << " / " << srsGameScoreTotal << " = " <<
    doubleFormatter(srsGameScoreWins / (double) srsGameScoreTotal, 3) << std::endl;

    std::cout << std::endl;

    std::cout << "Games with a spread" << std::endl;
    std::cout << "Vegas:\t\t\t" << vegasWinsSpread << " / " << vegasTotalSpread << " = " <<
    doubleFormatter(vegasWinsSpread / (double) vegasTotalSpread,3) << std::endl;

    std::cout << "gameScore:\t\t" << gameScoreWinsSpread << " / " << gameScoreTotalSpread << " = " <<
    doubleFormatter(gameScoreWinsSpread / (double) gameScoreTotalSpread, 3) << std::endl;

    std::cout << "SRS:\t\t\t" << srsWinsSpread << " / " << srsTotalSpread << " = " <<
    doubleFormatter(srsWinsSpread / (double) srsTotalSpread, 3) << std::endl;

    std::cout << "SRS, GameScore agree :\t" << srsGameScoreWinsSpread << " / " << srsGameScoreTotalSpread << " = " <<
    doubleFormatter(srsGameScoreWinsSpread / (double) srsGameScoreTotalSpread, 3) << std::endl;

    if (outFileName != "") {
        outFile->Write();
        outFile->Close();
    }

    return 0;
}

void printOptions(){
    std::cout << std::endl;
    std::cout << "checkGamePredictions Usage options:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y (int) comma-separated list of years (no default)[Required]" << std::endl;
    std::cout << "\t-H use histograms file (no default)[Optional]" << std::endl;
    std::cout << "\t-s (double) SRS value to determine weights file (default: \"free\")[Optional]" << std::endl;
    std::cout << "\t-o (string) write output ROOT file in rootFiles/ with designated name (no default)[Optional]" << std::endl;
    std::cout << "\t-d (date string) check games only on specified date (no default, all games checked)[Optional]" << std::endl;
    std::cout << "\t-D (date string) check games only between -d and -D dates (inclusive) (no default)[Optional, Requires -d set]" << std::endl;
    std::cout << "\t-S (double) set number of sigmas to use (default: 3)[Optional]" << std::endl;
    std::cout << "\t-h print this message" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: $CLION/checkGamePredictions -y 2015 -H" << std::endl;
    std::cout << std::endl;
    std::cout << "This program checks every game in the list of input years using the game" << std::endl;
    std::cout << "function to determine how well it did.  If an SRS value is provided, using the" << std::endl;
    std::cout << "-s option, a different game function weights file is used.  The histograms" << std::endl;
    std::cout << "file is used to convert from the game score to likelihood of winning." << std::endl;
    std::cout << std::endl;
}
