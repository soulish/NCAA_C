//
// Created by soulish on 12/2/15.
//

#include <stddef.h>
#include "ConstantTeamWinRatios.h"
#include <fstream>

ConstantTeamWinRatios* ConstantTeamWinRatios::uniqueInstance = NULL;

ConstantTeamWinRatios *ConstantTeamWinRatios::Instance() {
    if (!uniqueInstance){
        uniqueInstance = new ConstantTeamWinRatios;
    }

    return uniqueInstance;
}

void ConstantTeamWinRatios::initialize(std::string path) {
    int year;

    std::ifstream file(path);
    std::string value;
    while ( file.good() ) {
        getline(file, value, ',');
        if (value == "") break; //stops when reading last line
        year = stoi(value);
        team_win_ratios.emplace(year, new std::unordered_map<std::string, double>());
        getline(file, value, ',');
        team_win_ratios[year]->emplace("home", stod(value));
        getline(file, value, ',');
        team_win_ratios[year]->emplace("away", stod(value));
        getline(file, value, ',');
        team_win_ratios[year]->emplace("neutral", stod(value));
        getline(file, value, '\n');
    }
}

double ConstantTeamWinRatios::get(int year, std::string loc) {
    return team_win_ratios[year]->at(loc);
}

