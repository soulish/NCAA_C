//
// Created by soulish on 12/4/15.
//

#include <stddef.h>
#include "ConstantSeasonInfo.h"
#include <fstream>

ConstantSeasonInfo* ConstantSeasonInfo::uniqueInstance = NULL;

ConstantSeasonInfo *ConstantSeasonInfo::Instance() {
    if (!uniqueInstance)
        uniqueInstance = new ConstantSeasonInfo;
    return uniqueInstance;
}

void ConstantSeasonInfo::initialize(std::string path) {
    int year;
    unsigned short y, m, d;

    std::ifstream file(path);
    std::string value;
    while (file.good()){
        getline(file, value, ',');
        if (value == "") break; //stops when reading last line
        year = stoi(value);
        season_info.emplace(year, new std::unordered_map<std::string, boost::gregorian::date>());
        getline(file, value, ',');
        y = (unsigned short)stoi(value);
        getline(file, value, ',');
        m = (unsigned short)stoi(value);
        getline(file, value, ',');
        d = (unsigned short)stoi(value);
        season_info[year]->emplace("tournament start",boost::gregorian::date(y,m,d));
        getline(file, value, ',');
        y = (unsigned short)stoi(value);
        getline(file, value, ',');
        m = (unsigned short)stoi(value);
        getline(file, value, ',');
        d = (unsigned short)stoi(value);
        season_info[year]->emplace("tournament end",boost::gregorian::date(y,m,d));
        getline(file, value, ',');
        y = (unsigned short)stoi(value);
        getline(file, value, ',');
        m = (unsigned short)stoi(value);
        getline(file, value, ',');
        d = (unsigned short)stoi(value);
        season_info[year]->emplace("season end",boost::gregorian::date(y,m,d));
        getline(file, value, ',');
        y = (unsigned short)stoi(value);
        getline(file, value, ',');
        m = (unsigned short)stoi(value);
        getline(file, value, '\n');
        d = (unsigned short)stoi(value);
        season_info[year]->emplace("season start",boost::gregorian::date(y,m,d));
    }
}

const boost::gregorian::date &ConstantSeasonInfo::get(int year, std::string dateDesired) {
    return season_info[year]->at(dateDesired);
}
