//
// Created by soulish on 11/20/15.
//

#include "Team.h"
#include <string>

std::unordered_map<string,Team*> Team::teams;

Team::Team(string teamname) {
    name = teamname;
    year = stoi(teamname.substr(0,4)); //teamnames must follow this convention
    tournament_team = false;
    std::pair<std::string,Team*> teampair (teamname,this);
    teams.insert(teampair);
}

Team *Team::findteam(string name) {
    return teams[name];
}
int Team::getNumTeams() {
    return (int)teams.size();
}
