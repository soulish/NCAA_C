//
// Created by soulish on 11/23/15.
//

#include "TeamAverage.h"
#include <boost/algorithm/string.hpp>

TeamAverage::TeamAverage(std::string _teamname, boost::gregorian::date _date, int _pts, int twoa, double twop, int threea,
                         double threep, int fta, double ftp, int ora, double orp, int dra, double drp, int toa, double top,
                         int _dpts, int dtwoa, double dtwop, int dthreea, double dthreep, int dfta, double dftp, int dora,
                         double dorp, int ddra, double ddrp, int dtoa, double dtop, int _num_games)
    : teamname(_teamname),
      date(_date),
      num_games(_num_games),
      oposs(twoa+threea + top*toa + 0.44 * fta),
      dposs(dtwoa+dthreea + dtop*dtoa + 0.44 * dfta),

      oor(new Pct((int)round(orp*ora),ora,orp)),
      oefg(new Pct(twop*twoa+1.5*(threep*threea),(double)(twoa+threea))),
      oftmr(new Pct(ftp*fta,(double)(twoa+threea))),
      oto(new Pct((int)(round(top*toa)),toa,top)),

      dor(new Pct((int)round(dorp*dora),dora,dorp)),
      defg(new Pct(dtwop*dtwoa+1.5*(dthreep*dthreea),(double)(dtwoa+dthreea))),
      dftmr(new Pct(dftp*dfta,(double)(dtwoa+dthreea))),
      dto(new Pct((int)(round(dtop*dtoa)),dtoa,dtop)),

      opts(_pts),
      oppp(new Pct((double)_pts,oposs)),
      ofg(new Pct(twop*twoa + threep*threea,(double)(twoa+threea))),
      otwo(new Pct((int)(round(twop*twoa)),twoa,twop)),
      othree(new Pct((int)(round(threep*threea)),threea,threep)),
      oft(new Pct((int)(round(ftp*fta)),fta,ftp)),
      odr(new Pct((int)(round(drp*dra)),dra,drp)),
      otr(new Pct(orp*ora + drp*dra,(double)(ora+dra))),

      dpts(_dpts),
      dppp(new Pct((double)_dpts,dposs)),
      dfg(new Pct(dtwop*dtwoa + dthreep*dthreea,(double)(dtwoa+dthreea))),
      dtwo(new Pct((int)(round(dtwop*dtwoa)),dtwoa,dtwop)),
      dthree(new Pct((int)(round(dthreep*dthreea)),dthreea,dthreep)),
      dft(new Pct((int)(round(dftp*dfta)),dfta,dftp)),
      ddr(new Pct((int)(round(ddrp*ddra)),ddra,ddrp)),
      dtr(new Pct(dorp*dora + ddrp*ddra,(double)(dora+ddra)))   {   }

const Pct *TeamAverage::getPct(std::string which) const {
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
    else if (which == "oto")
        return oto;
    else if (which == "oefg")
        return oefg;
    else if (which == "oftmr")
        return oftmr;
    else if (which == "oppp")
        return oppp;
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
    else if (which == "dto")
        return dto;
    else if (which == "defg")
        return defg;
    else if (which == "dftmr")
        return dftmr;
    else if (which == "dppp")
        return dppp;
    else {
        std::cout << "Error " << which << std::endl;
        return nullptr;
    }
}

const double TeamAverage::getValue(std::string which) const {
    std::vector<std::string> statType;
    boost::split(statType, which, boost::is_any_of("."));
    if (statType[0] == "opts" && statType[1] == "m")
        return (double)opts;
    else if (statType[0] == "dpts" && statType[1] == "m")
        return (double)dpts;
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
