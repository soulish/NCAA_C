//
// Created by soulish on 11/20/15.
//

#include <math.h>
#include "Pct.h"

Pct::Pct(int makes, int attempts)
        : m(makes),
          a(attempts < 0 ? 0 : attempts),
          p(a == 0 ? 0.0 : (m >= a ? 0.99 : m/(double)a)){
    //the p assignment is m/a unless a == 0 when it is 0,
    //or m >= a (often by mistake) when it is 0.99
    //0.99 is chosen because the variance=0 when p=1, and 1/variance is
    //a commonly used weight.
}

Pct::Pct(int makes, int attempts, double pct)
        : m(makes),
          a(attempts < 0 ? 0 : attempts),
          p(pct){   }

Pct::Pct(double makes, double attempts)
        : m((int)round(makes)),
          a((int)round(attempts)),
          p(a == 0 ? 0.0 : makes/attempts) {  }

double Pct::Variance() const {
    return a == 0 ? 0 : p*(1-p)/a;
}

double Pct::StandardDeviation() const {
    return a == 0 ? 0 : (double)sqrt(p*(1-p)/a);
}

double Pct::StandardDeviationOfTheMean() const {
    return a == 0 ? 0 : (double)sqrt(p*(1-p)/(a*a));
}

double Pct::StandardDeviationOfStandardDeviation() const {
    return a <= 1 ? 0 : 1/(double)sqrt(2*(a - 1));
}

double Pct::VarianceOfVariance() const {
    return a <= 1 ? 0 : 1/(double)(2*(a - 1));
}
