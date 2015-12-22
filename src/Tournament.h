//
// Created by soulish on 12/21/15.
//

#ifndef NCAA_C_TOURNAMENT_H
#define NCAA_C_TOURNAMENT_H

#include <iostream>
#include "TournamentRegion.h"
#include "Team.h"
#include "ConstantSeasonInfo.h"
#include "ConstantGameFunction.h"

class Tournament {
public:
    Tournament(int _year);
    void addTeam(int region, std::string teamName, int seed);
    void play(TH1F* hist, bool verbose = true, std::string chosenTeam = "");
    void readTournamentInfo(std::string);

private:
    const int year;
    std::unordered_map<std::string, int> teams;
    std::unordered_map<std::string, double> pcts;
    std::array<TournamentRegion*,4> regions;
    std::array<std::string, 4> regionChamps;
    std::array<std::string, 2> championshipGameTeams;
    std::string champion;
    double championPct;
};


#endif //NCAA_C_TOURNAMENT_H
