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

int main(int argc,char *argv[]){
    int c;
    int outYear = 0;
    bool verbose = false, writeOutput = false;
    std::vector<std::string> years;
    std::string inYears;

    /*____________________________Parse Command Line___________________________*/
    while((c = getopt(argc,argv,"y:Y:vo")) != -1){
        switch(c){
            case 'y':
                std::cout << "inYears: " << optarg << std::endl;
                inYears.assign(optarg);
                boost::split(years,inYears,boost::is_any_of(","));
                break;
            case 'Y':
                outYear = atoi(optarg);
                break;
            case 'v':
                verbose = true;
                break;
            case 'o':
                writeOutput = true;
                break;
            default:
                // not an option
                break;
        }
    }

    //ensure years and outYear are set
    if (years.empty()){
        std::cout << "You must set the input years using the -y switch and a comma-separated list of years" << std::endl;
        return 0;
    }
    if (outYear == 0){
        std::cout << "You must set the output year using the -Y switch" << std::endl;
        return 0;
    }

    char *homePath, path[256];
    homePath = getenv("HOME");

    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo::Instance()->initialize(path);


    for (std::string &year : years){
        sprintf(path, "%s/cpp/NCAA_C/teams/%s/", homePath, year.c_str());
        std::cout << "Doing " << year << std::endl;
        readTeamsFromDir(path,"waverages");
    }

    std::string stats[] = {"oor","oefg","oftmr","oto"};

    std::unordered_map<int, std::unordered_map<std::string, Pcts* > * > pcts;
    for (int i = 0; i < 155; i++) {
        pcts.emplace(i, new std::unordered_map<std::string, Pcts*>());
        for (std::string &s : stats)
            pcts[i]->emplace(s, new Pcts());
    }

    boost::gregorian::days indDuration;
    std::unordered_map<std::string, TeamWAverage *> teamWAverages;
    std::unordered_map<std::string, Team *> teams = Team::getTeams();
    for (auto &team : teams){
        int year = team.second->getYear();
        teamWAverages = team.second->getWAveragesByDate();

        for (auto &waverage : teamWAverages){
            indDuration = ConstantSeasonInfo::Instance()->get(team.second->getYear(),"tournament start") -
                    *(waverage.second->getDate());
            int ind = 154 - (int)indDuration.days();

            for (std::string &s : stats){
                pcts[ind]->at(s)->add_pct(*(waverage.second->getPct(s)));
            }
        }
    }

    for (std::string &s : stats){
        for (int i = 0; i < 155; i++){
            double avg = pcts[i]->at(s)->p_bar();
            double wstd = pcts[i]->at(s)->weighted_std_dev();
            //consider what happens when there are no entries -> return 0 or 0.1 or something
            std::cout << "std_devs[\"" << s << "\"][" << i << "] = " << doubleFormatter(avg,3) <<
                    "\t" << doubleFormatter(wstd,3) << std::endl;
        }
    }

    return 0;
}