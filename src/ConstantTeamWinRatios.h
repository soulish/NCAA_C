//
// Created by soulish on 12/2/15.
//

#ifndef NCAA_C_CONSTANTTEAMWINRATIOS_H
#define NCAA_C_CONSTANTTEAMWINRATIOS_H


#include <iostream>
#include <unordered_map>

//Singleton class to hold the team 5 year averages
//
//Use it like this:
//
//    ConstantTeamWinRatios::Instance()->initialize(pathToFile);
//    ConstantTeamWinRatios::Instance()->get(2016,"home");
class ConstantTeamWinRatios {
public:
    static ConstantTeamWinRatios* Instance();

    void initialize(std::string path);
    double get(int,std::string);

private:
    ConstantTeamWinRatios(){};
    ConstantTeamWinRatios(ConstantTeamWinRatios const&){};
    ConstantTeamWinRatios& operator=(ConstantTeamWinRatios const&){};

    static ConstantTeamWinRatios* uniqueInstance;

    std::unordered_map< int, std::unordered_map<std::string, double > * > team_win_ratios;
};


#endif //NCAA_C_CONSTANTTEAMWINRATIOS_H
