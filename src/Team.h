//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_TEAM_H
#define NCAA_C_TEAM_H

#include <string>
#include <unordered_map>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "TeamGame.h"
#include "TeamAverage.h"
#include "TeamWAverage.h"

using namespace std;

class Team {
public:
    //Constructor
    Team(string teamname);
    //Destructor
    virtual ~Team() { teams.erase(this->name); } //make sure to remove this teamname from the Hash
    //Copy Constructor
    //Copy Assignment

    //Getters
    const string &getName() const {   return name;  }
    int getYear() const {  return year;  }
    void setTournament_team(bool tournament_team) { Team::tournament_team = tournament_team; }
    bool isTournament_team() const {  return tournament_team;  }
    const unordered_map<string, TeamGame *> &getGamesByDate() const {  return gamesByDate;  }
    const unordered_map<string, TeamAverage *> &getAveragesByDate() const  {  return averagesByDate;  }
    const unordered_map<string, TeamWAverage *> &getWaveragesByDate() const {  return waveragesByDate;  }

    //Other methods
    //adding games and averages
    void addGame(TeamGame* g);
    void addAverage(TeamAverage* a);
    void addWAverage(TeamWAverage* w);

    void addGames(string);

    //static functions
    static int getNumTeams(){  return (int)teams.size();  }
    static const unordered_map<string, Team *> &getTeams() {  return teams; }
    static Team *getLastTeam() {  return lastTeam;  }
    static Team *findTeam(string name){  return teams[name];  }

private:
    string name;
    bool tournament_team;
    int year;
    unordered_map<string,TeamGame*>     gamesByDate;      //date strings are of form YYYY-MM-DD
    unordered_map<string,TeamAverage*>  averagesByDate;
    unordered_map<string,TeamWAverage*> waveragesByDate;

    static unordered_map<string, Team*> teams; //hash to store all the teams by name
    static Team *lastTeam;

};


#endif //NCAA_C_TEAM_H
