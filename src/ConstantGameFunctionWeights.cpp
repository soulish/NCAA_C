//
// Created by soulish on 12/19/15.
//

#include <stddef.h>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "ConstantGameFunctionWeights.h"

ConstantGameFunctionWeights* ConstantGameFunctionWeights::uniqueInstance = NULL;

ConstantGameFunctionWeights *ConstantGameFunctionWeights::Instance() {
    if (!uniqueInstance)
        uniqueInstance = new ConstantGameFunctionWeights;
    return uniqueInstance;
}

void ConstantGameFunctionWeights::initialize(std::string path) {
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

std::vector<int> ConstantGameFunctionWeights::getKeys() {
    std::vector<int> keys;
    for (auto &w : weights)
        keys.push_back(w.first);

    return keys;
}

std::vector<double> ConstantGameFunctionWeights::getWeights(int year) {
    std::vector<double> values;
    values.push_back(weights[year]->at("oor.p"));
    values.push_back(weights[year]->at("oefg.p"));
    values.push_back(weights[year]->at("oftmr.p"));
    values.push_back(weights[year]->at("oto.p"));
    values.push_back(weights[year]->at("srs"));
    return values;
}
