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

std::vector<double> weightedAverageAndError(std::vector<double> values, std::vector<double> errors);

int main(int argc,char *argv[]) {
    int c;
    bool verbose = false, writeOutput = false;
    std::vector<std::string> years;
    std::string inYears = "";
    std::string outFileName = "";
    double nbins = 165;

    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:vo:n:")) != -1) {
        switch (c) {
            case 'y':
                std::cout << "inYears: " << optarg << std::endl;
                inYears.assign(optarg);
                boost::split(years, inYears, boost::is_any_of(","));
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

    //ensure years are set
    if (years.empty()) {
        std::cout << "You must set the input years using the -y switch and a comma-separated list of years" <<
        std::endl;
        return 0;
    }

    char *homePath, path[256], name[256], title[256];
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
//    std::unordered_map<std::string, std::vector<TH2F*> *> hist_slices;
    std::unordered_map<std::string, std::vector<std::vector<Pcts*> *> *> pcts;

    //initialize the hashes
    for (auto &s : stats){
        sprintf(name,"hist_%s",s.c_str());
        sprintf(title,"d%s vs o%s",s.c_str(),s.c_str());
        hist.emplace(s, new TH2F(name,title,range[s]->at(2),0,1,range[s]->at(2),0,1));

//        hist_slices.emplace(s, new std::vector<TH2F*>());
        pcts.emplace(s, new std::vector<std::vector<Pcts*>*>());
        for (int i = 0; i < range[s]->at(2); i++){
//            sprintf(name,"hist_slices_%s_%i",s.c_str(),i);
//            hist_slices[s]->push_back(new TH2F(name,"",range[s]->at(2), 0, 1, range[s]->at(2), 0, 1));
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
//                hist_slices[s]->at(i)->Fill(oppWaverage->getValue("d"+s+".p"),game.second->getValue("o"+s+".p"));
                pcts[s]->at(i)->at(j)->add_pct(*(game.second->getPct("o"+s)));
            }
        }
    }

    std::unordered_map<std::string, TGraphErrors*> graphs;
    std::unordered_map<std::string, TGraphErrors*> averagedGraphs;
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

        std::vector<double> allYAtX[2*size];
        std::vector<double> allYerrAtX[2*size];

        for (int i = 0; i < size; i++){
            used[0] = 0;
            for (int j = 0; j < size; j++){
                x[i][j] = 0;
                y[i][j] = 0;
                x_err[i][j] = 0;
                y_err[i][j] = 0;

                if (pcts[s]->at(i)->at(j)->length() > 5) {
                    used[i] = true;

                    x[i][j] = j / range[s]->at(2) - i / range[s]->at(2);
                    //the denominator here is the middle of the bin that i represents.
                    //so, a value of 1 for y[i][j] means that p_bar is the same as you would
                    //expect without considering the defense.  A number below 1 means that
                    //the defense is lowering the result, and a number above 1 means that
                    //the defense is causing a higher result than expected.
                    y[i][j] = pcts[s]->at(i)->at(j)->p_bar() / (i / range[s]->at(2) + 1 / (2 * range[s]->at(2)));
                    y_err[i][j] = pcts[s]->at(i)->at(j)->weighted_std_dev() /
                                  (i / range[s]->at(2) + 1 / (2 * range[s]->at(2)));

                    allX.push_back(x[i][j]);
                    allY.push_back(y[i][j]);
                    allX_err.push_back(0);
                    allY_err.push_back(y_err[i][j]);

                    allYAtX[j - i + ((int)(range[s]->at(2)) - 1)].push_back(y[i][j]);
                    allYerrAtX[j - i + ((int)(range[s]->at(2)) - 1)].push_back(y_err[i][j]);
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
        sprintf(title,"%s result vs difference",s.c_str());
        graphs[s]->SetTitle(title);

        std::vector<double> averagedX;
        std::vector<double> averagedY;
        std::vector<double> averagedXerr;
        std::vector<double> averagedYerr;

        for (int i = 0; i < 2*size; i++){
            if (allYAtX[i].size() > 0){
                std::vector<double> averageAndError = weightedAverageAndError(allYAtX[i], allYerrAtX[i]);
                if (averageAndError.size() == 0) continue;
                if (averageAndError[0] != averageAndError[0]) continue;//check for nans
                averagedX.push_back((i - range[s]->at(2)) / range[s]->at(2));
                averagedXerr.push_back(0);
                averagedY.push_back(averageAndError[0]);
                averagedYerr.push_back(averageAndError[1]);
//                if (s == "or"){
//                    std::cout << averagedX.back() << "\t" << averagedY.back() << "\t" <<
//                         averagedYerr.back() << std::endl;
//                }
            }
        }

        int averagedSize = (int) averagedX.size();
        double averagedXarr[averagedSize];
        std::copy(averagedX.begin(), averagedX.end(), averagedXarr);
        double averagedYarr[averagedSize];
        std::copy(averagedY.begin(), averagedY.end(), averagedYarr);
        double averagedX_err_arr[averagedSize];
        std::copy(averagedXerr.begin(), averagedXerr.end(), averagedX_err_arr);
        double averagedY_err_arr[averagedSize];
        std::copy(averagedYerr.begin(), averagedYerr.end(), averagedY_err_arr);

        averagedGraphs.emplace(s, new TGraphErrors(averagedSize, averagedXarr, averagedYarr,
                                                   averagedX_err_arr, averagedY_err_arr));
        sprintf(name,"averaged_graph_%s",s.c_str());
        averagedGraphs[s]->SetName(name);
        sprintf(title,"%s result vs difference averaged",s.c_str());
        averagedGraphs[s]->SetTitle(title);

        sprintf(name,"fn_%s",s.c_str());
        fns.emplace(s, new TF1(name,"pol1"));
        averagedGraphs[s]->Fit(fns[s], "eq");

//        std::cout << "fn for " << s << " parameters are\t\t" << fns[s]->GetParameter(0) << ",\t" <<
//        fns[s]->GetParameter(1) << std::endl;
    }

    outFile.Append(graphs["or"]);
    outFile.Append(graphs["efg"]);
    outFile.Append(graphs["ftmr"]);
    outFile.Append(graphs["to"]);

    outFile.Append(averagedGraphs["or"]);
    outFile.Append(averagedGraphs["efg"]);
    outFile.Append(averagedGraphs["ftmr"]);
    outFile.Append(averagedGraphs["to"]);

    outFile.Write();
    outFile.Close();

    return 0;
}

std::vector<double> weightedAverageAndError(std::vector<double> values, std::vector<double> errors){
    //based off Taylor's definitions.  Note that we care about the Error on
    //the average here, not the standard deviation, hence we return 1/sqrt(sum_weights) as error.
    //see Taylor chapter 7
    std::vector<double> result;
    if (values.size() <= 1 || values.size() != errors.size())
        return result;

    int size = (int)values.size();
    double sum_weights = 0.0;
    for (int i = 0; i < size; i++){
        sum_weights += errors[i] == 0 ? 0 : 1/ pow(errors[i],2);
    }

    if (sum_weights == 0)
        return result;

    double average = 0.0;
    for (int i = 0; i < size; i++){
        average += errors[i] == 0 ? 0 : (values[i] / pow(errors[i],2)) / sum_weights;
    }

    double error = sum_weights == 0 ? 0 : 1 / sqrt(sum_weights);

    result.push_back(average);
    result.push_back(error);
    return result;
}