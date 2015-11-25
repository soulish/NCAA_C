//
// Created by soulish on 11/20/15.
//

#include "Team.h"

//declaring the static variables
std::unordered_map<string,Team*> Team::teams;
Team *Team::lastTeam;

Team::Team(string teamname) {
    name = teamname;
    year = stoi(teamname.substr(0,4)); //teamnames must follow this convention
    tournament_team = false;
    std::pair<std::string,Team*> teampair (teamname,this);
    teams.insert(teampair);
    lastTeam = this;
}

void Team::addGame(TeamGame* g) {
    string d = boost::gregorian::to_iso_extended_string(*(g->date));
    std::pair<string,TeamGame*> gamePair (d,g);
    gamesByDate.insert(gamePair);
}

void Team::addAverage(TeamAverage *a) {
    string d = boost::gregorian::to_iso_extended_string(*(a->date));
    std::pair<string,TeamAverage*> averagePair (d,a);
    averagesByDate.insert(averagePair);
}

void Team::addWAverage(TeamWAverage *w) {
    string d = boost::gregorian::to_iso_extended_string(*(w->date));
    std::pair<string,TeamWAverage*> waveragePair (d,w);
    waveragesByDate.insert(waveragePair);
}

