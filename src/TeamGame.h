//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_TEAMGAME_H
#define NCAA_C_TEAMGAME_H


#include "Game.h"
#include <boost/date_time/gregorian/gregorian.hpp>

class TeamGame : public Game {

public:
    //Constructor
    TeamGame(std::string _team, std::string _opp, int _game_no, boost::gregorian::date *_date, std::string _loc, int _win, int _pts, int _opp_pts,
             int _fgm, int _fga, int _threem, int _threea, int _ftm, int _fta,
             int _orb, int _trb, int _ast, int _stl, int _blk, int _tov, int _pf,
             int _opp_fgm, int _opp_fga, int _opp_threem, int _opp_threea, int _opp_ftm, int _opp_fta,
             int _opp_orb, int _opp_trb, int _opp_ast, int _opp_stl, int _opp_blk, int _opp_tov, int _opp_pf,
             float _spread);
    //Destructor
    //Copy Constructor
    //Copy Assignment

    //Getters
    int getGame_no() const {  return game_no;  }
    int getWin() const { return win; }
    float getPoss() const { return poss;  }
    float getSpread() const { return spread; }
    std::string getOpp_loc() const { return opp_loc; }
    Pct *getOor() const {  return oor;  }
    Pct *getOdr() const {  return odr;  }
    Pct *getOtr() const {  return otr;  }
    Pct *getOas() const {  return oas;  }
    Pct *getOst() const {  return ost;  }
    Pct *getObl() const {  return obl;  }
    Pct *getOto() const {  return oto;  }
    int getDpts() const {  return dpts; }
    Pct *getDfg() const {  return dfg;  }
    Pct *getDtwo() const { return dtwo; }
    Pct *getDthree() const {  return dthree;  }
    Pct *getDft() const {  return dft;  }
    Pct *getDor() const {  return dor;  }
    Pct *getDdr() const {  return ddr;  }
    Pct *getDtr() const {  return dtr;  }
    Pct *getDas() const {  return das;  }
    Pct *getDst() const {  return dst;  }
    Pct *getDbl() const {  return dbl;  }
    Pct *getDto() const {  return dto;  }
    Pct *getDefg() const { return defg; }
    Pct *getDftmr() const {  return dftmr;  }
    int getDpf() const {   return dpf;  }
    float getDposs() const { return dposs;  }

    //Other methods

private:
    int game_no;
    int win;
    float poss;
    float spread;
    std::string opp_loc;

    Pct *oor;
    Pct *odr;
    Pct *otr;
    Pct *oas;
    Pct *ost;
    Pct *obl;
    Pct *oto;

    int dpts;
    Pct *dfg;
    Pct *dtwo;
    Pct *dthree;
    Pct *dft;
    Pct *dor;
    Pct *ddr;
    Pct *dtr;
    Pct *das;
    Pct *dst;
    Pct *dbl;
    Pct *dto;
    Pct *defg;
    Pct *dftmr;
    int dpf;
    float dposs;
};


#endif //NCAA_C_TEAMGAME_H
