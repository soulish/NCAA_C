//this file is for testing purposes only

#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "src/ConstantTeamNeutralRatios.h"
#include "src/Team.h"
#include "src/readTeams.h"
#include "helpers/doubleFormatter.h"
#include "src/ConstantSeasonInfo.h"

void printOptions();

int main(int argc,char *argv[]) {
    int c;
    int year = 0;
    bool overWrite = false;
    std::string startDay = "";
    std::string endDay = "";
    bool append = false;
    /*____________________________Parse Command Line___________________________*/
    while((c = getopt(argc,argv,"y:od:D:ah")) != -1){
        switch(c){
            case 'y':
                year = atoi(optarg);
                break;
            case 'o':
                overWrite = true;
                break;
            case 'd':
                startDay.assign(optarg);
                break;
            case 'D':
                endDay.assign(optarg);
                break;
            case 'a':
                append = true;
                break;
            case 'h':
                printOptions();
                return 0;
            default:
                // not an option
                break;
        }
    }
    if (year == 0){
        std::cout << "you must set the year" << std::endl;
        printOptions();
        return 0;
    }
    if (overWrite && append){
        std::cout << "You may either overwrite the files or append to them, not both.  Please" << std::endl;
        std::cout << "choose either the -a or -o switch, not both" << std::endl;
        printOptions();
        return 0;
    }
    if (append && startDay == ""){
        std::cout << "If you choose to append, you must select a start date as well with the -d switch" << std::endl;
        printOptions();
        return 0;
    }

    char *homePath, path[256];
    homePath = getenv("HOME");

    sprintf(path, "%s/cpp/NCAA_C/constants/team_neutral_ratios.d", homePath);
    ConstantTeamNeutralRatios *ratios = ConstantTeamNeutralRatios::Instance();
    ratios->initialize(path);
    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo *seasonInfo = ConstantSeasonInfo::Instance();
    seasonInfo->initialize(path);

    boost::gregorian::date startDate, endDate;

    if (startDay == "")
        startDate = seasonInfo->get(year,"season start") + boost::gregorian::date_duration(14);
    else
        startDate = boost::gregorian::date(boost::gregorian::from_string(startDay));

    if (endDay == "")
        endDate = seasonInfo->get(year,"tournament start");
    else
        endDate = boost::gregorian::date(boost::gregorian::from_string(endDay));

    sprintf(path, "%s/cpp/NCAA_C/teams/%i/", homePath, year);
    std::cout << "Reading in " << year << std::endl;
    readTeamsFromDir(path);

    std::unordered_map<std::string, Team *> teams = Team::getTeams();
    std::map<std::string, Team *> orderedTeams(teams.begin(), teams.end());
    Team *a;

    for (auto &team : orderedTeams) {
        a = team.second;
        std::vector<double> x;
        std::ofstream waveragesFile,averagesFile;

        sprintf(path,"%s/cpp/NCAA_C/teams/%i/teams.%s.averages.d",
                homePath,year,boost::replace_all_copy(a->getName()," ","_").c_str());

        if (!overWrite && !append) {
            struct stat buffer;
            if (stat(path, &buffer) == 0){
                std::cout << team.first << std::endl;
                continue;
            }
        }

        if (append)
            averagesFile.open(path, std::ios::app);
        else
            averagesFile.open(path);
        sprintf(path,"%s/cpp/NCAA_C/teams/%i/teams.%s.waverages.d",
                homePath,year,boost::replace_all_copy(a->getName()," ","_").c_str());
        if (append)
            waveragesFile.open(path, std::ios::app);
        else
            waveragesFile.open(path);

        boost::gregorian::day_iterator ditr(startDate);
        for (; ditr <= endDate; ++ditr) {
            std::cout << "Processing " << team.first << " date: " <<
            boost::gregorian::to_iso_extended_string(*ditr) << "\r";
            std::fflush(stdout);
            x = a->calcWeightedAverage(*ditr);

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
            averagesFile << "," << doubleFormatter(x[55], 0);//num_games
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
            waveragesFile << "," << doubleFormatter(x[53], 3);//origSRS, same as srs at the moment
            waveragesFile << "," << doubleFormatter(x[53], 3);//srs
            waveragesFile << "," << doubleFormatter(x[54], 3);//sos
            waveragesFile << "," << doubleFormatter(x[55], 0);//num_games
            waveragesFile << std::endl;
        }
        averagesFile.close();
        waveragesFile.close();
        std::cout << std::endl;
    }

    return 0;
}

void printOptions(){
    std::cout << std::endl;
    std::cout << "generateWeightedAverages Usage options:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y (int) year to generate weighted averages for (no default)[Required]" << std::endl;
    std::cout << "\t-o overwrite currently existing files in directory? (false)[Optional]" << std::endl;
    std::cout << "\t-d (string yyyy-mm-dd) start date (default: 2 weeks into season)[Required with -a switch]" << std::endl;
    std::cout << "\t-D (string yyyy-mm-dd) end date (default: first day of the tournament)[Optional]" << std::endl;
    std::cout << "\t-a append to file?[Optional]" << std::endl;
    std::cout << "\t-h print this message" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: $CLION/generateWeightedAverages -y 2015 -d \"2015-04-07\" -D \"2015-04-07\" -a" << std::endl;
    std::cout << std::endl;
    std::cout << "This program will generate the weighted averages for every team in a given year." << std::endl;
    std::cout << "It requires you to specify the year.  Then it searches the teams/YEAR/ directory" << std::endl;
    std::cout << "to find all of the team's game information.  It also reads in the averages" << std::endl;
    std::cout << "and neutral ratios files found in the constants directory.  It outputs two files" << std::endl;
    std::cout << "for each team, an (unweighted) averages file and a weighted averages file." << std::endl;
    std::cout << "If you have already generated some of the averages files in the year's directory," << std::endl;
    std::cout << "you may choose whether or not to overwrite them with the -o switch, or to append" << std::endl;
    std::cout << "to the file with the -a switch in conjunction with setting the start and end" << std::endl;
    std::cout << "dates with the -d and -D options." << std::endl;
    std::cout << "" << std::endl;
    std::cout << std::endl;
}
