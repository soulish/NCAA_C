//Note, this file is being used purely for testing purposes.
//It exists to ensure that all the code is working properly,
//it is not useful in and of itself.

#include <iostream>
#include "src/Team.h"
#include "src/Pct.h"
#include "src/Pcts.h"
#include "src/Game.h"
#include "src/TeamGame.h"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/foreach.hpp>
#define foreach_         BOOST_FOREACH
#define foreach_r_       BOOST_REVERSE_FOREACH


using namespace std;

int main() {
    typedef std::unordered_map<string, Team*> unordered_map;
    Team *a = new Team("2015 north carolina");
    new Team("2015 alabama");
    new Team("2015 clemson");
    cout << Team::getNumTeams() << endl;
    unordered_map teams = Team::getTeams();

    Team *b = Team::findTeam("2015 north carolina");

    if (b) // b/c I return a NULL pointer if not found, this is necessary
        cout << "hi: " << b->getName() << endl;
    cout << "bye:" << teams["2015 north carolina"]->getName() << endl;

    foreach_(unordered_map::value_type &p, teams) {
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
    cout << endl << p->sdom << endl;
    cout << endl << pcts->p_bar() << "\t" << pcts->length() <<
            "\t" << pcts->average('m') << "\t" << pcts->weighted_average() <<
            "\t" << pcts->weighted_average_alt() <<  endl;

    TeamGame *tg = new TeamGame("2015 north carolina","2015 iowa",7,new boost::gregorian::date(2014,12,3),"home",0,55,60,19,68,4,23,13,17,24,46,8,5,3,10,22,18,55,3,20,21,24,17,42,9,3,6,12,20,-8.0f);
    cout << endl;
    cout << tg->oefg->p << endl;

    a->addGame(tg);

    string temp = boost::gregorian::to_iso_extended_string(*(tg->date));
    cout << temp << endl;

    a->addGames("/home/soulish/cpp/NCAA_C/teams/2015/teams.2015_north_carolina.games.d");

    std::unordered_map<string, TeamGame *> games = a->getGamesByDate();
    cout << games["2014-11-16"]->opts << "-" << games["2014-11-16"]->dpts << endl;
    cout << games["2014-11-16"]->ofg->m << "-" << games["2014-11-16"]->ofg->a << " = " << games["2014-11-16"]->ofg->p << endl;


    return 0;
}