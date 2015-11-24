//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_TEAMGAME_H
#define NCAA_C_TEAMGAME_H


#include "Game.h"
#include <boost/date_time/gregorian/gregorian.hpp>

class TeamGame : public Game {

public:
    int game_no;
    int win;
    float poss;
    float spread;
    string opp_loc;

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

    TeamGame(string _team, string _opp, int _game_no, boost::gregorian::date *_date, string _loc, int _win, int _pts, int _opp_pts,
             int _fgm, int _fga, int _threem, int _threea, int _ftm, int _fta,
             int _orb, int _trb, int _ast, int _stl, int _blk, int _tov, int _pf,
             int _opp_fgm, int _opp_fga, int _opp_threem, int _opp_threea, int _opp_ftm, int _opp_fta,
             int _opp_orb, int _opp_trb, int _opp_ast, int _opp_stl, int _opp_blk, int _opp_tov, int _opp_pf,
             float _spread) : Game(_team,_opp,_date,_loc,_pts,_fgm,_fga,_threem,_threea,_ftm,_fta,_pf){
        game_no = _game_no;
        win = _win;
        poss = _fga + _tov + 0.44f*_fta;
        spread = _spread;

        if (_loc == "away") opp_loc = "home";
        else if (_loc == "home") opp_loc = "away";
        else if (_loc == "neutral") opp_loc = "neutral";

        oor = new Pct(_orb,_orb+(_opp_trb-_opp_orb));
        odr = new Pct(_trb-_orb,(_trb-_orb)+_opp_orb);
        otr = new Pct(_trb,_trb + _opp_trb);
        oas = new Pct(_ast,_fgm);
        ost = new Pct(_stl,(int)poss);
        obl = new Pct(_blk,(int)poss);
        oto = new Pct(_tov,(int)poss);

        dpts = _opp_pts;
        dfg = new Pct(_opp_fgm,_opp_fga);
        dtwo = new Pct(_opp_fgm-_opp_threem,_opp_fga-_opp_threea);
        dthree = new Pct(_opp_threem,_opp_threea);
        dft = new Pct(_opp_ftm,_opp_fta);
        dor = new Pct(_opp_orb,_opp_orb+(_trb - _orb));
        ddr = new Pct(_opp_trb - _opp_orb,(_opp_trb - _opp_orb) + _orb);
        dtr = new Pct(_opp_trb,_opp_trb+_trb);
        das = new Pct(_opp_ast,_opp_fga);

        dposs = _opp_fga + _opp_tov + 0.44f*_opp_fta;
        dst = new Pct(_opp_stl,(int)dposs);
        dbl = new Pct(_opp_blk,(int)dposs);
        dto = new Pct(_opp_tov,(int)dposs);
        dpf = _opp_pf;
        defg = new Pct(_opp_fgm+(int)(0.5*_opp_threem),_opp_fga);
        dftmr = new Pct(_opp_ftm,_opp_fga);
    }
};


#endif //NCAA_C_TEAMGAME_H
