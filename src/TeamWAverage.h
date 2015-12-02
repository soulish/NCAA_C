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
    TeamWAverage(std::string _teamname, boost::gregorian::date *_date, int _pts, int twoa, double twop,
                 int threea, double threep, int fta, double ftp, int ora, double orp, int dra, double drp,
                 int toa, double top,
                 int _dpts, int dtwoa, double dtwop,
                 int dthreea, double dthreep, int dfta, double dftp, int dora, double dorp, int ddra, double ddrp,
                 int dtoa, double dtop, double _rpi, double _srs, double _sos, double _pure_wp, double _alt_wp,
                 int _num_games, double _spread);
    //Destructor
    virtual ~TeamWAverage(){ }
    //Copy Constructor
    //Copy Assignment

    //Getters
    const double getRpi() const {  return rpi;  }
    const double getSrs() const {  return srs;  }
    const double getSos() const {  return sos;  }
    const double getPure_wp() const {return pure_wp;}
    const double getAlt_wp() const { return alt_wp; }

    //Other methods

private:
    const double rpi;
    const double srs;
    const double sos;
    const double pure_wp;
    const double alt_wp;
};


#endif //NCAA_C_TEAMWAVERAGE_H
