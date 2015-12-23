#include <iostream>
#include <fstream>
#include <getopt.h>
#include <unordered_map>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/foreach.hpp>
#include "src/Team.h"
#include "src/ConstantTeam5YearAverages.h"
#include "src/ConstantTeamNeutralRatios.h"
#include "src/ConstantSeasonInfo.h"
#include "src/TeamWAverage.h"
#include "src/readTeams.h"
#include "helpers/doubleFormatter.h"

std::vector<double> calcSRS(std::unordered_map<std::string, double>*, std::string, boost::gregorian::date);
void printOptions();

int main(int argc,char *argv[]){
    int c;
    int year = 2015, numIterations = 100;
    bool verbose = false, writeOutput = false;
    bool startFromOriginalSRS = false;
    /*____________________________Parse Command Line___________________________*/
    while((c = getopt(argc,argv,"y:vn:oOh")) != -1){
        switch(c){
            case 'y':
                year = atoi(optarg);
                break;
            case 'n':
                numIterations = atoi(optarg);
                break;
            case 'v':
                verbose = true;
                break;
            case 'o':
                writeOutput = true;
                break;
            case 'O':
                startFromOriginalSRS = true;
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

    if (numIterations == 0){
        std::cout << "you must set the number of iterations" << std::endl;
        printOptions();
        return 0;
    }

    char *homePath, path[256];
    homePath = getenv("HOME");

    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo::Instance()->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/teams/%i/", homePath, year);
    std::cout << "Reading in " << year << std::endl;
    readTeamsFromDir(path);
    readTeamsFromDir(path,"waverages");

    typedef std::unordered_map<std::string, double> teamHashType;
    typedef std::unordered_map<std::string, teamHashType *> dateHashType;
    typedef std::unordered_map<int, dateHashType *> srsHashType;
    typedef std::unordered_map<std::string, Team *> allTeamsHashType;

    srsHashType srsHash;
    srsHash.emplace(0,new dateHashType());
    allTeamsHashType allTeamsHash = Team::getTeams();

    //initialize the srsHash with the initial srs values
    boost::gregorian::day_iterator ditr(ConstantSeasonInfo::Instance()->get(year,"season start") +
                                        boost::gregorian::date_duration(14));//2 weeks after start of season
    for (; ditr <= ConstantSeasonInfo::Instance()->get(year,"tournament start"); ++ditr) {
        std::string date = boost::gregorian::to_iso_extended_string(*ditr);
        srsHash[0]->emplace(date,new teamHashType());
        for (auto &team : allTeamsHash){
            if (startFromOriginalSRS)
                srsHash[0]->at(date)->emplace(team.first, team.second->WAverageOnDate(*ditr)->getOrigSRS());
            else
                srsHash[0]->at(date)->emplace(team.first, team.second->WAverageOnDate(*ditr)->getSrs());
        }
    }

    std::vector<double> srsSOS;
    for (int i = 1; i < numIterations; i++){
        srsHash.emplace(i, new dateHashType());
        boost::gregorian::day_iterator day(ConstantSeasonInfo::Instance()->get(year,"season start") +
                                           boost::gregorian::date_duration(14));
        for (; day <= ConstantSeasonInfo::Instance()->get(year,"tournament start"); ++day) {
            std::string date = boost::gregorian::to_iso_extended_string(*day);
            srsHash[i]->emplace(date, new teamHashType());
            for (auto &team : allTeamsHash){
                srsSOS = calcSRS(srsHash[i-1]->at(date),team.first,*day);
                srsHash[i]->at(date)->emplace(team.first,srsSOS[0]);
            }
        }

        if (verbose) {
            //this will show the srs for 3 different teams (a generally good team, a generally
            //mediocre team, and a generally bad team, on four different dates throughout the
            //season, the first date SRS is calculated (14 days after the season starts),
            //then 50 days into the season, 100 days into the season, and the day the
            //tournament starts.
            char team1[256], team2[256], team3[256];
            sprintf(team1, "%i north carolina", year);
            sprintf(team2, "%i bradley", year);
            sprintf(team3, "%i grambling state", year);
            std::cout << i << "\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "season start") +
                    boost::gregorian::date_duration(14)))->at(team1), 3) <<
            "\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "season start") +
                    boost::gregorian::date_duration(14)))->at(team2), 3) <<
            "\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "season start") +
                    boost::gregorian::date_duration(14)))->at(team3), 3) <<
            "\t\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "season start") +
                    boost::gregorian::date_duration(50)))->at(team1), 3) <<
            "\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "season start") +
                    boost::gregorian::date_duration(50)))->at(team2), 3) <<
            "\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "season start") +
                    boost::gregorian::date_duration(50)))->at(team3), 3) <<
            "\t\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "season start") +
                    boost::gregorian::date_duration(100)))->at(team1), 3) <<
            "\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "season start") +
                    boost::gregorian::date_duration(100)))->at(team2), 3) <<
            "\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "season start") +
                    boost::gregorian::date_duration(100)))->at(team3), 3) <<
            "\t\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "tournament start")))->at(team1), 3) <<
            "\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "tournament start")))->at(team2), 3) <<
            "\t" << doubleFormatter(srsHash[i]->at(boost::gregorian::to_iso_extended_string(
                    ConstantSeasonInfo::Instance()->get(year, "tournament start")))->at(team3), 3) << std::endl;
        }
    }

    if (writeOutput) {
        TeamWAverage *wAverage;

        for (auto & team : allTeamsHash){
            //open up the file
            std::ofstream waveragesFile;
            sprintf(path, "%s/cpp/NCAA_C/teams/%i/teams.%s.waverages.d",
                    homePath, year, boost::replace_all_copy(team.first, " ", "_").c_str());
            waveragesFile.open(path);

            boost::gregorian::day_iterator day(ConstantSeasonInfo::Instance()->get(year, "season start") +
                                               boost::gregorian::date_duration(14));
            for (; day <= ConstantSeasonInfo::Instance()->get(year, "tournament start"); ++day) {
                std::string date = boost::gregorian::to_iso_extended_string(*day);
                srsSOS = calcSRS(srsHash[numIterations - 1]->at(date), team.first, *day);

                wAverage = team.second->WAverageOnDate(*day);

                //write in the file
                waveragesFile << team.first;
                waveragesFile << "," << day->year();
                waveragesFile << "," << day->month().as_number();
                waveragesFile << "," << day->day();
                waveragesFile << "," << wAverage->getOpts();
                waveragesFile << "," << wAverage->getOtwo()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getOtwo()->P(), 3);
                waveragesFile << "," << wAverage->getOthree()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getOthree()->P(), 3);
                waveragesFile << "," << wAverage->getOft()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getOft()->P(), 3);
                waveragesFile << "," << wAverage->getOor()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getOor()->P(), 3);
                waveragesFile << "," << wAverage->getOdr()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getOdr()->P(), 3);
                waveragesFile << "," << wAverage->getOto()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getOto()->P(), 3);
                waveragesFile << "," << wAverage->getDpts();
                waveragesFile << "," << wAverage->getDtwo()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getDtwo()->P(), 3);
                waveragesFile << "," << wAverage->getDthree()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getDthree()->P(), 3);
                waveragesFile << "," << wAverage->getDft()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getDft()->P(), 3);
                waveragesFile << "," << wAverage->getDor()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getDor()->P(), 3);
                waveragesFile << "," << wAverage->getDdr()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getDdr()->P(), 3);
                waveragesFile << "," << wAverage->getDto()->A();
                waveragesFile << "," << doubleFormatter(wAverage->getDto()->P(), 3);
                waveragesFile << "," << doubleFormatter(wAverage->getRpi(), 3);
                waveragesFile << "," << doubleFormatter(wAverage->getOrigSRS(), 3);
                waveragesFile << "," << doubleFormatter(srsSOS[0], 3);
                waveragesFile << "," << doubleFormatter(srsSOS[1], 3);
                waveragesFile << "," << wAverage->getNum_games() << std::endl;
            }

            //close the file
            waveragesFile.close();
        }
    }

    return 0;
}

