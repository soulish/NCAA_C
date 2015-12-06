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

      oor(new Pct((int)round(orp*ora),ora,orp)),
      oefg(new Pct(twop*twoa+1.5*(threep*threea),(double)twoa+threea)),
      oftmr(new Pct(ftp*fta,(double)twoa+threea)),
      oto(new Pct((int)(round(top*toa)),toa,top)),

      dor(new Pct((int)round(dorp*dora),dora,dorp)),
      defg(new Pct(dtwop*dtwoa+1.5*(dthreep*dthreea),(double)dtwoa+dthreea)),
      dftmr(new Pct(dftp*dfta,(double)dtwoa+dthreea)),
      dto(new Pct((int)(round(dtop*dtoa)),dtoa,dtop)),

      opts(_pts),
      ofg(new Pct(twop*twoa + threep*threea,(double)twoa+threea)),
      otwo(new Pct((int)(round(twop*twoa)),twoa,twop)),
      othree(new Pct((int)(round(threep*threea)),threea,threep)),
      oft(new Pct((int)(round(ftp*fta)),fta,ftp)),
      odr(new Pct((int)(round(drp*dra)),dra,drp)),
      otr(new Pct(orp*ora + drp*dra,(double)ora+dra)),

      dpts(_dpts),
      dfg(new Pct(dtwop*dtwoa + dthreep*dthreea,(double)dtwoa+dthreea)),
      dtwo(new Pct((int)(round(dtwop*dtwoa)),dtwoa,dtwop)),
      dthree(new Pct((int)(round(dthreep*dthreea)),dthreea,dthreep)),
      dft(new Pct((int)(round(dftp*dfta)),dfta,dftp)),
      ddr(new Pct((int)(round(ddrp*ddra)),ddra,ddrp)),
      dtr(new Pct(dorp*dora + ddrp*ddra,(double)dora+ddra))   {   }
