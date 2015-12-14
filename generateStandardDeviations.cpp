#include <iostream>
#include <fstream>
#include <getopt.h>
#include <unordered_map>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/algorithm/string.hpp>
#include "src/ConstantSeasonInfo.h"
#include "src/Team.h"
#include "src/TeamWAverage.h"
#include "src/readTeams.h"
#include "src/Pcts.h"
#include "helpers/doubleFormatter.h"

double standardDeviationOfVector(std::vector<double> *values);

int main(int argc,char *argv[]){
    int c;
    int outYear = 0;
    bool verbose = false, writeOutput = false;
    std::vector<std::string> years;
    std::string inYears = "";
    std::string outFileName = "";

    /*____________________________Parse Command Line___________________________*/
    while ((c = getopt(argc, argv, "y:Y:vo:")) != -1) {
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

    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo::Instance()->initialize(path);

    for (std::string &year : years) {
        sprintf(path, "%s/cpp/NCAA_C/teams/%s/", homePath, year.c_str());
        std::cout << "Reading in waverages for " << year << std::endl;
        readTeamsFromDir(path, "waverages");
    }

    std::string stats[] = {"or", "efg", "ftmr", "to"};

    std::unordered_map<int, std::unordered_map<std::string, Pcts *> *> pcts;
    std::unordered_map<int, std::vector<double> *> srses;
    for (int i = 0; i < 130; i++) {
        pcts.emplace(i, new std::unordered_map<std::string, Pcts *>());
        srses.emplace(i, new std::vector<double>());
        for (std::string &s : stats) {
            pcts[i]->emplace("o"+s, new Pcts());
            pcts[i]->emplace("d"+s, new Pcts());
        }
    }

    boost::gregorian::days indDuration;
    std::unordered_map<std::string, TeamWAverage *> teamWAverages;
    std::unordered_map<std::string, Team *> teams = Team::getTeams();
    for (auto &team : teams) {
        int year = team.second->getYear();
        teamWAverages = team.second->getWAveragesByDate();

        for (auto &waverage : teamWAverages) {
            indDuration = ConstantSeasonInfo::Instance()->get(team.second->getYear(), "tournament start") -
                          waverage.second->getDate();
            int ind = 129 - (int) indDuration.days();

            for (std::string &s : stats) {
                pcts[ind]->at("o"+s)->add_pct(*(waverage.second->getPct("o"+s)));
                pcts[ind]->at("d"+s)->add_pct(*(waverage.second->getPct("d"+s)));
            }
            srses[ind]->push_back(waverage.second->getSrs());
        }
    }

    if (verbose) {
        for (std::string &s : stats) {
            for (int i = 0; i < 130; i++) {
                double avgO, wstdO;
                if (pcts[i]->at("o"+s)->length() > 0) {
                    avgO = pcts[i]->at("o"+s)->p_bar();
                    wstdO = pcts[i]->at("o"+s)->weighted_std_dev();
                }
                else{
                    avgO = 0;
                    wstdO = 0.1;
                }
                //consider what happens when there are no entries -> return 0 or 0.1 or something
                if ((wstdO != wstdO) || (avgO != avgO)) {
                    wstdO = 0.1;
                }

                double avgD, wstdD;
                if (pcts[i]->at("d"+s)->length() > 0) {
                    avgD = pcts[i]->at("d"+s)->p_bar();
                    wstdD = pcts[i]->at("d"+s)->weighted_std_dev();
                }
                else{
                    avgD = 0;
                    wstdD = 0.1;
                }
                //consider what happens when there are no entries -> return 0 or 0.1 or something
                if ((wstdD != wstdD) || (avgD != avgD)) {
                    wstdD = 0.1;
                }
                double wstd = sqrt(wstdO*wstdO + wstdD*wstdD);
                std::cout << "std_devs[\"" << s << ".p\"][" << i << "] = " << doubleFormatter(avgO, 3) <<
                "\t" << doubleFormatter(wstd, 3) << std::endl;
            }
        }
    }

    if (writeOutput){
        sprintf(path, "%s/cpp/NCAA_C/constants/%s", homePath, outFileName.c_str());
        std::ofstream outFile(path, std::ios::app);
        for (std::string &s : stats){
            outFile << outYear << "," << "o" << s;
            for (int i = 0; i < 130; i++){
                double avgO, wstdO;
                if (pcts[i]->at("o"+s)->length() > 0) {
                    avgO = pcts[i]->at("o"+s)->p_bar();
                    wstdO = pcts[i]->at("o"+s)->weighted_std_dev();
                }
                else{
                    avgO = 0;
                    wstdO = 0.1;
                }
                //consider what happens when there are no entries -> return 0 or 0.1 or something
                if ((wstdO != wstdO) || (avgO != avgO)) {
                    wstdO = 0.1;
                }

                double avgD, wstdD;
                if (pcts[i]->at("d"+s)->length() > 0) {
                    avgD = pcts[i]->at("d"+s)->p_bar();
                    wstdD = pcts[i]->at("d"+s)->weighted_std_dev();
                }
                else{
                    avgD = 0;
                    wstdD = 0.1;
                }
                //consider what happens when there are no entries -> return 0 or 0.1 or something
                if ((wstdD != wstdD) || (avgD != avgD)) {
                    wstdD = 0.1;
                }
                double wstd = sqrt(wstdO*wstdO + wstdD*wstdD);
                outFile << "," << doubleFormatter(wstd, 3);
            }
            outFile << std::endl;
        }

        //now deal with SRS
        outFile << outYear << "," << "srs";
        for (int i = 0; i < 130; i++){
            double stdDevOfSRS = standardDeviationOfVector(srses[i]);
            if (stdDevOfSRS == 0) stdDevOfSRS = 20.0;
            outFile << "," << doubleFormatter(stdDevOfSRS, 3);
        }
        outFile << std::endl;

        outFile.close();
    }

    return 0;
}

double standardDeviationOfVector(std::vector<double> *values){
    if (values->size() <= 1) return 0;
    double average = 0;
    for (int i = 0; i < values->size(); i++)
        average += values->at(i);
    average /= values->size();

    double stdDev = 0;
    for (int i = 0; i < values->size(); i++)
        stdDev += pow(values->at(i) - average,2);

    stdDev /= (values->size() - 1);

    return sqrt(stdDev);
}