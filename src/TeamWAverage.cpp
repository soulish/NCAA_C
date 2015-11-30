//
// Created by soulish on 11/23/15.
//

#include "TeamWAverage.h"

TeamWAverage::TeamWAverage(std::string _teamname, boost::gregorian::date *_date, int _pts, int twoa, float twop,
                 int threea, float threep, int fta, float ftp, int ora, float orp, int dra, float drp,
                 int toa, float top,
                 int _dpts, int dtwoa, float dtwop,
                 int dthreea, float dthreep, int dfta, float dftp, int dora, float dorp, int ddra, float ddrp,
                 int dtoa, float dtop, float _rpi, float _srs, float _sos, float _pure_wp, float _alt_wp,
                 int _num_games, float _spread)
        : TeamAverage(_teamname,_date,_pts, twoa, twop,threea,threep,
                      fta,ftp,ora,orp,dra,drp, toa,top,
                      _dpts,dtwoa,dtwop,dthreea,dthreep,dfta,dftp,
                      dora,dorp,ddra,ddrp,dtoa,dtop,_num_games,_spread),
          rpi(_rpi),
          srs(_srs),
          sos(_sos),
          pure_wp(_pure_wp),
          alt_wp(_alt_wp){    }
