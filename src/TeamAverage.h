//
// Created by soulish on 11/23/15.
//

#ifndef NCAA_C_TEAMAVERAGE_H
#define NCAA_C_TEAMAVERAGE_H

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "Pct.h"
#include <math.h>

using namespace std;

class TeamAverage {
public:
    string teamname;
    string team;
    boost::gregorian::date *date;
    int num_games;
    float spread;

    //four factors***************
    Pct *oor;
    Pct *oefg;
    Pct *oftmr;
    Pct *oto;

    Pct *dor;
    Pct *defg;
    Pct *dftmr;
    Pct *dto;
    //****************************

    int opts;
    Pct *ofg;
    Pct *otwo;
    Pct *othree;
    Pct *oft;
    Pct *odr;
    Pct *otr;

    int dpts;
    Pct *dfg;
    Pct *dtwo;
    Pct *dthree;
    Pct *dft;
    Pct *ddr;
    Pct *dtr;

    TeamAverage(string _teamname, boost::gregorian::date *_date, int _pts, int twoa, float twop,
                int threea, float threep, int fta, float ftp, int ora, float orp, int dra, float drp,
                int toa, float top,
                int _dpts, int dtwoa, float dtwop,
                int dthreea, float dthreep, int dfta, float dftp, int dora, float dorp, int ddra, float ddrp,
                int dtoa, float dtop, int _num_games, float _spread);
};


#endif //NCAA_C_TEAMAVERAGE_H
