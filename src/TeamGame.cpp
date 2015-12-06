//
// Created by soulish on 11/20/15.
//

#include "TeamGame.h"
#include <boost/algorithm/string.hpp>

TeamGame::TeamGame(std::string _team, std::string _opp, int _game_no, boost::gregorian::date *_date, std::string _loc, int _win, int _pts, int _opp_pts,
                   int _fgm, int _fga, int _threem, int _threea, int _ftm, int _fta,
                   int _orb, int _trb, int _ast, int _stl, int _blk, int _tov, int _pf,
                   int _opp_fgm, int _opp_fga, int _opp_threem, int _opp_threea, int _opp_ftm, int _opp_fta,
                   int _opp_orb, int _opp_trb, int _opp_ast, int _opp_stl, int _opp_blk, int _opp_tov, int _opp_pf,
                   double _spread)
        : Game(_team,_opp,_date,_loc,_pts,_fgm,_fga,_threem,_threea,_ftm,_fta,_pf),
          game_no(_game_no),
          win(_win),
          oposs(_fga + _tov + 0.44f * _fta),
          spread(_spread),
          oor(new Pct(_orb,_orb+(_opp_trb-_opp_orb))),
          odr(new Pct(_trb-_orb,(_trb-_orb)+_opp_orb)),
          otr(new Pct(_trb,_trb + _opp_trb)),
          oas(new Pct(_ast,_fgm)),
          ost(new Pct((double)_stl,oposs)),
          obl(new Pct((double)_blk,oposs)),
          oto(new Pct((double)_tov,oposs)),
          dpts(_opp_pts),
          dfg(new Pct(_opp_fgm,_opp_fga)),
          dtwo(new Pct(_opp_fgm-_opp_threem,_opp_fga-_opp_threea)),
          dthree(new Pct(_opp_threem,_opp_threea)),
          dft(new Pct(_opp_ftm,_opp_fta)),
          dposs(_opp_fga + _opp_tov + 0.44f*_opp_fta),
          dor(new Pct(_opp_orb,_opp_orb+(_trb - _orb))),
          ddr(new Pct(_opp_trb - _opp_orb,(_opp_trb - _opp_orb) + _orb)),
          dtr(new Pct(_opp_trb,_opp_trb+_trb)),
          das(new Pct(_opp_ast,_opp_fga)),
          dst(new Pct((double)_opp_stl,dposs)),
          dbl(new Pct((double)_opp_blk,dposs)),
          dto(new Pct((double)_opp_tov,dposs)),
          dpf(_opp_pf),
          defg(new Pct(_opp_fgm+(0.5*_opp_threem),(double)_opp_fga)),
          dftmr(new Pct(_opp_ftm,_opp_fga)){
    if (_loc == "away") opp_loc = "home";
    else if (_loc == "home") opp_loc = "away";
    else if (_loc == "neutral") opp_loc = "neutral";
}

//generic Pct getter function. I can get any Pct stat with this one function.
const Pct *TeamGame::getPct(std::string which) const {
    if (which == "ofg")
        return ofg;
    else if (which == "otwo")
        return otwo;
    else if (which == "othree")
        return othree;
    else if (which == "oft")
        return oft;
    else if (which == "oor")
        return oor;
    else if (which == "odr")
        return odr;
    else if (which == "otr")
        return otr;
    else if (which == "oas")
        return oas;
    else if (which == "ost")
        return ost;
    else if (which == "obl")
        return obl;
    else if (which == "oto")
        return oto;
    else if (which == "oefg")
        return oefg;
    else if (which == "oftmr")
        return oftmr;
    else if (which == "dfg")
        return dfg;
    else if (which == "dtwo")
        return dtwo;
    else if (which == "dthree")
        return dthree;
    else if (which == "dft")
        return dft;
    else if (which == "dor")
        return dor;
    else if (which == "ddr")
        return ddr;
    else if (which == "dtr")
        return dtr;
    else if (which == "das")
        return das;
    else if (which == "dst")
        return dst;
    else if (which == "dbl")
        return dbl;
    else if (which == "dto")
        return dto;
    else if (which == "defg")
        return defg;
    else if (which == "dftmr")
        return dftmr;
    else {
        std::cout << "Error " << which << std::endl;
        return nullptr;
    }
}

const double TeamGame::getValue(std::string which) const {
    std::vector<std::string> statType;
    boost::split(statType, which, boost::is_any_of("."));
    if (statType[0] == "opts" && statType[1] == "m")
        return (double)opts;
    else if (statType[0] == "dpts" && statType[1] == "m")
        return (double)dpts;
    else if (statType[0] == "opf" && statType[1] == "m")
        return (double)opf;
    else if (statType[0] == "dpf" && statType[1] == "m")
        return (double)dpf;
    else{
        const Pct *pct = getPct(statType[0]);
        if (statType[1] == "m")
            return (double) pct->M();
        else if (statType[1] == "a")
            return (double) pct->A();
        else if (statType[1] == "p")
            return pct->P();
    }
}
