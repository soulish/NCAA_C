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
            team_neutral_ratios.emplace(year, new std::unordered_map<std::string, std::unordered_map<std::string, double> * >());
        }
        team_neutral_ratios[year]->emplace(loc, new std::unordered_map<std::string, double>());
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("opts.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("opf.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("ofg.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("ofg.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("ofg.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("otwo.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("otwo.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("otwo.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("othree.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("othree.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("othree.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oft.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oft.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oft.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oor.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oor.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oor.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("odr.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("odr.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("odr.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("otr.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("otr.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("otr.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oas.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oas.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oas.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("ost.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("ost.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("ost.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("obl.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("obl.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("obl.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oto.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oto.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oto.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oefg.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oefg.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oefg.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oftmr.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oftmr.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oftmr.p", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oppp.m", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oppp.a", stod(value));
        getline(file, value, ',');
        team_neutral_ratios[year]->at(loc)->emplace("oppp.p", stod(value));
        getline(file, value, '\n');
    }

}

double ConstantTeamNeutralRatios::get(int year, std::string loc, std::string stat) {
    //I should probably add something here to ensure that stat is a valid
    //member of the list of stats.
    return team_neutral_ratios[year]->at(loc)->at(stat);
}
