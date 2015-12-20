//
// Created by soulish on 12/19/15.
//

#ifndef NCAA_C_CONSTANTGAMEFUNCTION_H
#define NCAA_C_CONSTANTGAMEFUNCTION_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include "TeamWAverage.h"

//Singleton class to hold the weights used in the game function
//
//Use it like this:
//
//    ConstantGameFunctionWeights::Instance()->initialize(pathToFile);
//    ConstantGameFunctionWeights::Instance()->getKeys();
//    ConstantGameFunctionWeights::Instance()->getWeights(year);

class ConstantGameFunction {
public:
    static ConstantGameFunction * Instance();

    void initialize(std::string path);
    std::vector<int> getKeys();
    std::vector<double> getWeights(int year);
    double predictGame(TeamWAverage *wa1, TeamWAverage *wa2, int year, std::string loc, std::string oppLoc);
    double predictGame(TeamWAverage *wa1, int year, std::string loc, std::string oppLoc);

private:
    ConstantGameFunction(){};
    ConstantGameFunction(ConstantGameFunction const&){};
    ConstantGameFunction & operator=(ConstantGameFunction const&){};

    static ConstantGameFunction * uniqueInstance;

    std::unordered_map< int, std::unordered_map<std::string, double> *> weights;
};


#endif //NCAA_C_CONSTANTGAMEFUNCTION_H
