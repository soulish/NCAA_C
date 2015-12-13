//
// Created by soulish on 12/12/15.
//

#include <stddef.h>
#include <fstream>
#include "ConstantFunctions.h"

ConstantFunctions* ConstantFunctions::uniqueInstance = NULL;

ConstantFunctions *ConstantFunctions::Instance() {
    if (!uniqueInstance){
        uniqueInstance = new ConstantFunctions;
    }
    return uniqueInstance;
}

void ConstantFunctions::initialize(std::string path) {
    int year;
    std::string stat;

    std::ifstream file(path);
    std::string value;
    char name[256];
    while (file.good()){
        getline(file, value, ',');
        if (value == "") break; //stops when reading last line
        year = stoi(value);
        functions.emplace(year, new std::unordered_map<std::string, TF1*>());

        //oor.p
        getline(file, stat, ',');
        sprintf(name, "fn_%s",stat.c_str());
        functions[year]->emplace(stat, new TF1(name, "pol1"));
        getline(file, value, ',');
        functions[year]->at(stat)->SetParameter(0,stod(value));
        getline(file, value, ',');
        functions[year]->at(stat)->SetParameter(1,stod(value));

        //oefg.p
        getline(file, stat, ',');
        sprintf(name, "fn_%s",stat.c_str());
        functions[year]->emplace(stat, new TF1(name, "pol1"));
        getline(file, value, ',');
        functions[year]->at(stat)->SetParameter(0,stod(value));
        getline(file, value, ',');
        functions[year]->at(stat)->SetParameter(1,stod(value));

        //oftmr.p
        getline(file, stat, ',');
        sprintf(name, "fn_%s",stat.c_str());
        functions[year]->emplace(stat, new TF1(name, "pol1"));
        getline(file, value, ',');
        functions[year]->at(stat)->SetParameter(0,stod(value));
        getline(file, value, ',');
        functions[year]->at(stat)->SetParameter(1,stod(value));

        //oto.p
        getline(file, stat, ',');
        sprintf(name, "fn_%s",stat.c_str());
        functions[year]->emplace(stat, new TF1(name, "pol1"));
        getline(file, value, ',');
        functions[year]->at(stat)->SetParameter(0,stod(value));
        getline(file, value, '\n');
        functions[year]->at(stat)->SetParameter(1,stod(value));
    }
}

std::unordered_map<std::string, double> ConstantFunctions::predict(TeamWAverage *w1, TeamWAverage *w2, int year) {
    std::unordered_map<std::string, double> result;

    std::string stats[] = {"or.p", "efg.p", "ftmr.p", "to.p"};

    for (std::string &s : stats){
        double factor;
        factor = functions[year]->at("o" + s)->Eval(w1->getValue("o" + s) - w2->getValue("d" + s));
        result.emplace("o"+s, factor * w1->getValue("o" + s));
    }

    return result;
}
