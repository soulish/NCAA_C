//
// Created by soulish on 11/23/15.
//

#include "TeamWAverage.h"

TeamWAverage::TeamWAverage(std::string _teamname, boost::gregorian::date *_date, int _pts, int twoa, double twop,
                 int threea, double threep, int fta, double ftp, int ora, double orp, int dra, double drp,
                 int toa, double top,
                 int _dpts, int dtwoa, double dtwop,
                 int dthreea, double dthreep, int dfta, double dftp, int dora, double dorp, int ddra, double ddrp,
                 int dtoa, double dtop, double _rpi, double _srs, double _sos,
                 int _num_games, double _spread)
        : TeamAverage(_teamname,_date,_pts, twoa, twop,threea,threep,
                      fta,ftp,ora,orp,dra,drp, toa,top,
                      _dpts,dtwoa,dtwop,dthreea,dthreep,dfta,dftp,
                      dora,dorp,ddra,ddrp,dtoa,dtop,_num_games,_spread),
          rpi(_rpi),
          srs(_srs),
          sos(_sos) {  }
