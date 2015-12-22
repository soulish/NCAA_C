//
// Created by soulish on 12/21/15.
//

#ifndef NCAA_C_TOURNAMENTREGION_H
#define NCAA_C_TOURNAMENTREGION_H


#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <TH1.h>
#include "Team.h"
#include "ConstantSeasonInfo.h"
#include "ConstantGameFunction.h"

class TournamentRegion {
public:
    TournamentRegion(int _year);
    void addTeam(std::string team, int seed);
    void playInGames(TH1F* hist, bool verbose = true, std::string chosenTeam = "");
    void firstRound(TH1F* hist, bool verbose = true, std::string chosenTeam = "");
    void secondRound(TH1F* hist, bool verbose = true, std::string chosenTeam = "");
    void thirdRound(TH1F* hist, bool verbose = true, std::string chosenTeam = "");
    void fourthRound(TH1F* hist, bool verbose = true, std::string chosenTeam = "");
    void reset();

    //getters
    std::string getRegionChamp() { return regionChamp; }
    double getPct(std::string teamName) { return pcts[teamName]; }

private:
    const int year;
    std::unordered_map<std::string, int> teams;
    std::unordered_map<std::string, double> pcts;
    std::vector<std::string> firstRoundWinners;
    std::vector<std::string> secondRoundWinners;
    std::vector<std::string> thirdRoundWinners;
    std::string regionChamp;
    std::vector<int> playInSeeds;
    std::vector<int> seeds;
    std::unordered_map<std::string, std::array<double,5> *> pcts_all;
};


#endif //NCAA_C_TOURNAMENTREGION_H
