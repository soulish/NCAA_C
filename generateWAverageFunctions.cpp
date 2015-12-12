#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <array>
#include <fstream>
#include <TFile.h>
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "src/Team.h"
#include "src/TeamGame.h"
#include "src/Pcts.h"
#include "src/readTeams.h"
#include "helpers/doubleFormatter.h"
#include "src/ConstantSeasonInfo.h"

int main(int argc,char *argv[]) {
    int c;
    int outYear = 0;
    bool verbose = false, writeOutput = false;
    std::vector<std::string> years;
    std::string inYears = "";
    std::string outFileName = "";
    double nbins = 165;

    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:Y:vo:n:")) != -1) {
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
            case 'n':
                nbins = (double)(atoi(optarg));
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

    char *homePath, path[256], name[256];
    homePath = getenv("HOME");

    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo::Instance()->initialize(path);

    for (std::string &year : years) {
        sprintf(path, "%s/cpp/NCAA_C/teams/%s/", homePath, year.c_str());
        std::cout << "Reading in games and waverages for " << year << std::endl;
        readTeamsFromDir(path, "games");
        readTeamsFromDir(path, "waverages");
    }

    //create the output ROOT file
    sprintf(path, "%s/cpp/NCAA_C/rootFiles/%s",homePath,outFileName.c_str());
    TFile outFile(path,"recreate");

    std::string stats[] = {"or","efg","ftmr","to"};

    std::unordered_map<std::string, std::array<double,3> *> range;
    range.emplace("or", new std::array<double,3>());
    range["or"]->at(0) = 0.1;
    range["or"]->at(1) = 0.5;
    range["or"]->at(2) = nbins;
    range.emplace("efg", new std::array<double,3>());
    range["efg"]->at(0) = 0.3;
    range["efg"]->at(1) = 0.7;
    range["efg"]->at(2) = nbins;
    range.emplace("ftmr", new std::array<double,3>());
    range["ftmr"]->at(0) = 0.1;
    range["ftmr"]->at(1) = 0.5;
    range["ftmr"]->at(2) = nbins;
    range.emplace("to", new std::array<double,3>());
    range["to"]->at(0) = 0.0;
    range["to"]->at(1) = 0.4;
    range["to"]->at(2) = nbins;

    std::unordered_map<std::string, TH2F*> hist;
    std::unordered_map<std::string, std::vector<TH2F*> *> hist_slices;
    std::unordered_map<std::string, std::vector<std::vector<Pcts*> *> *> pcts;

    //initialize the hashes
    for (auto &s : stats){
        sprintf(name,"hist_%s",s.c_str());
        hist.emplace(s, new TH2F(name,"",range[s]->at(2),0,1,range[s]->at(2),0,1));

        hist_slices.emplace(s, new std::vector<TH2F*>());
        pcts.emplace(s, new std::vector<std::vector<Pcts*>*>());
        for (int i = 0; i < range[s]->at(2); i++){
            sprintf(name,"hist_slices_%s_%i",s.c_str(),i);
            hist_slices[s]->push_back(new TH2F(name,"",range[s]->at(2), 0, 1, range[s]->at(2), 0, 1));
            pcts[s]->push_back(new std::vector<Pcts*>());
            for (int j = 0; j < range[s]->at(2); j++){
                pcts[s]->at(i)->push_back(new Pcts());
            }
        }
    }

    std::unordered_map<std::string, Team *> teams = Team::getTeams();
    std::unordered_map<std::string, TeamGame *> teamGames;
    Team *opp;
    TeamGame *oppGame;
    TeamWAverage *waverage, *oppWaverage;
    boost::gregorian::date_duration twoWeeks(14);

    for (auto &team : teams){
        teamGames = team.second->getGamesByDate();
        int year = team.second->getYear();
        for (auto &game : teamGames){
            opp = Team::findTeam(game.second->getOpp());
            if (!opp) continue; //skip non-Division-I opponents
            if (game.second->getDate() >= ConstantSeasonInfo::Instance()->get(year,"tournament start"))
                continue; //skip tournament games
//            if (game.second->getDate() < ConstantSeasonInfo::Instance()->get(year,"season start") + twoWeeks)
//                continue;
            if (game.second->getDate().month().as_number() == 11)//skip games in November
                continue;

            oppGame = opp->GameOnDate(game.second->getDate(),team.first);//get the game played by self against opp
            if (!oppGame) continue;///these two lines can be deleted as oppGame is never used

            waverage = team.second->WAverageOnDate(game.second->getDate());
            if (!waverage) continue;
            oppWaverage = opp->WAverageOnDate(game.second->getDate());
            if (!oppWaverage) continue;

            for (std::string &s : stats){
                hist[s]->Fill(waverage->getValue("o"+s+".p"), oppWaverage->getValue("d"+s+".p"));
                int i = (int)floor(waverage->getValue("o"+s+".p") * range[s]->at(2));
                int j = (int)floor(oppWaverage->getValue("d"+s+".p") * range[s]->at(2));
                hist_slices[s]->at(i)->Fill(oppWaverage->getValue("d"+s+".p"),game.second->getValue("o"+s+".p"));
                pcts[s]->at(i)->at(j)->add_pct(*(game.second->getPct("o"+s)));
            }
        }
    }

    std::unordered_map<std::string, TGraphErrors*> graphs;
//    std::unordered_map<std::string, std::vector<TGraphErrors*> *> grs;
    std::unordered_map<std::string, TF1*> fns;

    for (std::string &s : stats){
        int size = (int)range[s]->at(2);
        double x[size][size];
        double y[size][size];
        double x_err[size][size];
        double y_err[size][size];

        std::vector<double> allX;
        std::vector<double> allY;
        std::vector<double> allX_err;
        std::vector<double> allY_err;

        bool used[size];

        for (int i = 0; i < size; i++){
            used[0] = 0;
            for (int j = 0; j < size; j++){
                x[i][j] = 0;
                y[i][j] = 0;
                x_err[i][j] = 0;
                y_err[i][j] = 0;

                if (pcts[s]->at(i)->at(j)->length() > 20) {
                    used[i] = true;

                    x[i][j] = j / range[s]->at(2) - i / range[s]->at(2);
                    y[i][j] = pcts[s]->at(i)->at(j)->p_bar() / (i / range[s]->at(2) + 1 / (2 * range[s]->at(2)));
                    y_err[i][j] = pcts[s]->at(i)->at(j)->weighted_std_dev() /
                                  (i / range[s]->at(2) + 1 / (2 * range[s]->at(2)));

                    allX.push_back(x[i][j]);
                    allY.push_back(y[i][j]);
                    allX_err.push_back(0);
                    allY_err.push_back(y_err[i][j]);
                }
            }
//            if (used[i]) {
//                grs[s]->push_back(new TGraphErrors(range[s]->at(2), x[i], y[i], x_err[i], y_err[i]));
//                sprintf(name, "");
//            }
        }

        int allSize = (int) allX.size();
        double allXarr[allSize];
        std::copy(allX.begin(), allX.end(), allXarr);
        double allYarr[allSize];
        std::copy(allY.begin(), allY.end(), allYarr);
        double allX_err_arr[allSize];
        std::copy(allX_err.begin(), allX_err.end(), allX_err_arr);
        double allY_err_arr[allSize];
        std::copy(allY_err.begin(), allY_err.end(), allY_err_arr);

        graphs.emplace(s, new TGraphErrors(allSize, allXarr, allYarr, allX_err_arr, allY_err_arr));
        sprintf(name,"graph_%s",s.c_str());
        graphs[s]->SetName(name);

        sprintf(name,"fn_%s",s.c_str());
        fns.emplace(s, new TF1(name,"pol1"));
        graphs[s]->Fit(fns[s], "eq");

        std::cout << "fn for " << s << " parameters are\t\t" << fns[s]->GetParameter(0) << ",\t" <<
        fns[s]->GetParameter(1) << std::endl;
    }

    outFile.Append(graphs["or"]);
    outFile.Append(graphs["efg"]);
    outFile.Append(graphs["ftmr"]);
    outFile.Append(graphs["to"]);

    outFile.Append(fns["or"]);
    outFile.Append(fns["efg"]);
    outFile.Append(fns["ftmr"]);
    outFile.Append(fns["to"]);

    outFile.Write();
    outFile.Close();

    return 0;
}