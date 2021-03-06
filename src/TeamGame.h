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
    TeamGame(std::string _team, std::string _opp, int _game_no, boost::gregorian::date _date, std::string _loc, int _win, int _pts, int _opp_pts,
             int _fgm, int _fga, int _threem, int _threea, int _ftm, int _fta,
             int _orb, int _trb, int _ast, int _stl, int _blk, int _tov, int _pf,
             int _opp_fgm, int _opp_fga, int _opp_threem, int _opp_threea, int _opp_ftm, int _opp_fta,
             int _opp_orb, int _opp_trb, int _opp_ast, int _opp_stl, int _opp_blk, int _opp_tov, int _opp_pf,
             double _spread);
    //Destructor
    virtual ~TeamGame() {
        //delete pointers
        delete oor;
        delete odr;
        delete otr;
        delete oas;
        delete ost;
        delete obl;
        delete oto;
        delete oppp;

        delete dfg;
        delete dtwo;
        delete dthree;
        delete dft;
        delete dor;
        delete ddr;
        delete dtr;
        delete das;
        delete dst;
        delete dbl;
        delete dto;
        delete defg;
        delete dftmr;
        delete dppp;
    }
    //Copy Constructor
    //Copy Assignment


    //Getters
    const int getGame_no() const {  return game_no;  }
    const int getWin() const { return win; }
    const double getOposs() const { return oposs;  }
    const double getSpread() const { return spread; }
    std::string getOppLoc() const { return opp_loc; }
    const Pct *getOor() const {  return oor;  }
    const Pct *getOdr() const {  return odr;  }
    const Pct *getOtr() const {  return otr;  }
    const Pct *getOas() const {  return oas;  }
    const Pct *getOst() const {  return ost;  }
    const Pct *getObl() const {  return obl;  }
    const Pct *getOto() const {  return oto;  }
    const int getDpts() const {  return dpts; }
    const Pct *getDfg() const {  return dfg;  }
    const Pct *getDtwo() const { return dtwo; }
    const Pct *getDthree() const {  return dthree;  }
    const Pct *getDft() const {  return dft;  }
    const Pct *getDor() const {  return dor;  }
    const Pct *getDdr() const {  return ddr;  }
    const Pct *getDtr() const {  return dtr;  }
    const Pct *getDas() const {  return das;  }
    const Pct *getDst() const {  return dst;  }
    const Pct *getDbl() const {  return dbl;  }
    const Pct *getDto() const {  return dto;  }
    const Pct *getDefg() const { return defg; }
    const Pct *getDftmr() const {  return dftmr;  }
    const int getDpf() const {   return dpf;  }
    const double getDposs() const { return dposs;  }
    const Pct *getPct(std::string which) const;
    const double getValue(std::string which) const;

    //Other methods

private:
    const int game_no;
    const int win;
    const double oposs;
    const double spread;
    std::string opp_loc; //not const b/c it must be set in constructor

    const Pct *oor;
    const Pct *odr;
    const Pct *otr;
    const Pct *oas;
    const Pct *ost;
    const Pct *obl;
    const Pct *oto;
    const Pct *oppp;

    const int dpts;
    const Pct *dfg;
    const Pct *dtwo;
    const Pct *dthree;
    const Pct *dft;

    const double dposs;
    const Pct *dor;
    const Pct *ddr;
    const Pct *dtr;
    const Pct *das;
    const Pct *dst;
    const Pct *dbl;
    const Pct *dto;
    const Pct *defg;
    const Pct *dftmr;
    const Pct *dppp;
    const int dpf;
};


#endif //NCAA_C_TEAMGAME_H
