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
    typedef std::unordered_map<string, TeamGame*> unordered_map2;
    char* home, path[256];
    home = getenv("HOME");

    sprintf(path,"%s/cpp/NCAA_C/teams/2015/",home);
    readTeamsFromDir(path);

    Team *e;
    e = Team::findTeam("2015 tulane");
    if (e) {
        std::unordered_map<string, TeamGame *> games = e->getGamesByDate();
        BOOST_FOREACH(unordered_map2::value_type & q, games) {
                        std::cout << "From foreach_ " << q.first << ";" << q.second->getOpp() << '\n';
                    }
    }

    return 0;
}