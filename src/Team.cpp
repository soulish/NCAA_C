//
// Created by soulish on 11/20/15.
//

#include "Team.h"
#include <string>

Team::Team(string teamname) {
    name = teamname;
    year = stoi(teamname.substr(0,4)); //teamnames must follow this convention
    tournament_team = false;
}
