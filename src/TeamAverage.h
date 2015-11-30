//
// Created by soulish on 11/23/15.
//

#ifndef NCAA_C_TEAMAVERAGE_H
#define NCAA_C_TEAMAVERAGE_H

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "Pct.h"
#include <math.h>

class TeamAverage {
public:
    //Constructor
    TeamAverage(std::string _teamname, boost::gregorian::date *_date, int _pts, int twoa, float twop,
                int threea, float threep, int fta, float ftp, int ora, float orp, int dra, float drp,
                int toa, float top,
                int _dpts, int dtwoa, float dtwop,
                int dthreea, float dthreep, int dfta, float dftp, int dora, float dorp, int ddra, float ddrp,
                int dtoa, float dtop, int _num_games, float _spread);
    //Destructor
    //Copy Constructor
    //Copy Assignment

    //Getters
    std::string getTeamname() const {  return teamname;  }
    std::string getTeam() const {      return teamname;  }
    boost::gregorian::date *getDate() const {  return date;  }
    int getNum_games() const {  return num_games;  }
    float getSpread() const {   return spread;     }
    Pct *getOor() const {   return oor;  }
    Pct *getOefg() const {  return oefg; }
    Pct *getOftmr() const { return oftmr;}
    Pct *getOto() const {   return oto;  }
    Pct *getDor() const {   return dor;  }
    Pct *getDefg() const {  return defg; }
    Pct *getDftmr() const { return dftmr;}
    Pct *getDto() const {   return dto;  }
    int getOpts() const {   return opts; }
    Pct *getOfg() const {   return ofg;  }
    Pct *getOtwo() const {  return otwo; }
    Pct *getOthree() const {return othree;}
    Pct *getOft() const {   return oft;  }
    Pct *getOdr() const {   return odr;  }
    Pct *getOtr() const {   return otr;  }
    int getDpts() const {   return dpts; }
    Pct *getDfg() const {   return dfg;  }
    Pct *getDtwo() const {  return dtwo; }
    Pct *getDthree() const {return dthree;}
    Pct *getDft() const {   return dft;  }
    Pct *getDdr() const {   return ddr;  }
    Pct *getDtr() const {   return dtr;  }

    //Other methods

protected:
    std::string teamname;
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
};


#endif //NCAA_C_TEAMAVERAGE_H
