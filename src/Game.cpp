//
// Created by soulish on 11/20/15.
//

#include "Game.h"

Game::Game(string _team, string _opp, boost::gregorian::date *_date,string _loc, int _pts, int _fgm, int _fga, int _threem, int _threea,
           int _ftm, int _fta, int _pf)
        : team(_team),
          opp(_opp),
          date(_date),
          loc(_loc),
          opts(_pts),
          ofg(new Pct(_fgm, _fga)),
          otwo(new Pct(_fgm - _threem, _fga - _threea)),
          othree(new Pct(_threem, _threea)),
          oft(new Pct(_ftm, _fta)),
          oefg(new Pct(_fgm + (int)(0.5 * _threem), _fga)),
          ots(new Pct(_pts, (int)(_fga + 0.44 * _fta))),
          oftmr(new Pct(_ftm, _fga)),
          opf(_pf){ }
