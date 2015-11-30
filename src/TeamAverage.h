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
    const std::string getTeamname() const {  return teamname;  }
    const std::string getTeam() const {      return teamname;  }
    const boost::gregorian::date *getDate() const {  return date;  }
    const int getNum_games() const {  return num_games;  }
    const float getSpread() const {   return spread;     }
    const Pct *getOor() const {   return oor;  }
    const Pct *getOefg() const {  return oefg; }
    const Pct *getOftmr() const { return oftmr;}
    const Pct *getOto() const {   return oto;  }
    const Pct *getDor() const {   return dor;  }
    const Pct *getDefg() const {  return defg; }
    const Pct *getDftmr() const { return dftmr;}
    const Pct *getDto() const {   return dto;  }
    const int getOpts() const {   return opts; }
    const Pct *getOfg() const {   return ofg;  }
    const Pct *getOtwo() const {  return otwo; }
    const Pct *getOthree() const {return othree;}
    const Pct *getOft() const {   return oft;  }
    const Pct *getOdr() const {   return odr;  }
    const Pct *getOtr() const {   return otr;  }
    const int getDpts() const {   return dpts; }
    const Pct *getDfg() const {   return dfg;  }
    const Pct *getDtwo() const {  return dtwo; }
    const Pct *getDthree() const {return dthree;}
    const Pct *getDft() const {   return dft;  }
    const Pct *getDdr() const {   return ddr;  }
    const Pct *getDtr() const {   return dtr;  }

    //Other methods

protected:
    const std::string teamname;
    const boost::gregorian::date *date;
    const int num_games;
    const float spread;

    //four factors***************
    const Pct *oor;
    const Pct *oefg;
    const Pct *oftmr;
    const Pct *oto;

    const Pct *dor;
    const Pct *defg;
    const Pct *dftmr;
    const Pct *dto;
    //****************************

    const int opts;
    const Pct *ofg;
    const Pct *otwo;
    const Pct *othree;
    const Pct *oft;
    const Pct *odr;
    const Pct *otr;

    const int dpts;
    const Pct *dfg;
    const Pct *dtwo;
    const Pct *dthree;
    const Pct *dft;
    const Pct *ddr;
    const Pct *dtr;
};


#endif //NCAA_C_TEAMAVERAGE_H
