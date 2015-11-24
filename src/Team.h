//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_TEAM_H
#define NCAA_C_TEAM_H

#include <string>

using namespace std;

class Team {
public:
    string name;
    bool tournament_team;
    int year;

    Team(string teamname);
};


#endif //NCAA_C_TEAM_H
