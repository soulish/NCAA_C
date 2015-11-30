//
// Created by soulish on 11/23/15.
//

#ifndef NCAA_C_TEAMWAVERAGE_H
#define NCAA_C_TEAMWAVERAGE_H

#include <string>
#include <boost/date_time/gregorian/greg_date.hpp>
#include "TeamAverage.h"

class TeamWAverage : public TeamAverage {
public:
    //Constructor
    TeamWAverage(std::string _teamname, boost::gregorian::date *_date, int _pts, int twoa, float twop,
                 int threea, float threep, int fta, float ftp, int ora, float orp, int dra, float drp,
                 int toa, float top,
                 int _dpts, int dtwoa, float dtwop,
                 int dthreea, float dthreep, int dfta, float dftp, int dora, float dorp, int ddra, float ddrp,
                 int dtoa, float dtop, float _rpi, float _srs, float _sos, float _pure_wp, float _alt_wp,
                 int _num_games, float _spread);
    //Destructor
    //Copy Constructor
    //Copy Assignment

    //Getters
    float getRpi() const {  return rpi;  }
    float getSrs() const {  return srs;  }
    float getSos() const {  return sos;  }
    float getPure_wp() const {return pure_wp;}
    float getAlt_wp() const { return alt_wp; }

    //Other methods

private:
    float rpi;
    float srs;
    float sos;
    float pure_wp;
    float alt_wp;
};


#endif //NCAA_C_TEAMWAVERAGE_H
