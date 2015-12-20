//
// Created by soulish on 12/19/15.
//

#include <stddef.h>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "ConstantGameFunction.h"
#include "ConstantWAverageFunctions.h"
#include "ConstantTeamNeutralRatios.h"
#include "ConstantStandardDeviations.h"
#include "ConstantSRSadditions.h"

ConstantGameFunction *ConstantGameFunction::uniqueInstance = NULL;

ConstantGameFunction *ConstantGameFunction::Instance() {
    if (!uniqueInstance)
        uniqueInstance = new ConstantGameFunction;
    return uniqueInstance;
}

void ConstantGameFunction::initialize(std::string path) {
    int year;
    std::string stat;

    std::ifstream file(path);
    std::string value;
    while (file.good()){
        getline(file, value, ',');
        if (value == "") break; //stops when reading last line
        year = stoi(value);
        weights.emplace(year, new std::unordered_map<std::string, double>());
        getline(file, value, ',');
        weights[year]->emplace("oor.p", stod(value));
        getline(file, value, ',');
        weights[year]->emplace("oefg.p", stod(value));
        getline(file, value, ',');
        weights[year]->emplace("oftmr.p", stod(value));
        getline(file, value, ',');
        weights[year]->emplace("oto.p", stod(value));
        getline(file, value, ',');
        weights[year]->emplace("srs", stod(value));
        getline(file, value, '\n');
        weights[year]->emplace("fcn_min", stod(value));
    }
}

std::vector<int> ConstantGameFunction::getKeys() {
    std::vector<int> keys;
    for (auto &w : weights)
        keys.push_back(w.first);

    return keys;
}

std::vector<double> ConstantGameFunction::getWeights(int year) {
    std::vector<double> values;
    values.push_back(weights[year]->at("oor.p"));
    values.push_back(weights[year]->at("oefg.p"));
    values.push_back(weights[year]->at("oftmr.p"));
    values.push_back(weights[year]->at("oto.p"));
    values.push_back(weights[year]->at("srs"));
    return values;
}

double ConstantGameFunction::predictGame(TeamWAverage *wa1, TeamWAverage *wa2, int year,
                                         std::string loc, std::string oppLoc) {
    ConstantWAverageFunctions *functions = ConstantWAverageFunctions::Instance();
    ConstantTeamNeutralRatios *ratios = ConstantTeamNeutralRatios::Instance();
    ConstantStandardDeviations *stdDevs = ConstantStandardDeviations::Instance();
    ConstantSRSadditions *additions = ConstantSRSadditions::Instance();

    std::unordered_map<std::string, double> predictions1 = functions->predictStats(wa1, wa2, year);
    std::unordered_map<std::string, double> predictions2 = functions->predictStats(wa2, wa1, year);

    double oor = (predictions1["oor.p"] * wa1->getValue("oor.p") / ratios->get(year,loc,"oor.p") -
                  predictions2["oor.p"] * wa2->getValue("oor.p") / ratios->get(year,oppLoc,"oor.p")) /
                 stdDevs->get(year,"oor.p");

    double oefg = (predictions1["oefg.p"] * wa1->getValue("oefg.p") / ratios->get(year,loc,"oefg.p") -
                   predictions2["oefg.p"] * wa2->getValue("oefg.p") / ratios->get(year,oppLoc,"oefg.p")) /
                  stdDevs->get(year,"oefg.p");

    double oftmr = (predictions1["oftmr.p"] * wa1->getValue("oftmr.p") / ratios->get(year,loc,"oftmr.p") -
                    predictions2["oftmr.p"] * wa2->getValue("oftmr.p") / ratios->get(year,oppLoc,"oftmr.p")) /
                   stdDevs->get(year,"oftmr.p");

    //This guy is reversed because turnovers are bad.
    double oto = (-predictions1["oto.p"] * wa1->getValue("oto.p") / ratios->get(year,loc,"oto.p") +
                  predictions2["oto.p"] * wa2->getValue("oto.p") / ratios->get(year,oppLoc,"oto.p")) /
                 stdDevs->get(year,"oto.p");

    double srs = (wa1->getSrs() - wa2->getSrs() + additions->get(year,loc)) /
                 stdDevs->get(year,"srs");

    std::unordered_map<std::string, double> *theWeights = weights[year];

    double sum = theWeights->at("oor.p") * oor +
                 theWeights->at("oefg.p") * oefg +
                 theWeights->at("oftmr.p") * oftmr +
                 theWeights->at("oto.p") * oto +
                 theWeights->at("srs") * srs;

    return sum;
}
