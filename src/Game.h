//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_GAME_H
#define NCAA_C_GAME_H

#include <string>
#include <vector>
#include "Pct.h"
#include <boost/date_time/gregorian/gregorian.hpp>

class Game {
public:
    //Constructor
    Game(std::string,std::string,boost::gregorian::date*,std::string,int,int,int,int,int,int,int,int);
    //Destructor
    //Copy Constructor
    //Copy Assignment

    //Getters
    const std::string getTeam() const { return team; }
    const std::string getOpp() const { return opp; }
    const std::string getLoc() const { return loc; }
    const boost::gregorian::date *getDate() const { return date; }
    const int getOpts() const { return opts; }
    const Pct *getOfg() const { return ofg; }
    const Pct *getOtwo() const {  return otwo;  }
    const Pct *getOthree() const { return othree;  }
    const Pct *getOft() const {  return oft; }
    const Pct *getOefg() const { return oefg; }
    const Pct *getOts() const { return ots; }
    const Pct *getOftmr() const { return oftmr;  }
    const int getOpf() const { return opf; }

    //Other Methods

protected:
    const std::string team;
    const std::string opp;
    const std::string loc;
    const boost::gregorian::date *date;
    const int opts;
    const Pct *ofg;
    const Pct *otwo;
    const Pct *othree;
    const Pct *oft;
    const Pct *oefg;
    const Pct *ots;
    const Pct *oftmr;
    const int opf;
};


#endif //NCAA_C_GAME_H
