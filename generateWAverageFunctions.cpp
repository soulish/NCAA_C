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
#include "src/ConstantTeamNeutralRatios.h"

std::vector<double> weightedAverageAndError(std::vector<double> values, std::vector<double> errors);
void printOptions();

int main(int argc,char *argv[]) {
    int c;
    bool writeOutput = false;
    std::vector<std::string> years;
    std::string inYears = "";
    std::string outFileName = "";
    double nbins = 165;

    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:o:n:h")) != -1) {
        switch (c) {
            case 'y':
                std::cout << "inYears: " << optarg << std::endl;
                inYears.assign(optarg);
                boost::split(years, inYears, boost::is_any_of(","));
                break;
            case 'o':
                outFileName.assign(optarg);
                writeOutput = true;
                break;
            case 'n':
                nbins = (double)(atoi(optarg));
                break;
            case 'h':
                printOptions();
                return 0;
            default:
                // not an option
                break;
        }
    }

    //ensure years are set
    if (years.empty()) {
        std::cout << "You must set the input years using the -y switch and a comma-separated list of years" <<
        std::endl;
        printOptions();
        return 0;
    }
    if (!writeOutput){
        std::cout << "You must set the output ROOT file name using the -o switch" << std::endl;
        printOptions();
        return 0;
    }

    char *homePath, path[256], name[256], title[256];
    homePath = getenv("HOME");

    sprintf(path, "%s/cpp/NCAA_C/constants/team_neutral_ratios.d", homePath);
    ConstantTeamNeutralRatios::Instance()->initialize(path);
    ConstantTeamNeutralRatios *ratios = ConstantTeamNeutralRatios::Instance();
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

    std::string stats[] = {"or","efg","ftmr","to","ppp"};

    std::unordered_map<std::string, std::array<double,3> *> range;
    range.emplace("or", new std::array<double,3>());
    range["or"]->at(0) = 0.1;//range["or"]->at(0) = 0.1;
    range["or"]->at(1) = 0.7;//range["or"]->at(1) = 0.5;
    range["or"]->at(2) = nbins;
    range.emplace("efg", new std::array<double,3>());
    range["efg"]->at(0) = 0.2;//range["efg"]->at(0) = 0.3;
    range["efg"]->at(1) = 0.8;//range["efg"]->at(1) = 0.7;
    range["efg"]->at(2) = nbins;
    range.emplace("ftmr", new std::array<double,3>());
    range["ftmr"]->at(0) = 0.0;//range["ftmr"]->at(0) = 0.1;
    range["ftmr"]->at(1) = 0.6;//range["ftmr"]->at(1) = 0.5;
    range["ftmr"]->at(2) = nbins;
    range.emplace("to", new std::array<double,3>());
    range["to"]->at(0) = 0.05;//range["to"]->at(0) = 0.0;
    range["to"]->at(1) = 0.35;//range["to"]->at(1) = 0.4;
    range["to"]->at(2) = nbins;
    range.emplace("ppp", new std::array<double,3>());
    range["ppp"]->at(0) = 0.3;
    range["ppp"]->at(1) = 1.3;
    range["ppp"]->at(2) = nbins;

    std::unordered_map<std::string, TH2F*> defVsOffHists;
    std::unordered_map<std::string, TH2F*> gameResultsVsDiffHists;
    std::unordered_map<std::string, std::vector<std::vector<Pcts*> *> *> pcts;
    std::unordered_map<std::string, TH1F*> numGamesAtDiff;

    //initialize the hashes
    for (auto &s : stats){
        sprintf(name,"hist_%s",s.c_str());
        sprintf(title,"d%s vs o%s",s.c_str(),s.c_str());
        defVsOffHists.emplace(s, new TH2F(name, title, range[s]->at(2), range[s]->at(0), range[s]->at(1),
                                          range[s]->at(2), range[s]->at(0), range[s]->at(1)));

        sprintf(name,"gameResultsVsDiff_%s",s.c_str());
        sprintf(title,"Result vs Diff %s",s.c_str());
        gameResultsVsDiffHists.emplace(s, new TH2F(name, title, 2*range[s]->at(2), -1*range[s]->at(1), range[s]->at(1),
                                                   2*range[s]->at(2), 0, 2));

        sprintf(name,"numGamesAtDiff_%s",s.c_str());
        numGamesAtDiff.emplace(s, new TH1F(name,"",2*range[s]->at(2), -1*range[s]->at(1), range[s]->at(1)));

        pcts.emplace(s, new std::vector<std::vector<Pcts*>*>());
        for (int i = 0; i < range[s]->at(2); i++){
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
                int i, j;
                if (s == "ppp") {
                    double waP = waverage->getValue("o" + s + ".m") / waverage->getValue("o" + s + ".a");
                    double oppWaP =  oppWaverage->getValue("d" + s + ".m") / oppWaverage->getValue("d" + s + ".a");
                    defVsOffHists[s]->Fill(waP,oppWaP);
                    i = (int)floor((waP - range[s]->at(0)) * range[s]->at(2) /
                                   (range[s]->at(1) - range[s]->at(0)));
                    j = (int)floor((oppWaP - range[s]->at(0)) * range[s]->at(2) /
                                   (range[s]->at(1) - range[s]->at(0)));
                }
                else{
                    defVsOffHists[s]->Fill(waverage->getValue("o" + s + ".p"), oppWaverage->getValue("d" + s + ".p"));
                    i = (int)floor((waverage->getValue("o"+s+".p") - range[s]->at(0)) * range[s]->at(2) /
                                   (range[s]->at(1) - range[s]->at(0)));
                    j = (int)floor((oppWaverage->getValue("d"+s+".p") - range[s]->at(0)) * range[s]->at(2) /
                                   (range[s]->at(1) - range[s]->at(0)));
                }
                //a very small number of games should have out-of-bounds problems, but this fixes them.
                i = i < 0 ? 0 : i;
                i = i >= range[s]->at(2) ? range[s]->at(2) - 1 : i;
                j = j < 0 ? 0 : j;
                j = j >= range[s]->at(2) ? range[s]->at(2) - 1 : j;
                gameResultsVsDiffHists[s]->Fill((i-j) * (range[s]->at(1) - range[s]->at(0)) / range[s]->at(2),
                                                game.second->getPct("o"+s)->P() * ratios->get(year,game.second->getLoc(),"o"+s+".p") /
                                                waverage->getValue("o"+s+".p"));
                numGamesAtDiff[s]->Fill((i-j) * (range[s]->at(1) - range[s]->at(0)) / range[s]->at(2));

                //neutralize the results
                pcts[s]->at(i)->at(j)->add_pct(Pct(game.second->getPct("o"+s)->M() * ratios->get(year,game.second->getLoc(),"o"+s+".m"),
                                                   game.second->getPct("o"+s)->A() * ratios->get(year,game.second->getLoc(),"o"+s+".a")));
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

                    //the convention here is that negative values on the x-axis
                    //means the offense averages less than the defense usually gives up
                    //while positive values means the offense averages more than the
                    //defense gives up.  So as we increase in x, the defense is getting better
                    //compared to the offense.  This convention was chosen so that we can
                    //evaluate the functions with team A's offense minus team B's defense.
//                    x[i][j] = i / range[s]->at(2) - j / range[s]->at(2);
                    x[i][j] = (i - j) * (range[s]->at(1) - range[s]->at(0)) / range[s]->at(2);

                    //the denominator here is the middle of the bin that i represents.
                    //so, a value of 1 for y[i][j] means that p_bar is the same as you would
                    //expect without considering the defense.  A number below 1 means that
                    //the defense is lowering the result, and a number above 1 means that
                    //the defense is causing a higher result than expected.
//                    y[i][j] = pcts[s]->at(i)->at(j)->p_bar() /
//                              (i / range[s]->at(2) + 1 / (2 * range[s]->at(2)));
//                    y_err[i][j] = pcts[s]->at(i)->at(j)->weighted_std_dev() /
//                                  (i / range[s]->at(2) + 1 / (2 * range[s]->at(2)));
                    y[i][j] = pcts[s]->at(i)->at(j)->p_bar() /
                            (i * (range[s]->at(1) - range[s]->at(0)) / range[s]->at(2) + range[s]->at(0) +
                             1 / (2 * range[s]->at(2)));
                    y_err[i][j] = pcts[s]->at(i)->at(j)->weighted_std_dev() /
                            (i * (range[s]->at(1) - range[s]->at(0)) / range[s]->at(2) + range[s]->at(0) +
                             1 / (2 * range[s]->at(2)));

                    allX.push_back(x[i][j]);
                    allY.push_back(y[i][j]);
                    allX_err.push_back(0);
                    allY_err.push_back(y_err[i][j]);

                    allYAtX[i - j + ((int)(range[s]->at(2)) - 1)].push_back(y[i][j]);
                    allYerrAtX[i - j + ((int)(range[s]->at(2)) - 1)].push_back(y_err[i][j]);
                }
            }
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
                averagedX.push_back((i - (range[s]->at(2) - 1)) * (range[s]->at(1) - range[s]->at(0)) /
                                    range[s]->at(2));
                averagedXerr.push_back(0);
                averagedY.push_back(averageAndError[0]);
                averagedYerr.push_back(averageAndError[1]);
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
    }

    outFile.Append(graphs["or"]);
    outFile.Append(graphs["efg"]);
    outFile.Append(graphs["ftmr"]);
    outFile.Append(graphs["to"]);
    outFile.Append(graphs["ppp"]);

    outFile.Append(averagedGraphs["or"]);
    outFile.Append(averagedGraphs["efg"]);
    outFile.Append(averagedGraphs["ftmr"]);
    outFile.Append(averagedGraphs["to"]);
    outFile.Append(averagedGraphs["ppp"]);

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

