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
#include <boost/foreach.hpp>

int main() {
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
    readTeamsFromDir(path);

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

    //These are the averages
    std::cout << "\n\nAverages" << std::endl;
    BOOST_FOREACH(std::string &s,basic_stats){
                    std::cout << s << ": " << pcts_total[s]->average('m') << std::endl;
                }
    BOOST_FOREACH(std::string &s,stats){
                    std::cout << s << ".m: " << pcts_total[s]->average('m') << std::endl;
                    std::cout << s << ".a: " << pcts_total[s]->average('a') << std::endl;
                    std::cout << s << ".p: " << pcts_total[s]->p_bar() << std::endl;
                }

    //these are the ratios of the total average to the home averages, used for weighting later
    std::cout << "\n\nNeutral Ratios: home" << std::endl;
    BOOST_FOREACH(std::string &s,basic_stats){
                    std::cout << s << ": " << pcts_total[s]->average('m') / pcts_home[s]->average('m') << std::endl;
                }
    BOOST_FOREACH(std::string &s,stats){
                    std::cout << s << ".m: " << pcts_total[s]->average('m') / pcts_home[s]->average('m') << std::endl;
                    std::cout << s << ".a: " << pcts_total[s]->average('a') / pcts_home[s]->average('a') << std::endl;
                    std::cout << s << ".p: " << pcts_total[s]->p_bar() / pcts_home[s]->p_bar() << std::endl;
                }

    //these are the ratios of the total average to the away averages, used for weighting later
    std::cout << "\n\nNeutral Ratios: away" << std::endl;
    BOOST_FOREACH(std::string &s,basic_stats){
                    std::cout << s << ": " << pcts_total[s]->average('m') / pcts_away[s]->average('m') << std::endl;
                }
    BOOST_FOREACH(std::string &s,stats){
                    std::cout << s << ".m: " << pcts_total[s]->average('m') / pcts_away[s]->average('m') << std::endl;
                    std::cout << s << ".a: " << pcts_total[s]->average('a') / pcts_away[s]->average('a') << std::endl;
                    std::cout << s << ".p: " << pcts_total[s]->p_bar() / pcts_away[s]->p_bar() << std::endl;
                }

    //these are the ratios of the total average to the neutral site averages, used for weighting later
    //these really should be very close to 1
    std::cout << "\n\nNeutral Ratios: neutral" << std::endl;
    BOOST_FOREACH(std::string &s,basic_stats){
                    std::cout << s << ": " << pcts_total[s]->average('m') / pcts_neutral[s]->average('m') << std::endl;
                }
    BOOST_FOREACH(std::string &s,stats){
                    std::cout << s << ".m: " << pcts_total[s]->average('m') / pcts_neutral[s]->average('m') << std::endl;
                    std::cout << s << ".a: " << pcts_total[s]->average('a') /  pcts_neutral[s]->average('a') << std::endl;
                    std::cout << s << ".p: " << pcts_total[s]->p_bar() /  pcts_neutral[s]->p_bar() << std::endl;
                }

    //these are the point differentials at home vs away.
    std::cout << "\n\nPoint differentials" << std::endl;
    std::cout << "Home    : " << pt_diff_home / (double)pcts_home["opts"]->length() <<
            "\t\t" << pcts_home["opts"]->length() << std::endl;
    std::cout << "Away    : " << pt_diff_away / (double)pcts_away["opts"]->length() <<
            "\t\t" << pcts_away["opts"]->length() << std::endl;
    std::cout << "Neutral : " << pt_diff_neutral / (double)pcts_neutral["opts"]->length() <<
            "\t\t" << pcts_neutral["opts"]->length() << std::endl;

    std::cout << "\n\nWin Ratio" << std::endl;
    std::cout << "Home    : " << 0.5/(wins_home/(double)total_home) << std::endl;
    std::cout << "Away    : " << 0.5/(wins_away/(double)total_away) << std::endl;
    std::cout << "Neutral : " << 0.5/(wins_neutral/(double)total_neutral) << std::endl;

    return 0;
}