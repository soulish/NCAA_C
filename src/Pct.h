//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_PCT_H
#define NCAA_C_PCT_H

#include <math.h>

class Pct {
public:
    //Constructors
    Pct(int makes, int attempts);
    Pct(int makes, int attempts, double pct);
    Pct(double makes, double attempts);
    //Destructor
    //Copy Constructor
    //Copy Assignment

    //Getters
    const int M() const {   return m;  }
    const int A() const {   return a;  }
    const double P() const { return p;  }

    //Other methods
    double Variance() const;
    double StandardDeviation() const;
    double StandardDeviationOfTheMean() const;
    double StandardDeviationOfStandardDeviation() const;
    double VarianceOfVariance() const;

private:
    const int m,a;
    const double p;
};


#endif //NCAA_C_PCT_H
