//
// Created by soulish on 12/2/15.
//

#ifndef NCAA_C_CONSTANTTEAMPOINTDIFFERENTIALS_H
#define NCAA_C_CONSTANTTEAMPOINTDIFFERENTIALS_H


#include <iostream>
#include <unordered_map>

//Singleton class to hold the team 5 year averages
//
//Use it like this:
//
//    ConstantTeamPointDifferentials::Instance()->initialize(pathToFile);
//    ConstantTeamPointDifferentials::Instance()->get(2016,"home");
class ConstantTeamPointDifferentials {
public:
    static ConstantTeamPointDifferentials* Instance();

    void initialize(std::string path);
    double get(int,std::string);

private:
    ConstantTeamPointDifferentials(){};
    ConstantTeamPointDifferentials(ConstantTeamPointDifferentials const&){};
    ConstantTeamPointDifferentials& operator=(ConstantTeamPointDifferentials const&){};

    static ConstantTeamPointDifferentials* uniqueInstance;

    std::unordered_map< int, std::unordered_map<std::string, double > * > team_point_differentials;
};


#endif //NCAA_C_CONSTANTTEAMPOINTDIFFERENTIALS_H
