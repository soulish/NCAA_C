//
// Created by soulish on 11/30/15.
//
// This file is for generating the averages for each of the stats over all teams
// in a year or a specified set of years.  It will yield the average of the number of
// successes (e.g. made field goals), attempts (e.g. attempted field goals), and
// percentage for each of the main stats.
//
// Then, it will also provide the ratio of these averages in home, away, and neutral
// games compared to all games (i.e. the sum of all home, neutral, and away games).
// This ratio is used when weighting the stats to come up with opponent-adjusted stats.

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "src/Team.h"
#include "src/TeamGame.h"
#include "src/Pcts.h"
#include "src/readTeams.h"
#include "helpers/doubleFormatter.h"

void printOptions();

int main(int argc,char *argv[]) {
    int c, outYear = 0;
    std::string inYears, outFileString;
    std::vector<std::string> years;
    std::vector<std::string> outFileNames;
    bool verbose = false, writeOutput = false;
    /*____________________________Parse Command Line___________________________*/
    while((c = getopt(argc,argv,"y:Y:vo:h")) != -1){
        switch(c){
            case 'Y':
                outYear = atoi(optarg);
                std::cout << "outYear: " << outYear << std::endl;
                break;
            case 'y':
                std::cout << "inYears: " << optarg << std::endl;
                inYears.assign(optarg);
                boost::split(years,inYears,boost::is_any_of(","));
                break;
            case 'v':
                verbose = true;
                break;
            case 'o':
                outFileString.assign(optarg);
                boost::split(outFileNames, outFileString, boost::is_any_of(","));
                writeOutput = true;
                break;
            case 'h':
                printOptions();
                return 0;
            default:
                // not an option
                break;
        }
    }

    //ensure years and outYear are set
    if (years.empty()){
        std::cout << "You must select the years to use for the averages with the -y switch " <<
                     "and a comma-separated list." << std::endl;
        printOptions();
        return 0;
    }
    if (outYear == 0){
        std::cout << "You must set the output year using the -Y switch" << std::endl;
        printOptions();
        return 0;
    }
    if (writeOutput && outFileNames.size() != 4){
        std::cout << "You must select 4 names for the files put in a comma-separated list" << std::endl;
        printOptions();
        return 0;
    }

    char *homePath, path[256];
    homePath = getenv("HOME");
    sprintf(path, "%s/cpp/NCAA_C/teams/2015/", homePath);

    //these are the stats that I will be taking the averages of.
    std::string basic_stats[] = {"opts","opf"};//these only matter in raw values, they don't have a m, a, and p.
    //these have m, a, and p values
    std::string stats[] = {"ofg","otwo","othree","oft","oor","odr","otr",
                           "oas","ost","obl","oto","oefg","oftmr"};

    //variables to keep track of the point differentials, wins, losses, and total games in the various locations
    int pt_diff_home = 0, pt_diff_away = 0, pt_diff_neutral = 0;
    int wins_home = 0, wins_away = 0, wins_neutral = 0;
    int losses_home = 0, losses_away = 0, losses_neutral = 0;
    int total_home = 0, total_away = 0, total_neutral = 0;

    //containers to hold the Pcts for home, away, neutral, and all together
    std::unordered_map<std::string, Pcts*> pcts_home;
    std::unordered_map<std::string, Pcts*> pcts_away;
    std::unordered_map<std::string, Pcts*> pcts_neutral;
    std::unordered_map<std::string, Pcts*> pcts_total;
    for (std::string &s : basic_stats){
        pcts_home.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
        pcts_away.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
        pcts_neutral.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
        pcts_total.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
    }
    for (std::string &s : stats){
        pcts_home.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
        pcts_away.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
        pcts_neutral.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
        pcts_total.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
    }

    //We look through the directory where the game files are,
    //add all the teams and their games.
    for (std::string &year : years){
        sprintf(path, "%s/cpp/NCAA_C/teams/%s/", homePath, year.c_str());
        std::cout << "Reading in games for " << year << std::endl;
        readTeamsFromDir(path);
    }

    //containers for the teams and the teamgames
    std::unordered_map<std::string, Team *> teams = Team::getTeams();
    std::unordered_map<std::string, TeamGame*> games;

    //cycle through all the teams
    for (auto &p : teams) {
        games = p.second->getGamesByDate();

        //cycle through all the games for this team
        for (auto &q : games) {
            //skip games with incomplete statistics
            if (q.second->getOpts() == 0) continue;
            if (q.second->getOposs() == 0) continue;

            //skip games involving non-Division-I schools
            if (!Team::findTeam(q.second->getOpp())) continue;

            //take care of the basic_stats and pt differential, etc, separated by location
            if (q.second->getLoc() == "home") {
                pt_diff_home += q.second->getOpts() - q.second->getDpts();
                if (q.second->getWin() == 1) wins_home += 1;
                else losses_home += 1;
                total_home += 1;
                pcts_home["opts"]->add_pct(Pct(q.second->getOpts(),5*q.second->getOpts()));//the second number is arbitrary
                pcts_home["opf"]->add_pct(Pct(q.second->getOpf(),5*q.second->getOpf()));//the second number is arbitrary
            }
            else if (q.second->getLoc() == "away") {
                pt_diff_away += q.second->getOpts() - q.second->getDpts();
                if (q.second->getWin() == 1) wins_away += 1;
                else losses_away += 1;
                total_away += 1;
                pcts_away["opts"]->add_pct(Pct(q.second->getOpts(),5*q.second->getOpts()));//the second number is arbitrary
                pcts_away["opf"]->add_pct(Pct(q.second->getOpf(),5*q.second->getOpf()));//the second number is arbitrary
            }
            else if (q.second->getLoc() == "neutral") {
                pt_diff_neutral += q.second->getOpts() - q.second->getDpts();
                if (q.second->getWin() == 1) wins_neutral += 1;
                else losses_neutral += 1;
                total_neutral += 1;
                pcts_neutral["opts"]->add_pct(Pct(q.second->getOpts(),5*q.second->getOpts()));//the second number is arbitrary
                pcts_neutral["opf"]->add_pct(Pct(q.second->getOpf(),5*q.second->getOpf()));//the second number is arbitrary
            }
            else{
                std::cout << "this game has no location!" << std::endl;
            }
            pcts_total["opts"]->add_pct(Pct(q.second->getOpts(),5*q.second->getOpts()));//the second number is arbitrary
            pcts_total["opf"]->add_pct(Pct(q.second->getOpf(),5*q.second->getOpf()));//the second number is arbitrary

            //cycle through the normal stats, with m, a, and p values
            for (std::string &s : stats){
                if (q.second->getLoc() == "home")
                    pcts_home[s]->add_pct(*(q.second->getPct(s)));
                else if (q.second->getLoc() == "away")
                    pcts_away[s]->add_pct(*(q.second->getPct(s)));
                else if (q.second->getLoc() == "neutral")
                    pcts_neutral[s]->add_pct(*(q.second->getPct(s)));
                pcts_total[s]->add_pct(*(q.second->getPct(s)));
            }
        }
    }

    //Spit this stuff out in a readable format
    if (verbose) {
        //These are the averages
        std::cout << "\n\nAverages: total\thome\t\taway\t\tneutral" << std::endl;
        for (std::string &s : basic_stats) {
            std::cout << s << ":\t" << doubleFormatter(pcts_total[s]->average('m'), 3) << "\t" <<
                    doubleFormatter(pcts_home[s]->average('m'),3) << "\t\t" <<
                    doubleFormatter(pcts_away[s]->average('m'),3) << "\t\t" <<
                    doubleFormatter(pcts_neutral[s]->average('m'),3) << std::endl;
        }
        for (std::string &s : stats) {
            std::cout << s << ".m:\t" << doubleFormatter(pcts_total[s]->average('m'), 3) << "\t" <<
                    doubleFormatter(pcts_home[s]->average('m'),3) << "\t\t" <<
                    doubleFormatter(pcts_away[s]->average('m'),3) << "\t\t" <<
                    doubleFormatter(pcts_neutral[s]->average('m'),3) << std::endl;
            std::cout << s << ".a:\t" << doubleFormatter(pcts_total[s]->average('a'), 3) << "\t" <<
                    doubleFormatter(pcts_home[s]->average('a'),3) << "\t\t" <<
                    doubleFormatter(pcts_away[s]->average('a'),3) << "\t\t" <<
                    doubleFormatter(pcts_neutral[s]->average('a'),3) << std::endl;
            std::cout << s << ".p:\t" << doubleFormatter(pcts_total[s]->p_bar(), 3) << "\t" <<
                    doubleFormatter(pcts_home[s]->p_bar(),3) << "\t\t" <<
                    doubleFormatter(pcts_away[s]->p_bar(),3) << "\t\t" <<
                    doubleFormatter(pcts_neutral[s]->p_bar(),3) << std::endl;
        }

        //these are the ratios of the total average to the home averages, used for weighting later
        //see below for more details
        std::cout << "\n\nNeutral Ratios: home" << std::endl;
        for (std::string &s : basic_stats) {
            std::cout << s << ": " <<
            doubleFormatter(pcts_total[s]->average('m') / pcts_home[s]->average('m'), 3) << std::endl;
        }
        for (std::string &s : stats) {
            std::cout << s << ".m: " <<
            doubleFormatter(pcts_total[s]->average('m') / pcts_home[s]->average('m'), 3) << std::endl;
            std::cout << s << ".a: " <<
            doubleFormatter(pcts_total[s]->average('a') / pcts_home[s]->average('a'), 3) << std::endl;
            std::cout << s << ".p: " <<
            doubleFormatter(pcts_total[s]->p_bar() / pcts_home[s]->p_bar(), 3) << std::endl;
        }

        //these are the ratios of the total average to the away averages, used for weighting later
        std::cout << "\n\nNeutral Ratios: away" << std::endl;
        for (std::string &s : basic_stats) {
            std::cout << s << ": " <<
            doubleFormatter(pcts_total[s]->average('m') / pcts_away[s]->average('m'), 3) << std::endl;
        }
        for (std::string &s : stats) {
            std::cout << s << ".m: " <<
            doubleFormatter(pcts_total[s]->average('m') / pcts_away[s]->average('m'), 3) << std::endl;
            std::cout << s << ".a: " <<
            doubleFormatter(pcts_total[s]->average('a') / pcts_away[s]->average('a'), 3) << std::endl;
            std::cout << s << ".p: " <<
            doubleFormatter(pcts_total[s]->p_bar() / pcts_away[s]->p_bar(), 3) << std::endl;
        }

        //these are the ratios of the total average to the neutral site averages, used for weighting later
        //these really should be very close to 1
        std::cout << "\n\nNeutral Ratios: neutral" << std::endl;
        for (std::string &s : basic_stats) {
            std::cout << s << ": " <<
            doubleFormatter(pcts_total[s]->average('m') / pcts_neutral[s]->average('m'), 3) << std::endl;
        }
        for (std::string &s : stats) {
            std::cout << s << ".m: " <<
            doubleFormatter(pcts_total[s]->average('m') / pcts_neutral[s]->average('m'), 3) << std::endl;
            std::cout << s << ".a: " <<
            doubleFormatter(pcts_total[s]->average('a') / pcts_neutral[s]->average('a'), 3) << std::endl;
            std::cout << s << ".p: " <<
            doubleFormatter(pcts_total[s]->p_bar() / pcts_neutral[s]->p_bar(), 3) << std::endl;
        }

        //these are the point differentials at home vs away.
        std::cout << "\n\nPoint differentials" << std::endl;
        std::cout << "Home    : " << doubleFormatter(pt_diff_home / (double) pcts_home["opts"]->length(), 3) <<
        "\t\t" << pcts_home["opts"]->length() << std::endl;
        std::cout << "Away    : " << doubleFormatter(pt_diff_away / (double) pcts_away["opts"]->length(), 3) <<
        "\t\t" << pcts_away["opts"]->length() << std::endl;
        std::cout << "Neutral : " << doubleFormatter(pt_diff_neutral / (double) pcts_neutral["opts"]->length(), 3) <<
        "\t\t" << pcts_neutral["opts"]->length() << std::endl;

        std::cout << "\n\nWin Ratio" << std::endl;
        std::cout << "Home    : " << doubleFormatter(0.5 / (wins_home / (double) total_home), 3) << std::endl;
        std::cout << "Away    : " << doubleFormatter(0.5 / (wins_away / (double) total_away), 3) << std::endl;
        std::cout << "Neutral : " << doubleFormatter(0.5 / (wins_neutral / (double) total_neutral), 3) << std::endl;
    }

    //version for text file
    if (writeOutput) {
        sprintf(path, "%s/cpp/NCAA_C/constants/%s", homePath, outFileNames[0].c_str());
        std::ofstream averagesFile(path, std::ios::app);

        averagesFile << outYear << ",";
        averagesFile << doubleFormatter(pcts_total["opts"]->average('m'), 3) << ",";
        averagesFile << doubleFormatter(pcts_total["opf"]->average('m'), 3) << ",";
        for (std::string &s :stats) {
            averagesFile << doubleFormatter(pcts_total[s]->average('m'), 3) << ",";
            averagesFile << doubleFormatter(pcts_total[s]->average('a'), 3) << ",";
            averagesFile << doubleFormatter(pcts_total[s]->p_bar(), 3) << ",";
        }
        averagesFile << outYear << std::endl;
        averagesFile.close();

        //these "neutral ratios" are actually ratios to the totals, not the ratio to the neutral
        //site values.  The reason I chose to use these instead of the average in neutral-site
        //games is that neutral-site games are most likely to be at the very beginning of the season
        //and at the very end of the season (conference tournaments and end-of-year tournaments).
        //Those games can therefore be a bit skewed and it seems safest to use the average
        //of all games together to properly "neutralize" our results.
        sprintf(path, "%s/cpp/NCAA_C/constants/%s", homePath, outFileNames[1].c_str());
        std::ofstream ratiosFile(path, std::ios::app);
        ratiosFile << outYear << ",";
        ratiosFile << "home" << ",";
        ratiosFile << doubleFormatter(pcts_total["opts"]->average('m') / pcts_home["opts"]->average('m'), 3) << ",";
        ratiosFile << doubleFormatter(pcts_total["opf"]->average('m') / pcts_home["opf"]->average('m'), 3) << ",";
        for (std::string &s : stats) {
            ratiosFile << doubleFormatter(pcts_total[s]->average('m') / pcts_home[s]->average('m'), 3) << ",";
            ratiosFile << doubleFormatter(pcts_total[s]->average('a') / pcts_home[s]->average('a'), 3) << ",";
            ratiosFile << doubleFormatter(pcts_total[s]->p_bar() / pcts_home[s]->p_bar(), 3) << ",";
        }
        ratiosFile << outYear << std::endl;

        //away
        ratiosFile << outYear << ",";
        ratiosFile << "away" << ",";
        ratiosFile << doubleFormatter(pcts_total["opts"]->average('m') / pcts_away["opts"]->average('m'), 3) << ",";
        ratiosFile << doubleFormatter(pcts_total["opf"]->average('m') / pcts_away["opf"]->average('m'), 3) << ",";
        for (std::string &s : stats) {
            ratiosFile << doubleFormatter(pcts_total[s]->average('m') / pcts_away[s]->average('m'), 3) << ",";
            ratiosFile << doubleFormatter(pcts_total[s]->average('a') / pcts_away[s]->average('a'), 3) << ",";
            ratiosFile << doubleFormatter(pcts_total[s]->p_bar() / pcts_away[s]->p_bar(), 3) << ",";
        }
        ratiosFile << outYear << std::endl;

        //neutral
        ratiosFile << outYear << ",";
        ratiosFile << "neutral" << ",";
        ratiosFile << doubleFormatter(pcts_total["opts"]->average('m') / pcts_neutral["opts"]->average('m'), 3) << ",";
        ratiosFile << doubleFormatter(pcts_total["opf"]->average('m') / pcts_neutral["opf"]->average('m'), 3) << ",";
        for (std::string &s : stats) {
            ratiosFile << doubleFormatter(pcts_total[s]->average('m') / pcts_neutral[s]->average('m'), 3) << ",";
            ratiosFile << doubleFormatter(pcts_total[s]->average('a') / pcts_neutral[s]->average('a'), 3) << ",";
            ratiosFile << doubleFormatter(pcts_total[s]->p_bar() / pcts_neutral[s]->p_bar(), 3) << ",";
        }
        ratiosFile << outYear << std::endl;
        ratiosFile.close();

        //these are the point differentials at home vs away.
        sprintf(path, "%s/cpp/NCAA_C/constants/%s", homePath, outFileNames[2].c_str());
        std::ofstream ptDiffsFile(path, std::ios::app);
        ptDiffsFile << outYear << ",";
        ptDiffsFile << doubleFormatter(pt_diff_home / (double) pcts_home["opts"]->length(), 3) << ",";
        ptDiffsFile << doubleFormatter(pt_diff_away / (double) pcts_away["opts"]->length(), 3) << ",";
        ptDiffsFile << doubleFormatter(pt_diff_neutral / (double) pcts_neutral["opts"]->length(), 3) << ",";
        ptDiffsFile << outYear << std::endl;
        ptDiffsFile.close();

        sprintf(path, "%s/cpp/NCAA_C/constants/%s", homePath, outFileNames[3].c_str());
        std::ofstream winsFile(path, std::ios::app);
        winsFile << outYear << ",";
        winsFile << doubleFormatter(0.5 / (wins_home / (double) total_home), 3) << ",";
        winsFile << doubleFormatter(0.5 / (wins_away / (double) total_away), 3) << ",";
        winsFile << doubleFormatter(0.5 / (wins_neutral / (double) total_neutral), 3) << ",";
        winsFile << outYear << std::endl;
        winsFile.close();
    }
    return 0;
}

