//
// Created by soulish on 12/2/15.
//

#ifndef NCAA_C_CONSTANTTEAM5YEARAVERAGES_H
#define NCAA_C_CONSTANTTEAM5YEARAVERAGES_H


#include <iostream>
#include <unordered_map>

//Singleton class to hold the team 5 year averages
//
//Use it like this:
//
//    ConstantTeam5YearAverages::Instance()->initialize(pathToFile);
//    ConstantTeam5YearAverages::Instance()->get(2016,"fgp");
class ConstantTeam5YearAverages {
public:
    static ConstantTeam5YearAverages* Instance();

    void initialize(std::string path);
    double get(int,std::string);

private:
    ConstantTeam5YearAverages(){};
    ConstantTeam5YearAverages(ConstantTeam5YearAverages const&){};
    ConstantTeam5YearAverages& operator=(ConstantTeam5YearAverages const&){};

    static ConstantTeam5YearAverages* uniqueInstance;

    std::unordered_map< int, std::unordered_map<std::string, double > * > team_5yr_averages;

};


#endif //NCAA_C_CONSTANTTEAM5YEARAVERAGES_H