std::vector<double> calcSRS(std::unordered_map<std::string, double>* srsHash, std::string teamName, boost::gregorian::date date){
    Team *team = Team::findTeam(teamName);

    double opp_srs = 0;
    int num_opps = 0, pt_diff = 0;

    typedef std::unordered_map<std::string, TeamGame*> gamesHashType;
    gamesHashType gamesHash = team->getGamesByDate();

    for (auto &game : gamesHash){
        if (game.second->getDate() >= date) continue; //only use games before date
        if (srsHash->find(game.second->getOpp()) == srsHash->end()) continue; //exclude non-Division-I schools

        opp_srs += srsHash->at(game.second->getOpp());
        pt_diff += game.second->getOpts() - game.second->getDpts();
        num_opps++;
    }

    double srs = 0, sos = 0;

    if (num_opps > 0) {
        if (opp_srs != opp_srs){ //check for nan
            srs = pt_diff/num_opps;
            sos = 0;
        }
        else {
            srs = (pt_diff + opp_srs) / num_opps;
            sos = opp_srs / num_opps;
        }
    }

    std::vector<double> result = {srs,sos};

    return result;
}

void printOptions(){// y:vn:oO
    std::cout << std::endl;
    std::cout << "generateSRS Usage options:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y (int) year to iterate SRS for (no default)[Required]" << std::endl;
    std::cout << "\t-n (int) number of iterations to do (default: 100)[Optional]" << std::endl;
    std::cout << "\t-o write output to file, overwriting current file (false)[Optional]" << std::endl;
    std::cout << "\t-O start calculations for original SRS (false)[Optional]" << std::endl;
    std::cout << "\t-v verbose (false)[Optional]" << std::endl;
    std::cout << "\t-h print this message" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: $CLION/generateSRS -y 2015 -n 100 -o -O -v" << std::endl;
    std::cout << std::endl;
    std::cout << "The program iterates over the SRS values to come up with the final values." << std::endl;
    std::cout << "It processes one year at a time.  It defaults to iterate 100 times.  You" << std::endl;
    std::cout << "may choose to write the output to file with the -o switch, or print the " << std::endl;
    std::cout << "results for three different teams at three different dates throughout the" << std::endl;
    std::cout << "season at each iteration.  This latter option is so that you can see whether" << std::endl;
    std::cout << "or not the results are converging.  Since you may wish start this process" << std::endl;
    std::cout << "over, due to changes in a few weighted average files, or a change to the" << std::endl;
    std::cout << "calculation, without having to start from the current iteration, it is" << std::endl;
    std::cout << "possible to start over using the original SRS values using the -O switch." << std::endl;
    std::cout << std::endl;
}
