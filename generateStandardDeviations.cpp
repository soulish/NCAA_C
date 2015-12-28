#include <iostream>
#include <fstream>
#include <getopt.h>
#include <unordered_map>
#include <vector>
#include "src/ConstantSeasonInfo.h"
#include "src/Team.h"
#include "src/TeamWAverage.h"
#include "src/readTeams.h"
#include "helpers/doubleFormatter.h"
#include "src/ConstantWAverageFunctions.h"
#include "TF1.h"
#include "TH1F.h"
#include "src/ConstantTeamNeutralRatios.h"
#include "src/ConstantSRSadditions.h"

void printOptions();

int main(int argc,char *argv[]){
    int c;
    int outYear = 0;
    bool verbose = false, writeOutput = false;
    std::vector<std::string> years;
    std::string inYears = "";
    std::string outFileName = "";

    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:Y:vo:h")) != -1) {
        switch (c) {
            case 'y':
                std::cout << "inYears: " << optarg << std::endl;
                inYears.assign(optarg);
                boost::split(years, inYears, boost::is_any_of(","));
                break;
            case 'Y':
                outYear = atoi(optarg);
                break;
            case 'v':
                verbose = true;
                break;
            case 'o':
                outFileName.assign(optarg);
                writeOutput = true;
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
    if (outYear == 0) {
        std::cout << "You must set the output year using the -Y switch" << std::endl;
        printOptions();
        return 0;
    }

    char *homePath, path[256];
    homePath = getenv("HOME");

    sprintf(path, "%s/cpp/NCAA_C/constants/waverage_functions.d", homePath);
    ConstantWAverageFunctions *functions = ConstantWAverageFunctions::Instance();
    functions->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo *seasonInfo = ConstantSeasonInfo::Instance();
    seasonInfo->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/team_neutral_ratios.d", homePath);
    ConstantTeamNeutralRatios *ratios = ConstantTeamNeutralRatios::Instance();
    ratios->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/srs_additions.d", homePath);
    ConstantSRSadditions *additions = ConstantSRSadditions::Instance();
    additions->initialize(path);

    for (std::string &year : years) {
        sprintf(path, "%s/cpp/NCAA_C/teams/%s/", homePath, year.c_str());
        std::cout << "Reading in waverages for " << year << std::endl;
        readTeamsFromDir(path, "games");
        readTeamsFromDir(path, "waverages");
    }

    std::unordered_map<std::string, Team*> teams = Team::getTeams();
    std::unordered_map<std::string, TeamGame*> games;
    Team *opp;
    TeamWAverage *wa1, *wa2;

    std::string stats[] = {"or.p", "efg.p", "ftmr.p", "to.p", "ppp.p"};
    std::unordered_map<std::string, TH1F*> hists;
    for (std::string &s : stats){
        hists.emplace(s, new TH1F(("hist_"+s).c_str(),"",200,-0.5,0.5));
    }
    hists.emplace("srs", new TH1F("hist_srs","",200,-50,50));

    boost::gregorian::days indDuration;

    for (auto &team : teams) {
        games = team.second->getGamesByDate();
        for (auto &game : games) {
            opp = Team::findTeam(game.second->getOpp());
            if (!opp) continue;
            if (game.second->getDate() >= seasonInfo->get(team.second->getYear(), "tournament start")) continue;
            if (game.second->getDate().month().as_number() == 11) continue;

            wa1 = team.second->WAverageOnDate(game.second->getDate());
            wa2 = opp->WAverageOnDate(game.second->getDate());

            indDuration = seasonInfo->get(team.second->getYear(), "tournament start") -
                          game.second->getDate();
            int ind = 124 - (int) indDuration.days();

            std::string loc = game.second->getLoc();

            std::unordered_map<std::string, double> predictions1 = functions->predictStats(wa1, wa2, outYear);
            std::unordered_map<std::string, double> predictions2 = functions->predictStats(wa2, wa1, outYear);

            for (std::string &s : stats){
                hists[s]->Fill(predictions1["o"+s] * wa1->getValue("o"+s) / ratios->get(outYear,game.second->getLoc(),"o"+s) -
                               predictions2["o"+s] * wa2->getValue("o"+s) / ratios->get(outYear,game.second->getOppLoc(),"o"+s));
            }
            hists["srs"]->Fill((wa1->getSrs() - wa2->getSrs() + additions->get(outYear,loc)));
        }
    }

    std::ofstream outFile;
    if (writeOutput) {
        sprintf(path, "%s/cpp/NCAA_C/constants/%s", homePath, outFileName.c_str());
        outFile.open(path, std::ios::app);
        outFile << outYear;
    }

    TF1 *tempFn = new TF1("tempFn","gaus");

    for (std::string &s : stats) {
        hists[s]->Fit(tempFn,"q");
        if (writeOutput)
            outFile << "," << doubleFormatter(tempFn->GetParameter(2), 3);
        if (verbose)
            std::cout << s << "\t" << doubleFormatter(tempFn->GetParameter(2), 3) << std::endl;
    }

    //now deal with SRS
    hists["srs"]->Fit(tempFn,"q");
    if (verbose)
        std::cout << "srs" << "\t" << doubleFormatter(tempFn->GetParameter(2), 3) << std::endl;

    if (writeOutput) {
        outFile << "," << doubleFormatter(tempFn->GetParameter(2), 3);
        outFile << std::endl;
        outFile.close();
    }

    return 0;
}

void printOptions(){//yYvo
    std::cout << std::endl;
    std::cout << "generateStandardDeviations Usage options:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y (int,int,...) comma-separated list of input years (no default)[Required]" << std::endl;
    std::cout << "\t-Y (int) output year (no default)[Required]" << std::endl;
    std::cout << "\t-o (string) output file name, created in constants/ dir (no default)[Optional]" << std::endl;
    std::cout << "\t-v verbose mode [Optional]" << std::endl;
    std::cout << "\t-h print this message" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: $CLION/generateStandardDeviations -y 2010,2011,2012,2013,2014 -Y 2015 -o waverage_standard_deviations.d" << std::endl;
    std::cout << std::endl;
    std::cout << "This program generates the standard deviations necessary for the game functions." << std::endl;
    std::cout << "It takes as input 5 years worth of weighted averages and then outputs the standard" << std::endl;
    std::cout << "deviations for a given year (set with -Y option) either to the terminal or" << std::endl;
    std::cout << "to a file (set by the -o option)." << std::endl;
    std::cout << std::endl;
}
