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
#include "src/ConstantFunctions.h"
#include "TH1F.h"
#include "TRandom3.h"
#include "src/ConstantSRSadditions.h"
#include "helpers/vectorMinMax.h"

std::vector<int> win;
std::vector<double> oor;
std::vector<double> oefg;
std::vector<double> oftmr;
std::vector<double> oto;
std::vector<double> srs;

void fcn(int& num_par, double* grad, double& f, double pars[], int flag);
std::vector<double> run_fit(double parOOR, double parOEFG, double parOFTMR,
                            double parOTO, double parSRS);

int main(int argc,char *argv[]) {
    int c;
    bool verbose = false;
    std::vector<std::string> years;
    std::string inYears = "";
    int numIterations = 1;
    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:vi:")) != -1) {
        switch (c) {
            case 'y':
                inYears.assign(optarg);
                boost::split(years, inYears, boost::is_any_of(","));
                break;
            case 'v':
                verbose = true;
                break;
            case 'i':
                numIterations = atoi(optarg);
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
    ConstantFunctions *functions = ConstantFunctions::Instance();
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

            boost::gregorian::days indDuration = seasonInfo->get(team.second->getYear(), "tournament start") -
                                                 wa1->getDate();
            int ind = 129 - (int) indDuration.days();
            int gameYear =team.second->getYear();
            std::string loc = game.second->getLoc();

            std::unordered_map<std::string, double> predictions1 =
                    ConstantFunctions::Instance()->predict(wa1,wa2,gameYear);
            std::unordered_map<std::string, double> predictions2 =
                    ConstantFunctions::Instance()->predict(wa2,wa1,gameYear);

            win.push_back(game.second->getWin());
            oor.push_back((predictions1["oor.p"] / ratios->get(gameYear,loc,"oor.p") -
                           predictions2["oor.p"] / ratios->get(gameYear, loc,"oor.p")) /
                          stdDevs->get(gameYear,"oor",ind));

            oefg.push_back((predictions1["oefg.p"] / ratios->get(gameYear,loc,"oefg.p") -
                            predictions2["oefg.p"] / ratios->get(gameYear, loc,"oefg.p")) /
                           stdDevs->get(gameYear,"oefg",ind));

            oftmr.push_back((predictions1["oftmr.p"] / ratios->get(gameYear,loc,"oftmr.p") -
                             predictions2["oftmr.p"] / ratios->get(gameYear, loc,"oftmr.p")) /
                            stdDevs->get(gameYear,"oftmr",ind));

            oto.push_back((predictions1["oto.p"] / ratios->get(gameYear,loc,"oto.p") -
                           predictions2["oto.p"] / ratios->get(gameYear, loc,"oto.p")) /
                          stdDevs->get(gameYear,"oto",ind));

            srs.push_back((wa1->getSrs() - wa2->getSrs() + additions->get(gameYear,loc)) /
                          stdDevs->get(gameYear,"srs",ind));
        }
    }

    std::cout << "Total number of games to analyze: " << totalGames << std::endl;

    std::vector<double> temp_ary, temp_ary2;
    std::vector<double> fcn_mins;
    std::vector< std::vector<double>*> params_ary;

    //random number generator, using Mersenne Twister method
    //the 0 means we use a unique seed each time
    TRandom3 rand(0);

    for (int i = 0; i < numIterations; i++){
        double randoms[5];
        rand.RndmArray(5,randoms);

        double rand_norm = 0;
        for (int j = 0; j < 5; j++)
            rand_norm += randoms[j];

        temp_ary = run_fit(randoms[0]/rand_norm, randoms[1]/rand_norm, randoms[2]/rand_norm,
                           randoms[3]/rand_norm, randoms[4]/rand_norm);

        double temp_fcn_min = temp_ary.back();
        double temp_fcn_min2;

        double temp_norm = 0;
        int num_times_through = 0;
        for (int k = 0; k < 100; k++){
            num_times_through++;
            temp_norm = 0;
            for (int j = 0; j < 5; j++)
                temp_norm += temp_ary[j];
            temp_ary2 = run_fit(temp_ary[0] / temp_norm, temp_ary[1] / temp_norm, temp_ary[2] / temp_norm,
                                temp_ary[3] / temp_norm, temp_ary[4] / temp_norm);
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
        std::cout << "  " << i << "\t" << doubleFormatter(fcn_mins.back(),4) << "\t" <<
                doubleFormatter(fcn_mins[indexOfMin],4) << "\t" << indexOfMin << "\r";
        fflush(stdout);
    }

    std::cout << "\n\n\nFinal Results";
    int indexOfMin = indexOfVectorMin(fcn_mins);
    std::cout << "Minimum = " << doubleFormatter(fcn_mins[indexOfMin],4) << std::endl;
    for (int i = 0; i < 5; i++)
        std::cout << i << "\t" << doubleFormatter(params_ary[indexOfMin]->at(i),4) << std::endl;

    return 0;
}

void fcn(int& num_par, double* grad, double& f, double pars[], int flag) {
    double value = 1;
    int wins = 0, total = 0;
    int num_events = (int) win.size();

//    double norm = (1-pars[4]) / (pars[0] + pars[1] + pars[2] + pars[3]);
//    double norm = pars[0] + pars[1] + pars[2] + pars[3];

    for (int i = 0; i < num_events; i++){
        value = pars[0] * oor.at(i) +
                pars[1] * oefg.at(i) +
                pars[2] * oftmr.at(i) +
                pars[3] * oto.at(i) +
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