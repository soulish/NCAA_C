//
// Created by soulish on 11/30/15.
//

#include <iostream>
#include <string>
#include <vector>
#include "src/Team.h"
#include "src/TeamGame.h"
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
                                    pcts_tot["fg"]->add_pct(*(q.second->getOfg()));
                                    pcts_tot["two"]->add_pct(*(q.second->getOtwo()));
                                    pcts_tot["three"]->add_pct(*(q.second->getOthree()));
                                    pcts_tot["ft"]->add_pct(*(q.second->getOft()));
                                    pcts_tot["or"]->add_pct(*(q.second->getOor()));
                                    pcts_tot["dr"]->add_pct(*(q.second->getOdr()));
                                    pcts_tot["tr"]->add_pct(*(q.second->getOtr()));
                                    pcts_tot["as"]->add_pct(*(q.second->getOas()));
                                    pcts_tot["st"]->add_pct(*(q.second->getOst()));
                                    pcts_tot["bl"]->add_pct(*(q.second->getObl()));
                                    pcts_tot["to"]->add_pct(*(q.second->getOto()));
                                    pcts_tot["efg"]->add_pct(*(q.second->getOefg()));
                                    pcts_tot["ftmr"]->add_pct(*(q.second->getOftmr()));
                                }
                }

    BOOST_FOREACH(std::string &s,stats){
                    std::cout << s << ": " << pcts_tot[s]->sum('m') << "/" << pcts_tot[s]->sum('a') <<
                            " = " << pcts_tot[s]->p_bar() << std::endl;
                }

    return 0;
}