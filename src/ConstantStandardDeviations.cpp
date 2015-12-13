//
// Created by soulish on 12/12/15.
//

#include <stddef.h>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "ConstantStandardDeviations.h"

ConstantStandardDeviations* ConstantStandardDeviations::uniqueInstance = NULL;

ConstantStandardDeviations *ConstantStandardDeviations::Instance() {
    if (!uniqueInstance){
        uniqueInstance = new ConstantStandardDeviations;
    }
    return uniqueInstance;
}

void ConstantStandardDeviations::initialize(std::string path) {
    int year;
    std::string stat;

    std::ifstream file(path);
    std::string value;
    while (file.good()){
        getline(file, value, ',');
        if (value == "") break; //stops when reading last line
        year = stoi(value);
        getline(file, stat, ',');
        if (stat == "oor"){
            //oor will always be the first entry in the year, so we know to insert the year
            //when we see it.
            standardDeviations.emplace(year, new std::unordered_map<std::string, std::vector<double>*>());
        }
        standardDeviations[year]->emplace(stat, new std::vector<double>());

        getline(file, value, '\n');
        std::vector<std::string> splitString;
        boost::split(splitString, value, boost::is_any_of(","));
        for (int i = 0; i < splitString.size(); i++)
            standardDeviations[year]->at(stat)->push_back(stod(splitString[i]));
    }
}

double ConstantStandardDeviations::get(int year, std::string stat, int day) {
    //if the day is negative, we treat it as if it is on the first day,
    //if day is beyond start of tournament (i.e. the end of the vector),
    //we treat it as if it is on the last day
    day = day < 0 ? 0 : day;
    day = day >= standardDeviations[year]->at(stat)->size() ?
          standardDeviations[year]->at(stat)->size() - 1 : day;

    return standardDeviations[year]->at(stat)->at(day);
}
