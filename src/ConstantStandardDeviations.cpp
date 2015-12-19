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
        standardDeviations.emplace(year, new std::unordered_map<std::string, double>());
        getline(file, value, ',');
        standardDeviations[year]->emplace("oor.p", stod(value));
        getline(file, value, ',');
        standardDeviations[year]->emplace("oefg.p", stod(value));
        getline(file, value, ',');
        standardDeviations[year]->emplace("oftmr.p", stod(value));
        getline(file, value, ',');
        standardDeviations[year]->emplace("oto.p", stod(value));
        getline(file, value, '\n');
        standardDeviations[year]->emplace("srs", stod(value));
    }
}

double ConstantStandardDeviations::get(int year, std::string stat) {
    return standardDeviations[year]->at(stat);
}
