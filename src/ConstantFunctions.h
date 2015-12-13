//
// Created by soulish on 12/12/15.
//

#ifndef NCAA_C_CONSTANTFUNCTIONS_H
#define NCAA_C_CONSTANTFUNCTIONS_H

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

class ConstantFunctions {
public:
    static ConstantFunctions* Instance();

    void initialize(std::string path);
    std::unordered_map<std::string, double> predict(TeamWAverage *w1, TeamWAverage *w2, int year);
    std::unordered_map<std::string, double> predict(TeamWAverage *w1, int year, std::string offOrDef = "offense");

private:
    ConstantFunctions(){};
    ConstantFunctions(ConstantFunctions const&){};
    ConstantFunctions& operator=(ConstantFunctions const&){};

    static ConstantFunctions* uniqueInstance;

    std::unordered_map<int, std::unordered_map<std::string, TF1*> *> functions;
};


#endif //NCAA_C_CONSTANTFUNCTIONS_H
