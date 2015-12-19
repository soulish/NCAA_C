//
// Created by soulish on 12/12/15.
//

#ifndef NCAA_C_CONSTANTSTANDARDDEVIATIONS_H
#define NCAA_C_CONSTANTSTANDARDDEVIATIONS_H

#include <iostream>
#include <unordered_map>
#include <vector>

//Singleton class to hold the standard deviations of the WAverages by day
//
//Use it like this:
//
//    ConstantStandardDeviations::Instance()->initialize(pathToFile);
//    ConstantStandardDeviations::Instance()->get(2016,"home",112);

class ConstantStandardDeviations {
public:
    static ConstantStandardDeviations* Instance();

    void initialize(std::string path);
    double get(int year, std::string stat);

private:
    ConstantStandardDeviations(){};
    ConstantStandardDeviations(ConstantStandardDeviations const&){};
    ConstantStandardDeviations& operator=(ConstantStandardDeviations const&){};

    static ConstantStandardDeviations* uniqueInstance;

    std::unordered_map< int, std::unordered_map<std::string, double> *> standardDeviations;
};


#endif //NCAA_C_CONSTANTSTANDARDDEVIATIONS_H
