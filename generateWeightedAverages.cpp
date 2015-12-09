//this file is for testing purposes only

#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/foreach.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "src/ConstantTeam5YearAverages.h"
#include "src/ConstantTeamNeutralRatios.h"
#include "src/Team.h"
#include "src/TeamGame.h"
#include "src/Pcts.h"
#include "src/readTeams.h"
#include "helpers/doubleFormatter.h"
#include "src/ConstantSeasonInfo.h"

int main(int argc,char *argv[]) {
    int c;
    int year = 0;
    bool overWrite = false;
    /*____________________________Parse Command Line___________________________*/
    while((c = getopt(argc,argv,"y:o")) != -1){
        switch(c){
            case 'y':
                year = atoi(optarg);
                break;
            case 'o':
                overWrite = true;
                break;
            default:
                // not an option
                break;
        }
    }

    if (year == 0){
        std::cout << "you must set the year" << std::endl;
        return 0;
    }

    char *homePath, path[256];
    homePath = getenv("HOME");

    sprintf(path, "%s/cpp/NCAA_C/constants/team_5yr_averages.d", homePath);
    ConstantTeam5YearAverages::Instance()->initialize(path);
    sprintf(path, "%s/cpp/NCAA_C/constants/team_neutral_ratios.d", homePath);
    ConstantTeamNeutralRatios::Instance()->initialize(path);
    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo::Instance()->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/teams/%i/", homePath, year);
    std::cout << "Reading in " << year << std::endl;
    readTeamsFromDir(path);

    typedef std::unordered_map<std::string, Team *> teamHash;
    teamHash teams = Team::getTeams();
    Team *a;
    TeamGame *tg;

    for (auto &team : teams) {
        std::cout << team.first << std::endl;
        a = team.second;
        std::vector<double> x;
        std::ofstream waveragesFile,averagesFile;

        sprintf(path,"%s/cpp/NCAA_C/teams/%i/teams.%s.averages.d",
                homePath,year,boost::replace_all_copy(a->getName()," ","_").c_str());

        if (!overWrite) {
            struct stat buffer;
            if (stat(path, &buffer) == 0) continue;
        }

        averagesFile.open(path);
        sprintf(path,"%s/cpp/NCAA_C/teams/%i/teams.%s.waverages.d",
                homePath,year,boost::replace_all_copy(a->getName()," ","_").c_str());
        waveragesFile.open(path);
        boost::gregorian::day_iterator ditr(ConstantSeasonInfo::Instance()->get(year,"season start") +
                                            boost::gregorian::date_duration(14));//2 weeks after start of season
        for (; ditr <= ConstantSeasonInfo::Instance()->get(year,"tournament start"); ++ditr) {
            std::cout<< "Processing date: " <<
            boost::gregorian::to_iso_extended_string(*ditr) << "\r";
            std::fflush(stdout);
            x = a->calcWeightedAverage(*ditr);

            tg = a->GameOnDate(*ditr);

            averagesFile << a->getName();
            averagesFile << "," << ditr->year() << "," << ditr->month().as_number() << "," << ditr->day();
            averagesFile << "," << doubleFormatter(x[0], 0);
            for (int ii = 1; ii < 13; ii++) {
                if (ii % 2 == 1) averagesFile << "," << doubleFormatter(x[ii], 0);
                if (ii % 2 == 0) averagesFile << "," << doubleFormatter(x[ii], 3);
            }
            averagesFile << "," << doubleFormatter(x[13], 0);
            for (int ii = 14; ii < 26; ii++) {
                if (ii % 2 == 0) averagesFile << "," << doubleFormatter(x[ii], 0);
                if (ii % 2 == 1) averagesFile << "," << doubleFormatter(x[ii], 3);
            }
            if (tg) averagesFile << "," << doubleFormatter(tg->getSpread(), 1);
            else averagesFile << "," << "0.0";
            averagesFile << std::endl;

            waveragesFile << a->getName();
            waveragesFile << "," << ditr->year() << "," << ditr->month().as_number() << "," << ditr->day();
            waveragesFile << "," << doubleFormatter(x[26], 0);
            for (int ii = 27; ii < 39; ii++) {
                if (ii % 2 == 1) waveragesFile << "," << doubleFormatter(x[ii], 0);
                if (ii % 2 == 0) waveragesFile << "," << doubleFormatter(x[ii], 3);
            }
            waveragesFile << "," << doubleFormatter(x[39], 0);
            for (int ii = 40; ii < 52; ii++) {
                if (ii % 2 == 0) waveragesFile << "," << doubleFormatter(x[ii], 0);
                if (ii % 2 == 1) waveragesFile << "," << doubleFormatter(x[ii], 3);
            }
            waveragesFile << "," << doubleFormatter(x[52], 3);//rpi
            waveragesFile << "," << doubleFormatter(x[53], 3);//srs
            waveragesFile << "," << doubleFormatter(x[54], 3);//sos
            waveragesFile << "," << doubleFormatter(x[55], 0);//num_games
            if (tg) waveragesFile << "," << doubleFormatter(tg->getSpread(), 1);//spread
            else waveragesFile << "," << "0.0";
            waveragesFile << std::endl;
        }
        averagesFile.close();
        waveragesFile.close();
        std::cout << std::endl;
    }


    return 0;
}