//Note, this file is being used purely for testing purposes.
//It exists to ensure that all the code is working properly,
//it is not useful in and of itself.

#include <iostream>
#include <stdlib.h>
#include "src/Team.h"
#include "src/Pct.h"
#include "src/Pcts.h"
#include "src/Game.h"
#include "src/TeamGame.h"
#include "src/readTeams.h"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/foreach.hpp>


using namespace std;

int main() {
    typedef std::unordered_map<string, Team*> unordered_map;
    Team *a = new Team("2015 north carolina");
    new Team("2015 alabama");
    new Team("2015 clemson");
    cout << Team::getNumTeams() << endl;
    unordered_map teams = Team::getTeams();

    BOOST_FOREACH(unordered_map::value_type &p, teams) {
                    std::cout << "From foreach_ " << p.first << ";" << p.second->getName() << '\n';
                }

    Pct *p = new Pct(50,100);

    Pcts *pcts = new Pcts();
    pcts->add_pct(*p);
    pcts->add_pct(*(new Pct(25,100)));
    pcts->add_pct(*(new Pct(35,110)));
    pcts->add_pct(*(new Pct(55,120)));
    pcts->add_pct(*(new Pct(45,90)));
    pcts->add_pct(*(new Pct(15,80)));
    pcts->add_pct(*(new Pct(25,100)));
    pcts->add_pct(*(new Pct(11,100)));
    pcts->add_pct(*(new Pct(24,100)));
    pcts->add_pct(*(new Pct(45,110)));
    pcts->add_pct(*(new Pct(43,120)));

    cout << a->getYear() << endl;
    cout << a->getName() << endl;
    cout << endl << p->getSdom() << endl;
    cout << endl << pcts->p_bar() << "\t" << pcts->length() <<
            "\t" << pcts->average('m') << "\t" << pcts->weighted_average() <<
            "\t" << pcts->weighted_average_alt() <<  endl;

    char* home, path[256];
    home = getenv("HOME");
    sprintf(path,"%s/cpp/NCAA_C/teams/2015/teams.2015_north_carolina.games.d",home);
    a->addGames(path);

    typedef std::unordered_map<string, TeamGame*> unordered_map2;
    std::unordered_map<string, TeamGame *> games = a->getGamesByDate();
    cout << games["2014-11-16"]->getOpts() << "-" << games["2014-11-16"]->getDpts() << endl;
    cout << games["2014-11-16"]->getOfg()->getM() << "-" << games["2014-11-16"]->getOfg()->getA() << " = " << games["2014-11-16"]->getOfg()->getP() << endl;

    Team *c;
    c = Team::findTeam("2015 alabama");
    sprintf(path,"%s/cpp/NCAA_C/teams/2015/teams.2015_alabama.games.d",home);
    c->addGames(path);
    games = c->getGamesByDate();
    BOOST_FOREACH(unordered_map2::value_type &q, games) {
                    std::cout << "From foreach_ " << q.first << ";" << q.second->getOpp() << '\n';
                }


    sprintf(path,"%s/cpp/NCAA_C/teams/2015/",home);
    readTeamsFromDir(path);

    Team *e;
    e = Team::findTeam("2015 tulane");
    games = e->getGamesByDate();
    BOOST_FOREACH(unordered_map2::value_type &q, games) {
                    std::cout << "From foreach_ " << q.first << ";" << q.second->getOpp() << '\n';
                }

    return 0;
}