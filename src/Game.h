//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_GAME_H
#define NCAA_C_GAME_H

#include <string>
#include <vector>
#include "Pct.h"
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;

class Game {
public://probably will go to protected later
    string team;
    string opp;
    string loc;
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


    Game(string,string,boost::gregorian::date*,string,int,int,int,int,int,int,int,int);
};


#endif //NCAA_C_GAME_H
