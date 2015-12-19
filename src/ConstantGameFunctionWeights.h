//
// Created by soulish on 12/19/15.
//

#ifndef NCAA_C_CONSTANTGAMEFUNCTIONWEIGHTS_H
#define NCAA_C_CONSTANTGAMEFUNCTIONWEIGHTS_H

#include <iostream>
#include <unordered_map>
#include <vector>

//Singleton class to hold the weights used in the game function
//
//Use it like this:
//
//    ConstantGameFunctionWeights::Instance()->initialize(pathToFile);
//    ConstantGameFunctionWeights::Instance()->getKeys();
//    ConstantGameFunctionWeights::Instance()->getWeights(year);

class ConstantGameFunctionWeights {
public:
    static ConstantGameFunctionWeights* Instance();

    void initialize(std::string path);
    std::vector<int> getKeys();
    std::vector<double> getWeights(int year);

private:
    ConstantGameFunctionWeights(){};
    ConstantGameFunctionWeights(ConstantGameFunctionWeights const&){};
    ConstantGameFunctionWeights& operator=(ConstantGameFunctionWeights const&){};

    static ConstantGameFunctionWeights* uniqueInstance;

    std::unordered_map< int, std::unordered_map<std::string, double> *> weights;
};


#endif //NCAA_C_CONSTANTGAMEFUNCTIONWEIGHTS_H
