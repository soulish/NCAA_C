//
// Created by soulish on 11/23/15.
//

#include "TeamAverage.h"

TeamAverage::TeamAverage(string _teamname, boost::gregorian::date *_date, int _pts, int twoa, float twop, int threea,
                         float threep, int fta, float ftp, int ora, float orp, int dra, float drp, int toa, float top,
                         int _dpts, int dtwoa, float dtwop, int dthreea, float dthreep, int dfta, float dftp, int dora,
                         float dorp, int ddra, float ddrp, int dtoa, float dtop, int _num_games, float _spread) {
    teamname = _teamname;
    team = _teamname;
    date = _date;
    num_games = _num_games;
    spread = _spread;

    oor = new Pct((int)round(orp*ora),ora);
    oefg = new Pct((int)(round(twop*twoa)+1.5*(threep*threea)),twoa+threea);
    oftmr = new Pct((int)(round(ftp*fta)),twoa+threea);
    oto = new Pct((int)(round(top*toa)),toa);

    dor = new Pct((int)round(dorp*dora),dora);
    defg = new Pct((int)(round(dtwop*dtwoa)+1.5*(dthreep*dthreea)),dtwoa+dthreea);
    dftmr = new Pct((int)(round(dftp*dfta)),dtwoa+dthreea);
    dto = new Pct((int)(round(dtop*dtoa)),dtoa);

    opts = _pts;
    ofg = new Pct((int)(round(twop*twoa) + round(threep*threea)),twoa+threea);
    otwo = new Pct((int)(round(twop*twoa)),twoa);
    othree = new Pct((int)(round(threep*threea)),threea);
    oft = new Pct((int)(round(ftp*fta)),fta);
    odr = new Pct((int)(round(drp*dra)),dra);
    otr = new Pct((int)(round(orp*ora) + round(drp*dra)),ora+dra);

    dpts = _pts;
    dfg = new Pct((int)(round(dtwop*dtwoa) + round(dthreep*dthreea)),dtwoa+dthreea);
    dtwo = new Pct((int)(round(dtwop*dtwoa)),dtwoa);
    dthree = new Pct((int)(round(dthreep*dthreea)),dthreea);
    dft = new Pct((int)(round(dftp*dfta)),dfta);
    ddr = new Pct((int)(round(ddrp*ddra)),ddra);
    dtr = new Pct((int)(round(dorp*dora) + round(ddrp*ddra)),dora+ddra);
}
