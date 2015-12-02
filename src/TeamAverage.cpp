//
// Created by soulish on 11/23/15.
//

#include "TeamAverage.h"

TeamAverage::TeamAverage(std::string _teamname, boost::gregorian::date *_date, int _pts, int twoa, double twop, int threea,
                         double threep, int fta, double ftp, int ora, double orp, int dra, double drp, int toa, double top,
                         int _dpts, int dtwoa, double dtwop, int dthreea, double dthreep, int dfta, double dftp, int dora,
                         double dorp, int ddra, double ddrp, int dtoa, double dtop, int _num_games, double _spread)
    : teamname(_teamname),
      date(_date),
      num_games(_num_games),
      spread(_spread),

      oor(new Pct((int)round(orp*ora),ora)),
      oefg(new Pct((int)(round(twop*twoa)+1.5*(threep*threea)),twoa+threea)),
      oftmr(new Pct((int)(round(ftp*fta)),twoa+threea)),
      oto(new Pct((int)(round(top*toa)),toa)),

      dor(new Pct((int)round(dorp*dora),dora)),
      defg(new Pct((int)(round(dtwop*dtwoa)+1.5*(dthreep*dthreea)),dtwoa+dthreea)),
      dftmr(new Pct((int)(round(dftp*dfta)),dtwoa+dthreea)),
      dto(new Pct((int)(round(dtop*dtoa)),dtoa)),

      opts(_pts),
      ofg(new Pct((int)(round(twop*twoa) + round(threep*threea)),twoa+threea)),
      otwo(new Pct((int)(round(twop*twoa)),twoa)),
      othree(new Pct((int)(round(threep*threea)),threea)),
      oft(new Pct((int)(round(ftp*fta)),fta)),
      odr(new Pct((int)(round(drp*dra)),dra)),
      otr(new Pct((int)(round(orp*ora) + round(drp*dra)),ora+dra)),

      dpts(_dpts),
      dfg(new Pct((int)(round(dtwop*dtwoa) + round(dthreep*dthreea)),dtwoa+dthreea)),
      dtwo(new Pct((int)(round(dtwop*dtwoa)),dtwoa)),
      dthree(new Pct((int)(round(dthreep*dthreea)),dthreea)),
      dft(new Pct((int)(round(dftp*dfta)),dfta)),
      ddr(new Pct((int)(round(ddrp*ddra)),ddra)),
      dtr(new Pct((int)(round(dorp*dora) + round(ddrp*ddra)),dora+ddra))   {   }
