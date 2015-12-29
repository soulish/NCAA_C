//
// Created by soulish on 12/4/15.
//

#include <stddef.h>
#include "ConstantSeasonInfo.h"
#include <fstream>
#include <boost/algorithm/string.hpp>

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

const boost::gregorian::date ConstantSeasonInfo::get(int year, std::string dateDesired) {
    if (dateDesired == "season start" || dateDesired == "season end" ||
        dateDesired == "tournament start" || dateDesired == "tournament end")
        return season_info[year]->at(dateDesired);
    else{
        std::vector<std::string> splitVec;
        boost::split(splitVec, dateDesired, boost::is_any_of("-"));
        if (splitVec.size() == 0 || splitVec.size() == 1)
            return season_info[year]->at("tournament start");
        else if (splitVec.size() == 2) {
            std::string dateDesiredString;
            if (atoi(splitVec[0].c_str()) >= 11)
                dateDesiredString = std::to_string(year - 1) + "-" + dateDesired;
            else
                dateDesiredString = std::to_string(year) + "-" + dateDesired;
            return boost::gregorian::date(boost::gregorian::from_string(dateDesiredString));
        }
        else if (splitVec.size() == 3){
            return boost::gregorian::date(boost::gregorian::from_string(dateDesired));
        }
    }
}
