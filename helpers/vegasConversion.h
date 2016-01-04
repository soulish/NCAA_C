//
// Created by soulish on 12/29/15.
//

#ifndef NCAA_C_VEGASCONVERSION_H
#define NCAA_C_VEGASCONVERSION_H

#include <math.h>

double moneylineToSpread(double moneyline){
    if (moneyline < 0) moneyline = -10000 / moneyline;
    double spread;
    if (moneyline >= 100)
        spread = (log(moneyline / 14.7) - 1.7) / 0.169;
    else
        spread = (log(moneyline / 16.31) - 1.877) / 0.17;
    return spread;
}

double spreadToMoneyline(double spread, bool convert = false){
    double moneyline;
    if (spread >= 0)
        moneyline = 14.7 * exp(0.169 * spread + 1.7);
    else
        moneyline = 16.31 * exp(0.17 * spread + 1.877);
    if (convert && moneyline < 100) moneyline = -10000 / moneyline;
    return moneyline;
}

double pctToMoneyline(double pct, bool convert = false){
    double moneyline;
    if (pct < 0.5) //simple conversion for the underdog
        moneyline = 100 / pct - 100;
    else{ //for favorites, first determine underdog's spread, then use opposite of that to get moneyline
        double oppositeMoneyLine = 100 / (1-pct) - 100;
        double spread = moneylineToSpread(oppositeMoneyLine);
        moneyline = spreadToMoneyline(-spread);
    }

    if (convert && moneyline < 100) moneyline = -10000 / moneyline;
    return moneyline;
}

double pctToSpread(double pct){
    double spread;
    if (pct < 0.5) {
        double moneyline = pctToMoneyline(pct);
        spread = moneylineToSpread(moneyline);
    }
    else{
        double oppositeMoneyLine = pctToMoneyline(1-pct);
        spread = -1 * moneylineToSpread(oppositeMoneyLine);
    }
    return spread;
}

double pureMoneylineToPct(double moneyline){ //this is the actual conversion, but vegas uses a different way
    if (moneyline < 0) moneyline = -10000 / moneyline;
    double pct = 100.0 / (100.0 + moneyline);
    return pct;
}

double moneylineToPct(double moneyline){
    if (moneyline < 0) moneyline = -10000 / moneyline;
    double pct;
    if (moneyline >= 100) //straightforward for underdogs
        pct = 100.0 / (100.0 + moneyline);
    else{//favorites are different because vegas is weird.  Get the opposite spread to determine opp moneyline
        //which then can be used to get the opposite percentage, which can be used to get this pct.
        double spread = moneylineToSpread(moneyline);
        double oppMoneyline = spreadToMoneyline(-spread);
        pct = 1 - (100.0 / (100.0 + oppMoneyline));
    }
    return pct;
}

double spreadToPct(double spread){
    double moneyline;
    double pct;
    if (spread > 0) {
        moneyline = spreadToMoneyline(spread);
        pct = moneylineToPct(moneyline);
    }
    else{
        moneyline = spreadToMoneyline(-spread);
        pct = 1 - moneylineToPct(moneyline);
    }
    return pct;
}

#endif //NCAA_C_VEGASCONVERSION_H
