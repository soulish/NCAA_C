//
// Created by soulish on 11/30/15.
//

#include <iostream>
#include <string>
#include <vector>
#include "src/Team.h"
#include "src/Pcts.h"
#include "src/readTeams.h"
#include <boost/foreach.hpp>

int main() {
    typedef std::unordered_map<std::string, Team *> unordered_map;
    typedef std::unordered_map<std::string, TeamGame *> unordered_map2;
    char *home, path[256];
    home = getenv("HOME");
    sprintf(path, "%s/cpp/NCAA_C/teams/2015/", home);

    std::string stats[] = {"fg","two","three","ft","or","dr","tr","as","st","bl","to","efg","ftmr"};
    std::unordered_map<std::string, Pcts*> pcts_tot;
    BOOST_FOREACH(std::string &s,stats){
                    pcts_tot.insert(std::pair<std::string,Pcts*>(s,new Pcts()));
                }


    Pcts *pcts_total = new Pcts();

    readTeamsFromDir(path);
    unordered_map teams = Team::getTeams();
    std::unordered_map<std::string, TeamGame *> games;

    BOOST_FOREACH(unordered_map::value_type &p, teams) {
                    games = p.second->getGamesByDate();
                    BOOST_FOREACH(unordered_map2::value_type & q, games) {
                                    pcts_total->add_pct(*(q.second->getOfg()));
                                    pcts_tot["fg"]->add_pct(*(q.second->getOfg()));
                                }
                }

    std::cout << pcts_total->p_bar() << std::endl;
    std::cout << pcts_tot["fg"]->p_bar() << std::endl;

    Pct test(100,200);
    std::cout << test.P() << "\t" << test.Variance() << "\t" << test.StandardDeviationOfStandardDeviation()<< std::endl;

    return 0;
}