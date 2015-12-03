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
#include "src/Team.h"
#include "src/TeamGame.h"
#include "src/Pcts.h"
#include "src/readTeams.h"
#include "helpers/doubleFormatter.h"
#include <boost/foreach.hpp>

int main(int argc,char *argv[]) {
    int c, outYear;
    std::string inYears;
    std::vector<std::string> years;
    bool verbose = false;
    /*____________________________Parse Command Line___________________________*/
    while((c = getopt(argc,argv,"y:Y:v")) != -1){
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
            default:
                // not an option
                break;
        }
    }

    //ensure years and outYear are set
    if (years.empty()){
        years.push_back("2001");
        outYear = 2002;
    }

    typedef std::unordered_map<std::string, Team *> teamHash;
    typedef std::unordered_map<std::string, TeamGame *> teamGameHash;

    char *homePath, path[256];
    homePath = getenv("HOME");
    sprintf(path, "%s/cpp/NCAA_C/teams/2015/", homePath);

    //these are the stats that I will be taking the averages of.
    std::string basic_stats[] = {"opts","opf"};//these only matter in raw values, they don't have a m, a, and p.
    std::string stats[] = {"ofg","otwo","othree","oft","oor","odr","otr","oas","ost","obl","oto","oefg","oftmr"};

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
    BOOST_FOREACH(std::string &s,basic_stats){
                    pcts_home.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
                    pcts_away.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
                    pcts_neutral.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
                    pcts_total.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
                }
    BOOST_FOREACH(std::string &s,stats){
                    pcts_home.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
                    pcts_away.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
                    pcts_neutral.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
                    pcts_total.insert(std::pair<std::string,Pcts*>(s, new Pcts()));
                }

    //We look through the directory where the game files are,
    //add all the teams and their games.
    BOOST_FOREACH(std::string &year, years){
                    sprintf(path, "%s/cpp/NCAA_C/teams/%s/", homePath, year.c_str());
                    std::cout << "Doing " << year << std::endl;
                    readTeamsFromDir(path);
                }

    //containers for the teams and the teamgames
    teamHash teams = Team::getTeams();
    std::unordered_map<std::string, TeamGame*> games;

    //cycle through all the teams
    BOOST_FOREACH(teamHash::value_type &p, teams) {
                    games = p.second->getGamesByDate();
                    //cycle through all the games for this team
                    BOOST_FOREACH(teamGameHash::value_type & q, games) {
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
                                        pcts_home["opts"]->add_pct(Pct(q.second->getOpts(),5*q.second->getOpts()));
                                        pcts_home["opf"]->add_pct(Pct(q.second->getOpf(),5*q.second->getOpf()));
                                    }
                                    else if (q.second->getLoc() == "away") {
                                        pt_diff_away += q.second->getOpts() - q.second->getDpts();
                                        if (q.second->getWin() == 1) wins_away += 1;
                                        else losses_away += 1;
                                        total_away += 1;
                                        pcts_away["opts"]->add_pct(Pct(q.second->getOpts(),5*q.second->getOpts()));
                                        pcts_away["opf"]->add_pct(Pct(q.second->getOpf(),5*q.second->getOpf()));
                                    }
                                    else if (q.second->getLoc() == "neutral") {
                                        pt_diff_neutral += q.second->getOpts() - q.second->getDpts();
                                        if (q.second->getWin() == 1) wins_neutral += 1;
                                        else losses_neutral += 1;
                                        total_neutral += 1;
                                        pcts_neutral["opts"]->add_pct(Pct(q.second->getOpts(),5*q.second->getOpts()));
                                        pcts_neutral["opf"]->add_pct(Pct(q.second->getOpf(),5*q.second->getOpf()));
                                    }
                                    pcts_total["opts"]->add_pct(Pct(q.second->getOpts(),5*q.second->getOpts()));
                                    pcts_total["opf"]->add_pct(Pct(q.second->getOpf(),5*q.second->getOpf()));

                                    //cycle through the normal stats, with m, a, and p values
                                    BOOST_FOREACH(std::string &s,stats){
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

    if (verbose) {
        //These are the averages
        std::cout << "\n\nAverages" << std::endl;
        BOOST_FOREACH(std::string &s, basic_stats) {
                        std::cout << s << ": " << doubleFormatter(pcts_total[s]->average('m'), 3) << std::endl;
                    }
        BOOST_FOREACH(std::string &s, stats) {
                        std::cout << s << ".m: " << doubleFormatter(pcts_total[s]->average('m'), 3) << std::endl;
                        std::cout << s << ".a: " << doubleFormatter(pcts_total[s]->average('a'), 3) << std::endl;
                        std::cout << s << ".p: " << doubleFormatter(pcts_total[s]->p_bar(), 3) << std::endl;
                    }

        //these are the ratios of the total average to the home averages, used for weighting later
        std::cout << "\n\nNeutral Ratios: home" << std::endl;
        BOOST_FOREACH(std::string &s, basic_stats) {
                        std::cout << s << ": " <<
                        doubleFormatter(pcts_total[s]->average('m') / pcts_home[s]->average('m'), 3) << std::endl;
                    }
        BOOST_FOREACH(std::string &s, stats) {
                        std::cout << s << ".m: " <<
                        doubleFormatter(pcts_total[s]->average('m') / pcts_home[s]->average('m'), 3) << std::endl;
                        std::cout << s << ".a: " <<
                        doubleFormatter(pcts_total[s]->average('a') / pcts_home[s]->average('a'), 3) << std::endl;
                        std::cout << s << ".p: " <<
                        doubleFormatter(pcts_total[s]->p_bar() / pcts_home[s]->p_bar(), 3) << std::endl;
                    }

        //these are the ratios of the total average to the away averages, used for weighting later
        std::cout << "\n\nNeutral Ratios: away" << std::endl;
        BOOST_FOREACH(std::string &s, basic_stats) {
                        std::cout << s << ": " <<
                        doubleFormatter(pcts_total[s]->average('m') / pcts_away[s]->average('m'), 3) << std::endl;
                    }
        BOOST_FOREACH(std::string &s, stats) {
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
        BOOST_FOREACH(std::string &s, basic_stats) {
                        std::cout << s << ": " <<
                        doubleFormatter(pcts_total[s]->average('m') / pcts_neutral[s]->average('m'), 3) << std::endl;
                    }
        BOOST_FOREACH(std::string &s, stats) {
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
    std::cout << "Averages" << std::endl;
    std::cout << outYear << ",";
    std::cout << doubleFormatter(pcts_total["opts"]->average('m'),3) << ",";
    std::cout << doubleFormatter(pcts_total["opf"]->average('m'),3) << ",";
    BOOST_FOREACH(std::string &s,stats){
                    std::cout << doubleFormatter(pcts_total[s]->average('m'),3) << ",";
                    std::cout << doubleFormatter(pcts_total[s]->average('a'),3) << ",";
                    std::cout << doubleFormatter(pcts_total[s]->p_bar(),3) << ",";
                }
    std::cout << outYear << std::endl;

    std::cout << "\n\nNeutral Ratios" << std::endl;
    std::cout << outYear << ",";
    std::cout << "home" << ",";
    std::cout << doubleFormatter(pcts_total["opts"]->average('m') / pcts_home["opts"]->average('m'), 3) << ",";
    std::cout << doubleFormatter(pcts_total["opf"]->average('m') / pcts_home["opf"]->average('m'), 3) << ",";
    BOOST_FOREACH(std::string &s, stats) {
                    std::cout << doubleFormatter(pcts_total[s]->average('m') / pcts_home[s]->average('m'), 3) << ",";
                    std::cout << doubleFormatter(pcts_total[s]->average('a') / pcts_home[s]->average('a'), 3) << ",";
                    std::cout << doubleFormatter(pcts_total[s]->p_bar() / pcts_home[s]->p_bar(), 3) << ",";
                }
    std::cout << outYear << std::endl;

    //away
    std::cout << outYear << ",";
    std::cout << "away" << ",";
    std::cout << doubleFormatter(pcts_total["opts"]->average('m') / pcts_away["opts"]->average('m'), 3) << ",";
    std::cout << doubleFormatter(pcts_total["opf"]->average('m') / pcts_away["opf"]->average('m'), 3) << ",";
    BOOST_FOREACH(std::string &s, stats) {
                    std::cout << doubleFormatter(pcts_total[s]->average('m') / pcts_away[s]->average('m'), 3) << ",";
                    std::cout << doubleFormatter(pcts_total[s]->average('a') / pcts_away[s]->average('a'), 3) << ",";
                    std::cout << doubleFormatter(pcts_total[s]->p_bar() / pcts_away[s]->p_bar(), 3) << ",";
                }
    std::cout << outYear << std::endl;

    //neutral
    std::cout << outYear << ",";
    std::cout << "neutral" << ",";
    std::cout << doubleFormatter(pcts_total["opts"]->average('m') / pcts_neutral["opts"]->average('m'), 3) << ",";
    std::cout << doubleFormatter(pcts_total["opf"]->average('m') / pcts_neutral["opf"]->average('m'), 3) << ",";
    BOOST_FOREACH(std::string &s, stats) {
                    std::cout << doubleFormatter(pcts_total[s]->average('m') / pcts_neutral[s]->average('m'), 3) << ",";
                    std::cout << doubleFormatter(pcts_total[s]->average('a') / pcts_neutral[s]->average('a'), 3) << ",";
                    std::cout << doubleFormatter(pcts_total[s]->p_bar() / pcts_neutral[s]->p_bar(), 3) << ",";
                }
    std::cout << outYear << std::endl;

    //these are the point differentials at home vs away.
    std::cout << "\n\nPoint differentials" << std::endl;
    std::cout << outYear << ",";
    std::cout << doubleFormatter(pt_diff_home / (double) pcts_home["opts"]->length(), 3) << ",";
    std::cout << doubleFormatter(pt_diff_away / (double) pcts_away["opts"]->length(), 3) << ",";
    std::cout << doubleFormatter(pt_diff_neutral / (double) pcts_neutral["opts"]->length(), 3) << ",";
    std::cout << outYear << std::endl;

    std::cout << "\n\nWin Ratio" << std::endl;
    std::cout << outYear << ",";
    std::cout << doubleFormatter(0.5 / (wins_home / (double) total_home), 3) << ",";
    std::cout << doubleFormatter(0.5 / (wins_away / (double) total_away), 3) << ",";
    std::cout << doubleFormatter(0.5 / (wins_neutral / (double) total_neutral), 3) << ",";
    std::cout << outYear << std::endl;

    return 0;
}