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
    TeamAverage(std::string _teamname, boost::gregorian::date _date, int _pts, int twoa, double twop,
                int threea, double threep, int fta, double ftp, int ora, double orp, int dra, double drp,
                int toa, double top,
                int _dpts, int dtwoa, double dtwop,
                int dthreea, double dthreep, int dfta, double dftp, int dora, double dorp, int ddra, double ddrp,
                int dtoa, double dtop, int _num_games);
    //Destructor
    virtual ~TeamAverage(){
        //delete pointers
        delete oor;
        delete oefg;
        delete oftmr;
        delete oto;

        delete dor;
        delete defg;
        delete dftmr;
        delete dto;

        delete oppp;
        delete ofg;
        delete otwo;
        delete othree;
        delete oft;
        delete odr;
        delete otr;

        delete dppp;
        delete dfg;
        delete dtwo;
        delete dthree;
        delete dft;
        delete ddr;
        delete dtr;
    }
    //Copy Constructor
    //Copy Assignment

    //Getters
    const std::string getTeamname() const {  return teamname;  }
    const std::string getTeam() const {      return teamname;  }
    const boost::gregorian::date &getDate() const {  return date;  }
    const int getNum_games() const {  return num_games;  }
    const double getOposs() const {  return oposs;  }
    const double getDposs() const {  return dposs;  }
    const Pct *getOor() const {   return oor;  }
    const Pct *getOefg() const {  return oefg; }
    const Pct *getOftmr() const { return oftmr;}
    const Pct *getOto() const {   return oto;  }
    const Pct *getDor() const {   return dor;  }
    const Pct *getDefg() const {  return defg; }
    const Pct *getDftmr() const { return dftmr;}
    const Pct *getDto() const {   return dto;  }
    const int getOpts() const {   return opts; }
    const Pct *getOppp() const {  return oppp;  }
    const Pct *getOfg() const {   return ofg;  }
    const Pct *getOtwo() const {  return otwo; }
    const Pct *getOthree() const {return othree;}
    const Pct *getOft() const {   return oft;  }
    const Pct *getOdr() const {   return odr;  }
    const Pct *getOtr() const {   return otr;  }
    const int getDpts() const {   return dpts; }
    const Pct *getDppp() const {  return dppp;  }
    const Pct *getDfg() const {   return dfg;  }
    const Pct *getDtwo() const {  return dtwo; }
    const Pct *getDthree() const {return dthree;}
    const Pct *getDft() const {   return dft;  }
    const Pct *getDdr() const {   return ddr;  }
    const Pct *getDtr() const {   return dtr;  }

    const Pct *getPct(std::string which) const;
    const double getValue(std::string which) const;

    //Other methods

protected:
    const std::string teamname;
    const boost::gregorian::date date;
    const int num_games;
    const double oposs;
    const double dposs;

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
    const Pct *oppp;
    const Pct *ofg;
    const Pct *otwo;
    const Pct *othree;
    const Pct *oft;
    const Pct *odr;
    const Pct *otr;

    const int dpts;
    const Pct *dppp;
    const Pct *dfg;
    const Pct *dtwo;
    const Pct *dthree;
    const Pct *dft;
    const Pct *ddr;
    const Pct *dtr;
};


#endif //NCAA_C_TEAMAVERAGE_H
