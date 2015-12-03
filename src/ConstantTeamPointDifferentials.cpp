//
// Created by soulish on 12/2/15.
//

#include <stddef.h>
#include "ConstantTeamPointDifferentials.h"
#include <fstream>

ConstantTeamPointDifferentials* ConstantTeamPointDifferentials::uniqueInstance = NULL;

ConstantTeamPointDifferentials *ConstantTeamPointDifferentials::Instance() {
    if (!uniqueInstance){
        uniqueInstance = new ConstantTeamPointDifferentials;
    }

    return uniqueInstance;
}

void ConstantTeamPointDifferentials::initialize(std::string path) {
    int year;

    std::ifstream file(path);
    std::string value;
    while ( file.good() ) {
        getline(file, value, ',');
        if (value == "") break; //stops when reading last line
        year = stoi(value);
        team_point_differentials.insert(std::pair<int, std::unordered_map<std::string, double> *>(year, new std::unordered_map<std::string, double>()));
        getline(file, value, ',');
        team_point_differentials[year]->insert(std::pair<std::string, double>("home", stod(value)));
        getline(file, value, ',');
        team_point_differentials[year]->insert(std::pair<std::string, double>("away", stod(value)));
        getline(file, value, ',');
        team_point_differentials[year]->insert(std::pair<std::string, double>("neutral", stod(value)));
        getline(file, value, '\n');
    }
}

double ConstantTeamPointDifferentials::get(int year, std::string loc) {
    return team_point_differentials[year]->at(loc);
}
