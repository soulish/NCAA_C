//
// Created by soulish on 11/20/15.
//

#include <math.h>
#include "Pct.h"

Pct::Pct(int makes, int attempts)
        : m(makes),
          a(attempts < 0 ? 0 : attempts),
          p(0),var(0),std_dev(0),sdom(0),sdosd(0),vofv(0){
        if (attempts == 0)
            p = 0;
        else if (makes >= attempts)
            p = 0.99;
        else
            p = makes/(float)attempts;

        var = attempts == 0 ? 0 : p*(1-p)/attempts;
        std_dev = attempts == 0 ? 0 : (float)sqrt(var);
        sdom = attempts == 0 ? 0 : (float)sqrt(p*(1-p)/(attempts*attempts));
        sdosd = attempts <= 1 ? 0 : 1/(float)sqrt(2*(attempts - 1));
        vofv = attempts <= 1 ? 0 : 1/(float)(2*(attempts - 1));
}

Pct::Pct(int makes, int attempts, float pct)
        : m(makes),
          a(attempts < 0 ? 0 : attempts),
          p(pct),
          var(0),std_dev(0),sdom(0),sdosd(0),vofv(0) {
    var = attempts == 0 ? 0 : p*(1-p)/attempts;
    std_dev = attempts == 0 ? 0 : (float)sqrt(var);
    sdom = attempts == 0 ? 0 : (float)sqrt(p*(1-p)/(attempts*attempts));
    sdosd = attempts <= 1 ? 0 : 1/(float)sqrt(2*(attempts - 1));
    vofv = attempts <= 1 ? 0 : 1/(float)(2*(attempts - 1));
}
