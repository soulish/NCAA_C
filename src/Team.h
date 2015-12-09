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

class Team {
public:
    //Constructor
    Team(std::string teamname);
    //Destructor
    virtual ~Team() {
        //make sure to remove this teamname from the Hash
        teams.erase(this->name);
    }
    //Copy Constructor
    //Copy Assignment

    //Getters
    const std::string &getName() const {   return name;  }
    int getYear() const {  return year;  }
    void setTournament_team(bool tournament_team) { Team::tournament_team = tournament_team; }
    bool isTournament_team() const {  return tournament_team;  }
    const std::unordered_map<std::string, TeamGame *> &getGamesByDate() const {  return gamesByDate;  }
    const std::unordered_map<std::string, TeamAverage *> &getAveragesByDate() const  {  return averagesByDate;  }
    const std::unordered_map<std::string, TeamWAverage *> &getWAveragesByDate() const {  return waveragesByDate;  }
    TeamGame *GameOnDate(boost::gregorian::date d) const;
    TeamAverage *AverageOnDate(boost::gregorian::date d) const;
    TeamWAverage *WAverageOnDate(boost::gregorian::date d) const;

    //Other methods
    //adding games and averages
    void addGame(TeamGame* g);
    void addAverage(TeamAverage* a);
    void addWAverage(TeamWAverage* w);

    void addGames(std::string);
    void addAverages(std::string);
    void addWAverages(std::string);

    std::vector<double> calcWeightedAverage(boost::gregorian::date date);

    //static functions
    static int getNumTeams(){  return (int)teams.size();  }
    static const std::unordered_map<std::string, Team *> &getTeams() {  return teams; }
    static Team *getLastTeam() {  return lastTeam;  }
    static Team *findTeam(std::string name);

private:
    std::string name;
    bool tournament_team;
    int year;
    std::unordered_map<std::string,TeamGame*>     gamesByDate;      //date strings are of form YYYY-MM-DD
    std::unordered_map<std::string,TeamAverage*>  averagesByDate;
    std::unordered_map<std::string,TeamWAverage*> waveragesByDate;

    static std::unordered_map<std::string, Team*> teams; //hash to store all the teams by name
    static Team *lastTeam;

};


#endif //NCAA_C_TEAM_H
