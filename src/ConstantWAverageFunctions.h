//
// Created by soulish on 12/12/15.
//

#ifndef NCAA_C_CONSTANTWAVERAGEFUNCTIONS_H
#define NCAA_C_CONSTANTWAVERAGEFUNCTIONS_H

#include <iostream>
#include <unordered_map>
#include "TF1.h"
#include "TeamWAverage.h"

//Singleton class to hold the team 5 year averages
//
//Use it like this:
//
//    ConstantFunctions::Instance()->initialize(pathToFile);
//    ConstantFunctions::Instance()->evaluate(waverage1, waverage2);

class ConstantWAverageFunctions {
public:
    static ConstantWAverageFunctions * Instance();

    void initialize(std::string path);
    std::unordered_map<std::string, double> predictStats(TeamWAverage *w1, TeamWAverage *w2, int year);
    std::unordered_map<std::string, double> predictStats(TeamWAverage *w1, int year, std::string offOrDef = "offense");
    TF1 * getFunction(int year, std::string stat);

private:
    ConstantWAverageFunctions(){};
    ConstantWAverageFunctions(ConstantWAverageFunctions const&){};
    ConstantWAverageFunctions & operator=(ConstantWAverageFunctions const&){};

    static ConstantWAverageFunctions * uniqueInstance;

    std::unordered_map<int, std::unordered_map<std::string, TF1*> *> functions;
};


#endif //NCAA_C_CONSTANTWAVERAGEFUNCTIONS_H