void printOptions(){
    std::cout << std::endl;
    std::cout << "generateWAverageFunctions Usage options:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y (int,int,...) comma-separated list of input years (no default)[Required]" << std::endl;
    std::cout << "\t-o (string) ROOT file name generated in rootFiles directory (no default)[Required]" << std::endl;
    std::cout << "\t-n (int) number of bins to use for the histograms (default: 165)[Optional]" << std::endl;
    std::cout << "\t-h print this message" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: $CLION/generateWAverageFunctions -y 2010,2011,2012,2013,2014 -o functions_2015_301.root -n 301" << std::endl;
    std::cout << std::endl;
    std::cout << "This program generates the histograms which will be used to generate the" << std::endl;
    std::cout << "functions which take as input Team A's offensive WAverages and Team B's" << std::endl;
    std::cout << "defensive WAverage, and returns a prediction for what Team A's offensive" << std::endl;
    std::cout << "WAverage would be in that game.  There is a function for each of the four" << std::endl;
    std::cout << "weighted average stats." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "It takes as input 5 years worth of WAverages, and output a ROOT file with" << std::endl;
    std::cout << "the histograms and graphs needed to calculate the functions using the " << std::endl;
    std::cout << "showWAverageFns.rb program.  The number of bins will depend on the number" << std::endl;
    std::cout << "of input years (generally 5 => 301, less => 165)." << std::endl;
    std::cout << std::endl;
}
