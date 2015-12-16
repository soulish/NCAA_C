//
// Created by soulish on 12/15/15.
//

#ifndef NCAA_C_CONSTANTSRSADDITIONS_H
#define NCAA_C_CONSTANTSRSADDITIONS_H

#include <iostream>
#include <unordered_map>

//Singleton class to hold the SRS additions
//
//Use it like this:
//
//    ConstantSRSadditions::Instance()->initialize(pathToFile);
//    ConstantSRSadditions::Instance()->get(2016,"home");

class ConstantSRSadditions {
public:
    static ConstantSRSadditions* Instance();

    void initialize(std::string path);
    const double get(int year, std::string loc);
private:
    ConstantSRSadditions(){};
    ConstantSRSadditions(ConstantSRSadditions const&){};
    ConstantSRSadditions& operator=(ConstantSRSadditions const&){};

    static ConstantSRSadditions* uniqueInstance;

    std::unordered_map<int,double> srsAddition;
};


#endif //NCAA_C_CONSTANTSRSADDITIONS_H
