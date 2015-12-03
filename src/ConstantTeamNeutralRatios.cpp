//
// Created by soulish on 12/2/15.
//

#include <stddef.h>
#include <fstream>
#include "ConstantTeamNeutralRatios.h"

ConstantTeamNeutralRatios* ConstantTeamNeutralRatios::uniqueInstance = NULL;

ConstantTeamNeutralRatios *ConstantTeamNeutralRatios::Instance() {
    if (!uniqueInstance){
        uniqueInstance = new ConstantTeamNeutralRatios;
    }
    return uniqueInstance;
}

void ConstantTeamNeutralRatios::initialize(std::string path) {
    int year;
    std::string loc;

    std::ifstream file(path);
    std::string value;
    while ( file.good() ) {
        getline(file, value, ',');
        if (value == "") break; //stops when reading last line
        year = stoi(value);
        getline(file, loc, ',');
        if (loc == "home") {
            //home is always the first one in a year so we make the new entry when we see it
            team_neutral_ratios.insert(std::pair< int, std::unordered_map<std::string, std::unordered_map<std::string, double > * > * >(year, new std::unordered_map<std::string, std::unordered_map<std::string, double> * >()));
        }
        team_neutral_ratios[year]->insert(std::pair<std::string, std::unordered_map<std::string, double> *>(loc, new std::unordered_map<std::string, double>()));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("opts", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("opf", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("ofgm", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("ofga", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("ofgp", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("otwom", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("otwoa", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("otwop", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("othreem", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("othreea", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("othreep", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oftm", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("ofta", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oftp", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oorm", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oora", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oorp", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("odrm", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("odra", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("odrp", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("otrm", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("otra", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("otrp", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oasm", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oasa", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oasp", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("ostm", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("osta", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("ostp", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oblm", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("obla", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oblp", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("otom", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("otoa", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("otop", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oefgm", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oefga", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oefgp", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oftmrm", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oftmra", stod(value)));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->insert(std::pair<std::string, double>("oftmrp", stod(value)));
        getline(file, value, '\n');
    }

}

double ConstantTeamNeutralRatios::get(int year, std::string loc, std::string stat) {
    //I should probably add something here to ensure that stat is a valid
    //member of the list of stats.
    return team_neutral_ratios[year]->at(loc)->at(stat);
}
