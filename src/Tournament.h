//
// Created by soulish on 12/21/15.
//

#ifndef NCAA_C_TOURNAMENT_H
#define NCAA_C_TOURNAMENT_H

#include <iostream>
#include <array>
#include "TournamentRegion.h"
#include "Team.h"
#include "ConstantSeasonInfo.h"
#include "ConstantGameFunction.h"

class Tournament {
public:
    Tournament(int _year);
    void addTeam(int region, std::string teamName, int seed);
    void readTournamentInfo(std::string);
    void play(TH1F* hist, bool verbose = true, std::string chosenTeam = "");
    void calculateTotalPercentages(TH1F* hist);
    int getChallengePoints() { return challengePoints; }

private:
    const int year;
    std::unordered_map<std::string, int> teams;
    std::unordered_map<std::string, double> pcts;
    std::array<TournamentRegion*,4> regions;
    std::array<std::string, 4> regionChamps;
    std::array<std::string, 2> championshipGameTeams;
    std::string champion;
    double championPct;
    std::unordered_map<std::string, std::array<double,7> *> pcts_all;
    int challengePoints;
};


#endif //NCAA_C_TOURNAMENT_H
