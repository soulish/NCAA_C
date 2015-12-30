//
// Created by soulish on 12/29/15.
//

#ifndef NCAA_C_VEGASCONVERSION_H
#define NCAA_C_VEGASCONVERSION_H

#include <math.h>

double pctToMoneyline(double pct, bool convert = false){
    double moneyline = (pct > 0 && pct <= 1) ? 100 / pct - 100 : 0;
    if (convert && moneyline < 100) moneyline = -10000 / moneyline;
    return moneyline;
}

double spreadToMoneyline(double spread, bool convert = false){
    double moneyline = 14.6995 * exp(0.165574 * spread + 1.74433);
    if (convert && moneyline < 100) moneyline = -10000 / moneyline;
    return moneyline;
}

double moneylineToSpread(double moneyline){
    if (moneyline < 0) moneyline = -10000 / moneyline;
    double spread = (log(moneyline / 14.6995) - 1.74433) / 0.165574;
    return spread;
}

double pctToSpread(double pct){
    double moneyline = pctToMoneyline(pct);
    double spread = moneylineToSpread(moneyline);
    return spread;
}

double moneylineToPct(double moneyline){
    if (moneyline < 0) moneyline = -10000 / moneyline;
    double pct = 100.0 / (100.0 + moneyline);
    return pct;
}

double spreadToPct(double spread){
    double moneyline = spreadToMoneyline(spread);
    double pct = moneylineToPct(moneyline);
    return pct;
}

#endif //NCAA_C_VEGASCONVERSION_H
