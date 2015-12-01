//
// Created by soulish on 11/20/15.
//

#include <math.h>
#include "Pct.h"

Pct::Pct(int makes, int attempts)
        : m(makes),
          a(attempts < 0 ? 0 : attempts),
          p(a == 0 ? 0.0f : (m >= a ? 0.99f : m/(float)a)){
    //the p assignment is m/a unless a == 0 when it is 0,
    //or m >= a (often by mistake) when it is 0.99
    //0.99 is chosen because the variance=0 when p=1, and 1/variance is
    //a commonly used weight.
}

Pct::Pct(int makes, int attempts, float pct)
        : m(makes),
          a(attempts < 0 ? 0 : attempts),
          p(pct){   }

float Pct::Variance() const {
    return a == 0 ? 0 : p*(1-p)/a;
}

float Pct::StandardDeviation() const {
    return a == 0 ? 0 : (float)sqrt(p*(1-p)/a);
}

float Pct::StandardDeviationOfTheMean() const {
    return a == 0 ? 0 : (float)sqrt(p*(1-p)/(a*a));
}

float Pct::StandardDeviationOfStandardDeviation() const {
    return a <= 1 ? 0 : 1/(float)sqrt(2*(a - 1));
}

float Pct::VarianceOfVariance() const {
    return a <= 1 ? 0 : 1/(float)(2*(a - 1));
}
