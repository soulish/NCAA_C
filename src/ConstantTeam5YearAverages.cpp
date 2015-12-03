//
// Created by soulish on 12/2/15.
//

#include <stddef.h>
#include "ConstantTeam5YearAverages.h"
#include <fstream>

//calling static variable
ConstantTeam5YearAverages* ConstantTeam5YearAverages::uniqueInstance = NULL;

ConstantTeam5YearAverages *ConstantTeam5YearAverages::Instance() {
    if (!uniqueInstance){
        uniqueInstance = new ConstantTeam5YearAverages;
    }

    return uniqueInstance;
}

void ConstantTeam5YearAverages::initialize(std::string path) {
    int year;

    std::ifstream file(path);
    std::string value;
    while ( file.good() ) {
        getline(file, value, ',');
        if (value == "") break; //stops when reading last line
        year = stoi(value);
        team_5yr_averages.insert(std::pair<int, std::unordered_map<std::string, double> *>(year, new std::unordered_map<std::string, double>()));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("opts", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("opf", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("ofgm", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("ofga", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("ofgp", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("otwom", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("otwoa", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("otwop", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("othreem", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("othreea", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("othreep", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oftm", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("ofta", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oftp", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oorm", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oora", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oorp", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("odrm", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("odra", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("odrp", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("otrm", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("otra", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("otrp", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oasm", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oasa", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oasp", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("ostm", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("osta", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("ostp", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oblm", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("obla", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oblp", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("otom", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("otoa", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("otop", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oefgm", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oefga", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oefgp", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oftmrm", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oftmra", stod(value)));
        getline(file, value, ',');
        team_5yr_averages[year]->insert(std::pair<std::string, double>("oftmrp", stod(value)));
        getline(file, value, '\n');
    }
}

double ConstantTeam5YearAverages::get(int year, std::string stat) {
    //I should probably add something here to ensure that stat is a valid
    //member of the list of stats.
    return team_5yr_averages[year]->at(stat);
}
