//
// Created by soulish on 12/12/15.
//

#include <stddef.h>
#include <fstream>
#include "ConstantWAverageFunctions.h"
#include "ConstantTeam5YearAverages.h"

ConstantWAverageFunctions *ConstantWAverageFunctions::uniqueInstance = NULL;

ConstantWAverageFunctions *ConstantWAverageFunctions::Instance() {
    if (!uniqueInstance){
        uniqueInstance = new ConstantWAverageFunctions;
    }
    return uniqueInstance;
}

void ConstantWAverageFunctions::initialize(std::string path) {
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

std::unordered_map<std::string, double> ConstantWAverageFunctions::predictStats(TeamWAverage *w1, TeamWAverage *w2,
                                                                                int year) {
    std::unordered_map<std::string, double> result;

    std::string stats[] = {"or.p", "efg.p", "ftmr.p", "to.p"};

    for (std::string &s : stats){
        double factor;
        factor = functions[year]->at("o" + s)->Eval(w1->getValue("o" + s) - w2->getValue("d" + s));
        result.emplace("o"+s, factor);
    }

    return result;
}
//this function predicts assuming the opponent is completely average, thus we use
//the 5 year averages as the opponent's stats.  We can choose whether to calculate
//how w1 will do on offense or on defense with the offOrDef option, which defaults to offense
std::unordered_map<std::string, double> ConstantWAverageFunctions::predictStats(TeamWAverage *w1, int year,
                                                                                std::string offOrDef) {
    std::unordered_map<std::string, double> result;

    std::string stats[] = {"or.p", "efg.p", "ftmr.p", "to.p"};

    for (std::string &s : stats){
        double factor;
        if (offOrDef == "offense") {
            factor = functions[year]->at("o" + s)->Eval(w1->getValue("o" + s) -
                                                        ConstantTeam5YearAverages::Instance()->get(year, "o" + s));
            result.emplace("o" + s, factor);
        }
        else if (offOrDef == "defense"){
            factor = functions[year]->at("o" + s)->Eval(ConstantTeam5YearAverages::Instance()->get(year, "o" + s) -
                                                        w1->getValue("d" + s));
            result.emplace("d" + s, factor);
        }
    }

    return result;
}

TF1 *ConstantWAverageFunctions::getFunction(int year, std::string stat) {
    return functions[year]->at(stat);
}
