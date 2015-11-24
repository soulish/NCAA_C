//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_TEAM_H
#define NCAA_C_TEAM_H

#include <string>
#include <unordered_map>

using namespace std;

class Team {
private:
    string name;
    bool tournament_team;
    int year;
    static std::unordered_map<string, Team*> teams; //hash to store all the teams by name

public:
    Team(string teamname);
    virtual ~Team() { teams.erase(this->name); } //make sure to remove this teamname from the Hash

    const string &getName() const {   return name;  }
    int getYear() const {  return year;  }
    bool isTournament_team() const {  return tournament_team;  }

    static int getNumTeams();
    static const unordered_map<string, Team *> &getTeams() {  return teams; }


    static Team *findteam(string name);
};


#endif //NCAA_C_TEAM_H
