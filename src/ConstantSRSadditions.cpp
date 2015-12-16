//
// Created by soulish on 12/15/15.
//
#include <stddef.h>
#include <fstream>
#include "ConstantSRSadditions.h"

ConstantSRSadditions* ConstantSRSadditions::uniqueInstance = NULL;

ConstantSRSadditions *ConstantSRSadditions::Instance() {
    if (!uniqueInstance)
        uniqueInstance = new ConstantSRSadditions;
    return uniqueInstance;
}

void ConstantSRSadditions::initialize(std::string path) {
    int year;
    std::ifstream file(path);
    std::string value;
    while (file.good()) {
        getline(file, value, ',');
        if (value == "") break; //stops when reading last line
        year = stoi(value);
        getline(file, value, '\n');
        srsAddition.emplace(year, stod(value));
    }
}

const double ConstantSRSadditions::get(int year, std::string loc) {
    double result = 0;
    if (loc == "home") result = srsAddition[year];
    else if (loc == "away") result = -1 * srsAddition[year];
    return result;
}