void printOptions(){
    std::cout << std::endl;
    std::cout << "generateAverages Usage options:" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-y comma-separated list of years (no default)[Required]" << std::endl;
    std::cout << "\t-Y year to output (no default)[Required]" << std::endl;
    std::cout << "\t-o comma-separated list of out file names, see below (no default)[Required]" << std::endl;
    std::cout << "\t-v verbose (false)[Optional]" << std::endl;
    std::cout << "\t-h print this message" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: $CLION/generateAverages -y 2011,2012,2013,2014,2015 -Y 2016 " << std::endl;
    std::cout << "       -o team_5yr_averages.dd,team_neutral_ratios.dd,team_point_differentials.dd,team_win_ratios.dd" << std::endl;
    std::cout << std::endl;
    std::cout << "This file is for generating the averages for each of the stats over all teams" << std::endl;
    std::cout << "in a year or a specified set of years.  It will yield the average of the number of" << std::endl;
    std::cout << "successes (e.g. made field goals), attempts (e.g. attempted field goals), and" << std::endl;
    std::cout << "percentage for each of the main stats." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Then, it will also provide the ratio of these averages in home, away, and neutral" << std::endl;
    std::cout << "games compared to all games (i.e. the sum of all home, neutral, and away games)." << std::endl;
    std::cout << "This ratio is used when weighting the stats to come up with opponent-adjusted stats." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "You must provide the name of four files to hold the average, neutral ratios," << std::endl;
    std::cout << "point differentials, and win ratios.  They will be created in the constants directory." << std::endl;
    std::cout << std::endl;
}

