//
// Created by soulish on 12/2/15.
//

#ifndef NCAA_C_CONSTANTTEAMNEUTRALRATIOS_H
#define NCAA_C_CONSTANTTEAMNEUTRALRATIOS_H


#include <iostream>
#include <unordered_map>

//Singleton class to hold the team 5 year averages
//
//Use it like this:
//
//    ConstantTeamNeutralRatios::Instance()->initialize(pathToFile);
//    ConstantTeamNeutralRatios::Instance()->get(2016,"home","fgp");

class ConstantTeamNeutralRatios {
public:
    static ConstantTeamNeutralRatios* Instance();

    void initialize(std::string path);
    double get(int, std::string, std::string);

private:
    ConstantTeamNeutralRatios(){};
    ConstantTeamNeutralRatios(ConstantTeamNeutralRatios const&){};
    ConstantTeamNeutralRatios& operator=(ConstantTeamNeutralRatios const&){};

    static ConstantTeamNeutralRatios* uniqueInstance;

    std::unordered_map< int, std::unordered_map<std::string, std::unordered_map<std::string, double > * > * > team_neutral_ratios;
};


#endif //NCAA_C_CONSTANTTEAMNEUTRALRATIOS_H
