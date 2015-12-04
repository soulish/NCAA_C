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
        team_5yr_averages.emplace(year, new std::unordered_map<std::string, double>());
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("opts.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("opf.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("ofg.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("ofg.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("ofg.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("otwo.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("otwo.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("otwo.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("othree.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("othree.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("othree.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oft.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oft.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oft.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oor.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oor.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oor.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("odr.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("odr.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("odr.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("otr.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("otr.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("otr.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oas.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oas.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oas.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("ost.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("ost.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("ost.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("obl.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("obl.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("obl.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oto.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oto.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oto.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oefg.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oefg.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oefg.p", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oftmr.m", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oftmr.a", stod(value));
        getline(file, value, ',');
        team_5yr_averages[year]->emplace("oftmr.p", stod(value));
        getline(file, value, '\n');
    }
}

double ConstantTeam5YearAverages::get(int year, std::string stat) {
    //I should probably add something here to ensure that stat is a valid
    //member of the list of stats.
    return team_5yr_averages[year]->at(stat);
}
