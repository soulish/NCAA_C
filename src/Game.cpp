//
// Created by soulish on 11/20/15.
//

#include "Game.h"

Game::Game(string _team, string _opp, boost::gregorian::date *_date,string _loc, int _pts, int _fgm, int _fga, int _threem, int _threea,
           int _ftm, int _fta, int _pf) {
    team = _team;
    opp = _opp;
    date = _date;
    loc = _loc;
    pts = _pts;
    fg = new Pct(_fgm,_fga);
    two = new Pct(_fgm - _threem, _fga - _threea);
    three = new Pct(_threem, _threea);
    ft = new Pct(_ftm,_fta);
    efg = new Pct(_fgm+(int)(0.5*_threem),_fga);
    ts = new Pct(_pts,(int)(_fga+0.44*_fta));
    ftmr = new Pct(_ftm,_fga);
    pf = _pf;
}
