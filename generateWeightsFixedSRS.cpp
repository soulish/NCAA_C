#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <boost/foreach.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <TFile.h>
#include <TMinuit.h>
#include "src/ConstantTeam5YearAverages.h"
#include "src/ConstantTeamNeutralRatios.h"
#include "src/ConstantTeamPointDifferentials.h"
#include "src/ConstantTeamWinRatios.h"
#include "src/Team.h"
#include "src/TeamGame.h"
#include "src/Pcts.h"
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

std::vector<int> win;
std::vector<double> oor;
std::vector<double> oefg;
std::vector<double> oftmr;
std::vector<double> oto;
std::vector<double> srs;

void fcn(int& num_par, double* grad, double& f, double pars[], int flag);
std::vector<double> run_fit(double parOOR, double parOEFG, double parOFTMR,
                            double parOTO, double parSRS);
void printOptions();

int main(int argc,char *argv[]) {
    int c;
    bool writeOutput = false;
    std::string outFileName = "";
    std::vector<std::string> years;
    std::string inYears = "";
    int numIterations = 1;
    double srsWeight = -1;
    int outYear = 0;
    bool useSeededValues = false;
    std::string seededLocation = "";
    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:Y:i:s:o:S:h")) != -1) {
        switch (c) {
            case 'y':
                inYears.assign(optarg);
                boost::split(years, inYears, boost::is_any_of(","));
                break;
            case 'Y':
                outYear = atoi(optarg);
                break;
            case 'o':
                outFileName.assign(optarg);
                writeOutput = true;
                break;
            case 'i':
                numIterations = atoi(optarg);
                break;
            case 's':
                srsWeight = atof(optarg);
                break;
            case 'S':
                useSeededValues = true;
                seededLocation.assign(optarg);
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
    if (!useSeededValues && srsWeight == -1){
        std::cout << "You must set the srsWeight using the -s switch" << std::endl;
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

    //read in the game and waverage info
    for (std::string &year : years) {
        sprintf(path, "%s/cpp/NCAA_C/teams/%s/", homePath, year.c_str());
        std::cout << "Reading in games and waverages for " << year << std::endl;
        readTeamsFromDir(path);
        readTeamsFromDir(path, "waverages");
    }

    std::unordered_map<std::string, Team*> teams = Team::getTeams();
    std::unordered_map<std::string, TeamGame*> games;
    Team *opp;
    TeamWAverage *wa1, *wa2;
    int totalGames = 0;

    for (auto &team : teams) {
        games = team.second->getGamesByDate();
        for (auto &game : games) {
            if (team.first < game.second->getOpp()) continue; //look at each game only once
            opp = Team::findTeam(game.second->getOpp());
            if (!opp) continue;
            if (game.second->getDate() >= seasonInfo->get(team.second->getYear(), "tournament start")) continue;
            if (game.second->getDate().month().as_number() == 11) continue;

            totalGames++;

            wa1 = team.second->WAverageOnDate(game.second->getDate());
            wa2 = opp->WAverageOnDate(game.second->getDate());

            std::string loc = game.second->getLoc();
            std::string oppLoc = game.second->getOppLoc();

            std::unordered_map<std::string, double> predictions1 = functions->predictStats(wa1, wa2, outYear);
            std::unordered_map<std::string, double> predictions2 = functions->predictStats(wa2, wa1, outYear);

            win.push_back(game.second->getWin());
            oor.push_back((predictions1["oor.p"] * wa1->getValue("oor.p") / ratios->get(outYear,loc,"oor.p") -
                           predictions2["oor.p"] * wa2->getValue("oor.p") / ratios->get(outYear,oppLoc,"oor.p")) /
                          stdDevs->get(outYear,"oor.p"));

            oefg.push_back((predictions1["oefg.p"] * wa1->getValue("oefg.p") / ratios->get(outYear,loc,"oefg.p") -
                            predictions2["oefg.p"] * wa2->getValue("oefg.p") / ratios->get(outYear,oppLoc,"oefg.p")) /
                           stdDevs->get(outYear,"oefg.p"));

            oftmr.push_back((predictions1["oftmr.p"] * wa1->getValue("oftmr.p") / ratios->get(outYear,loc,"oftmr.p") -
                             predictions2["oftmr.p"] * wa2->getValue("oftmr.p") / ratios->get(outYear,oppLoc,"oftmr.p")) /
                            stdDevs->get(outYear,"oftmr.p"));

            //This guy is reversed because turnovers are bad.
            oto.push_back((-predictions1["oto.p"] * wa1->getValue("oto.p") / ratios->get(outYear,loc,"oto.p") +
                           predictions2["oto.p"] * wa2->getValue("oto.p") / ratios->get(outYear,oppLoc,"oto.p")) /
                          stdDevs->get(outYear,"oto.p"));

            srs.push_back((wa1->getSrs() - wa2->getSrs() + additions->get(outYear,loc)) /
                          stdDevs->get(outYear,"srs"));
        }
    }

    std::cout << "Total number of games to analyze: " << totalGames << std::endl;

    std::vector<double> temp_ary, temp_ary2;
    std::vector<double> fcn_mins;
    std::vector< std::vector<double>*> params_ary;

    if (!useSeededValues) {
        //random number generator, using Mersenne Twister method
        //the 0 means we use a unique seed each time
        TRandom3 rand(0);

        for (int i = 0; i < numIterations; i++) {
            double randoms[4];
            rand.RndmArray(4, randoms);

            double norm = (1 - srsWeight) / (randoms[0] + randoms[1] + randoms[2] + randoms[3]);

            temp_ary = run_fit(randoms[0] * norm, randoms[1] * norm, randoms[2] * norm,
                               randoms[3] * norm, srsWeight);

            double temp_fcn_min = temp_ary.back();
            double temp_fcn_min2;

            double temp_norm = 0;
            int num_times_through = 0;
            for (int k = 0; k < 100; k++) {
                num_times_through++;
                temp_norm = (1 - temp_ary[4]) / (temp_ary[0] + temp_ary[1] + temp_ary[2] + temp_ary[3]);
                temp_ary2 = run_fit(temp_ary[0] * temp_norm, temp_ary[1] * temp_norm, temp_ary[2] * temp_norm,
                                    temp_ary[3] * temp_norm, temp_ary[4]);
                temp_fcn_min2 = temp_ary2.back();
                if (temp_fcn_min2 == temp_fcn_min && temp_ary == temp_ary2) break;

                temp_ary = temp_ary2;
                temp_fcn_min = temp_fcn_min2;
            }

            fcn_mins.push_back(temp_fcn_min);
            params_ary.push_back(new std::vector<double>());
            for (int ii = 0; ii < temp_ary.size(); ii++)
                params_ary.back()->push_back(temp_ary[ii]);

            int indexOfMin = indexOfVectorMin(fcn_mins);
            std::cout << "  " << i << "\t" << doubleFormatter(fcn_mins.back(), 4) << "\t" <<
            doubleFormatter(fcn_mins[indexOfMin], 4) << "\t" << indexOfMin << "\r";
            fflush(stdout);
        }
    }
    else{
        sprintf(path, "%s/cpp/NCAA_C/%s", homePath, seededLocation.c_str());
        ConstantGameFunction *gameFunction = ConstantGameFunction::Instance();
        gameFunction->initialize(path);

        std::vector<int> keys = gameFunction->getKeys();
        for (int &year : keys){
            std::vector<double> initialValuesVec = gameFunction->getWeights(year);

            temp_ary = run_fit(initialValuesVec[0],initialValuesVec[1],initialValuesVec[2],
                               initialValuesVec[3],initialValuesVec[4]);

            double temp_fcn_min = temp_ary.back();
            double temp_fcn_min2;

            double temp_norm = 0;
            int num_times_through = 0;
            for (int k = 0; k < 100; k++) {
                num_times_through++;
                temp_norm = (1 - temp_ary[4]) / (temp_ary[0] + temp_ary[1] + temp_ary[2] + temp_ary[3]);
                temp_ary2 = run_fit(temp_ary[0] * temp_norm, temp_ary[1] * temp_norm, temp_ary[2] * temp_norm,
                                    temp_ary[3] * temp_norm, temp_ary[4]);
                temp_fcn_min2 = temp_ary2.back();
                if (temp_fcn_min2 == temp_fcn_min && temp_ary == temp_ary2) break;

                temp_ary = temp_ary2;
                temp_fcn_min = temp_fcn_min2;
            }

            fcn_mins.push_back(temp_fcn_min);
            params_ary.push_back(new std::vector<double>());
            for (int ii = 0; ii < temp_ary.size(); ii++)
                params_ary.back()->push_back(temp_ary[ii]);

            int indexOfMin = indexOfVectorMin(fcn_mins);
            std::cout << "  " << year << "\t" << doubleFormatter(fcn_mins.back(), 4) << "\t" <<
            doubleFormatter(fcn_mins[indexOfMin], 4) << "\t" << indexOfMin << std::endl;

            for (int ii = 0; ii < 5; ii++)
                std::cout << ii << "\t" << doubleFormatter(temp_ary[ii],4) << std::endl;

        }
    }

    std::cout << "\n\n\nFinal Results";
    int indexOfMin = indexOfVectorMin(fcn_mins);
    std::cout << "Minimum = " << doubleFormatter(fcn_mins[indexOfMin],4) << std::endl;
    for (int i = 0; i < 5; i++)
        std::cout << i << "\t" << doubleFormatter(params_ary[indexOfMin]->at(i),4) << std::endl;

    if (writeOutput) {
        sprintf(path, "%s/cpp/NCAA_C/constants/%s", homePath, outFileName.c_str());
        std::ofstream outFile(path, std::ios::app);
        outFile << outYear;
        for (int i = 0; i < 6; i++)
            outFile << "," << doubleFormatter(params_ary[indexOfMin]->at(i),4);
        outFile << std::endl;
        outFile.close();
    }

    return 0;
}

void fcn(int& num_par, double* grad, double& f, double pars[], int flag) {
    double value = 1;
    int wins = 0, total = 0;
    int num_events = (int) win.size();

    double norm = (1-pars[4]) / (pars[0] + pars[1] + pars[2] + pars[3]);

    for (int i = 0; i < num_events; i++){
        value = (pars[0] * oor.at(i) +
                 pars[1] * oefg.at(i) +
                 pars[2] * oftmr.at(i) +
                 pars[3] * oto.at(i)) * norm +
                pars[4] * srs.at(i);
        total++;
        if (value > 0){
            if (win.at(i) == 1) wins++;
        }
        else{
            if (win.at(i) == 0) wins++;
        }
    }

    f = -wins / (double) total;//f is essentially the return value
}

std::vector<double> run_fit(double parOOR, double parOEFG, double parOFTMR,
                            double parOTO, double parSRS) {
    TMinuit minuit;
    double arglist[10];
    int iflag;
    arglist[0] = -1;
    minuit.mnexcm("SET PRINT",arglist,1,iflag);
    minuit.mnexcm("SET NOW",arglist,0,iflag);
    minuit.SetFCN(fcn);
    arglist[0] = 1;
    minuit.mnexcm("SET STR",arglist,1,iflag);
    minuit.mnparm(0,"parOOR",parOOR,0.1,0,100,iflag);//or
    minuit.mnparm(1,"parEFG",parOEFG,0.1,0,100,iflag);//efgp
    minuit.mnparm(2,"parFTMR",parOFTMR,0.1,0,100,iflag);//ftr
    minuit.mnparm(3,"parTO",parOTO,0.1,0,100,iflag);//top
    minuit.mnparm(4,"parSRS",parSRS,0.1,0,100,iflag);//srs

    minuit.FixParameter(4);//lock srs

    arglist[0] = 10000;
    arglist[1] = 0.0001;
    minuit.mnexcm("MIGRAD",arglist,2,iflag);
    minuit.mnexcm("HESSE",arglist,0,iflag);

    double fmin, fedm,errdef;
    int npari, nparx, istat;
    minuit.mnstat(fmin,fedm,errdef,npari,nparx,istat);

    double val,err;
    std::vector<double> ret_ary;
    for(int p = 0; p < 5; p++) {
        minuit.GetParameter(p, val, err);
        ret_ary.push_back(val);
    }
    ret_ary.push_back(fmin);

    return ret_ary;
}

void printOptions(){
    std::cout << std::endl;
    std::cout << "generateWeightsFixedSRS Usage options:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y (int,int,...) comma-separated list of input years (no default)[Required]" << std::endl;
    std::cout << "\t-Y (int) year to generate weights for (no default)[Required]" << std::endl;
    std::cout << "\t-i (int) number of fit iterations (default: 1)[Optional]" << std::endl;
    std::cout << "\t-o (string) output file name to be created in constants directory (no default)[Optional]" << std::endl;
    std::cout << "\t-s (int) fixed value for SRS (no default)[Required unless -S option used]" << std::endl;
    std::cout << "\t-S (string) seed file to read starting values from (no default)[Optional]" << std::endl;
    std::cout << "\t-h print this message" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: $CLION/generateWeightsFixedSRS -y 2010,2011,2012,2013,2014 -Y 2015 -i 500" << std::endl;
    std::cout << "                   -s 0.5 -o game_function_weights.d" << std::endl;
    std::cout << std::endl;
    std::cout << "This program determines the best weights to use to maximize the number" << std::endl;
    std::cout << "of games correctly predicted by the game function.  It takes as input" << std::endl;
    std::cout << "5 seasons worth of weighted averages, and the associated standard deviations" << std::endl;
    std::cout << "and outputs the set of weights that has the highest percentage of correct" << std::endl;
    std::cout << "games over the number of iterations chosen." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "The SRS value is locked to a particular value which can be chosen using" << std::endl;
    std::cout << "the -s switch." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "By default, a fresh set of 4 random starting values are generated for each" << std::endl;
    std::cout << "iteration and then a fit is run starting from those values.  But if you" << std::endl;
    std::cout << "wish to, you can enter a (properly formatted) file with set starting values" << std::endl;
    std::cout << "using the -S option.  If you use the -S option, you don't need to specify" << std::endl;
    std::cout << "the SRS value." << std::endl;
    std::cout << std::endl;
}
