#include <iostream>
#include "src/Team.h"
#include "src/Pct.h"
#include "src/Pcts.h"
#include "src/Game.h"
#include "src/TeamGame.h"

using namespace std;

int main() {

    Team *a = new Team("2015 North Carolina");

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

    cout << a->year << endl;
    cout << a->name << endl;
    cout << endl << p->sdom << endl;
    cout << endl << pcts->p_bar() << "\t" << pcts->length() <<
            "\t" << pcts->average('m') << "\t" << pcts->weighted_average() <<
            "\t" << pcts->weighted_average_alt() <<  endl;

    TeamGame *tg = new TeamGame("2015 north carolina","2015 iowa",7,new boost::gregorian::date(2014,12,3),"home",0,55,60,19,68,4,23,13,17,24,46,8,5,3,10,22,18,55,3,20,21,24,17,42,9,3,6,12,20,-8.0f);
    cout << endl;
    cout << tg->efg->p << endl;

    return 0;
}