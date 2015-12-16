#include <iostream>
#include <unordered_map>
#include <map>
#include <boost/foreach.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <TFile.h>
#include <TMinuit.h>
#include "TH1F.h"
#include "TRandom3.h"
#include "src/Team.h"
#include "src/TeamGame.h"
#include "src/Pcts.h"
#include "src/readTeams.h"
#include "helpers/doubleFormatter.h"
#include "src/ConstantSeasonInfo.h"
#include "helpers/vectorMinMax.h"

std::vector<int> win;
std::vector<double> location;
std::vector<double> srs;

void fcn(int& num_par, double* grad, double& f, double pars[], int flag);
std::vector<double> run_fit(double parSRS);

int main(int argc,char *argv[]) {
    int c;
    std::vector<std::string> years;
    std::string inYears = "";
    int numIterations = 1;
    std::string outFileName = "";
    bool writeOutput = false;
    int outYear = 0;

    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:Y:i:o:")) != -1) {
        switch (c) {
            case 'y':
                inYears.assign(optarg);
                boost::split(years, inYears, boost::is_any_of(","));
                break;
            case 'Y':
                outYear = atoi(optarg);
                break;
            case 'i':
                numIterations = atoi(optarg);
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
    if (outYear == 0) {
        std::cout << "You must set the output year using the -Y switch" << std::endl;
        return 0;
    }

    char *homePath, path[256];
    homePath = getenv("HOME");

    //read in the necessary constants
    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo *seasonInfo = ConstantSeasonInfo::Instance();
    seasonInfo->initialize(path);

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

    std::cout << "Processing Games" << std::endl;
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

            int loc = 0;
            if (game.second->getLoc() == "home") loc = 1;
            else if (game.second->getLoc() == "away") loc = -1;

            win.push_back(game.second->getWin());
            location.push_back(loc);
            srs.push_back((wa1->getSrs() - wa2->getSrs()));
        }
    }

    std::cout << "Total number of games to analyze: " << totalGames << std::endl;

    std::vector<double> temp_ary, temp_ary2;
    std::vector<double> fcn_min;
    std::vector< std::vector<double>*> params_ary;

    //random number generator, using Mersenne Twister method
    //the 0 means we use a unique seed each time
    TRandom3 rand(0);

    for (int i = 0; i < numIterations; i++){
        temp_ary = run_fit(rand.Rndm()*40 - 20);//random number between -20 and 20

        double temp_fcn_min = temp_ary.back();
        double temp_fcn_min2;

        int num_times_through = 0;
        for (int k = 0; k < 100; k++){
            num_times_through++;
            temp_ary2 = run_fit(temp_ary[0]);
            temp_fcn_min2 = temp_ary2.back();
            if (temp_fcn_min2 == temp_fcn_min && temp_ary == temp_ary2) break;

            temp_ary = temp_ary2;
            temp_fcn_min = temp_fcn_min2;
        }

        fcn_min.push_back(temp_fcn_min);
        params_ary.push_back(new std::vector<double>());
        for (int ii = 0; ii < temp_ary.size(); ii++)
            params_ary.back()->push_back(temp_ary[ii]);
    }

    int indexOfMin = indexOfVectorMin(fcn_min);
    std::cout << indexOfMin << "\t" << fcn_min[indexOfMin] << "\t" <<
            params_ary[indexOfMin]->at(0) << std::endl;

    if (writeOutput) {
        sprintf(path, "%s/cpp/NCAA_C/constants/%s", homePath, outFileName.c_str());
        std::ofstream outFile(path, std::ios::app);
        outFile << outYear << "," << doubleFormatter(params_ary[indexOfMin]->at(0), 2) << std::endl;
        outFile.close();
    }

    return 0;
}

void fcn(int& num_par, double* grad, double& f, double pars[], int flag) {
    double value = 1;
    int wins = 0, total = 0;
    int num_events = (int) win.size();

    for (int i = 0; i < num_events; i++){
        value = srs[i] + pars[0]*location[i];
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

std::vector<double> run_fit(double parSRS){
    TMinuit minuit;
    double arglist[10];
    int iflag;
    arglist[0] = -1;
    minuit.mnexcm("SET PRINT",arglist,1,iflag);
    minuit.mnexcm("SET NOW",arglist,0,iflag);
    minuit.SetFCN(fcn);
    minuit.mnexcm("SET STR",arglist,1,iflag);
    minuit.mnparm(0,"parSRS",parSRS,0.1,-100,100,iflag);

    arglist[0] = 10000;
    arglist[1] = 0.01;
    minuit.mnexcm("MIGRAD",arglist,2,iflag);
    minuit.mnexcm("HESSE",arglist,0,iflag);

    double fmin, fedm,errdef;
    int npari, nparx, istat;
    minuit.mnstat(fmin,fedm,errdef,npari,nparx,istat);

    double val,err;
    std::vector<double> ret_ary;
    for(int p = 0; p < 1; p++) {
        minuit.GetParameter(p, val, err);
        ret_ary.push_back(val);
    }
    ret_ary.push_back(fmin);

    return ret_ary;
}
