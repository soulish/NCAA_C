//
// Created by soulish on 12/4/15.
//

#ifndef NCAA_C_CONSTANTSEASONINFO_H
#define NCAA_C_CONSTANTSEASONINFO_H

#include <iostream>
#include <unordered_map>
#include <boost/date_time/gregorian/gregorian.hpp>

//Singleton class to hold the team 5 year averages
//
//Use it like this:
//
//    ConstantSeasonInfo::Instance()->initialize(pathToFile);
//    ConstantSeasonInfo::Instance()->get(2016,"home");

class ConstantSeasonInfo {
public:
    static ConstantSeasonInfo* Instance();

    void initialize(std::string path);
    const boost::gregorian::date &get(int,std::string);

private:
    ConstantSeasonInfo(){};
    ConstantSeasonInfo(ConstantSeasonInfo const&){};
    ConstantSeasonInfo& operator=(ConstantSeasonInfo const&){};

    static ConstantSeasonInfo* uniqueInstance;

    std::unordered_map<int, std::unordered_map<std::string, boost::gregorian::date > *> season_info;
};


#endif //NCAA_C_CONSTANTSEASONINFO_H
