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
    std::string getTeam() const { return team; }
    std::string getOpp() const { return opp; }
    std::string getLoc() const { return loc; }
    boost::gregorian::date *getDate() const { return date; }
    int getOpts() const { return opts; }
    Pct *getOfg() const { return ofg; }
    Pct *getOtwo() const {  return otwo;  }
    Pct *getOthree() const { return othree;  }
    Pct *getOft() const {  return oft; }
    Pct *getOefg() const { return oefg; }
    Pct *getOts() const { return ots; }
    Pct *getOftmr() const { return oftmr;  }
    int getOpf() const { return opf; }

    //Other Methods

protected:
    std::string team;
    std::string opp;
    std::string loc;
    boost::gregorian::date *date;
    int opts;
    Pct *ofg;
    Pct *otwo;
    Pct *othree;
    Pct *oft;
    Pct *oefg;
    Pct *ots;
    Pct *oftmr;
    int opf;
};


#endif //NCAA_C_GAME_H
