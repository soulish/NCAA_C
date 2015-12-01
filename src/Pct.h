//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_PCT_H
#define NCAA_C_PCT_H


class Pct {
public:
    //Constructors
    Pct(int makes, int attempts);
    Pct(int makes, int attempts, float pct);
    //Destructor
    //Copy Constructor
    //Copy Assignment

    //Getters
    const int M() const {   return m;  }
    const int A() const {   return a;  }
    const float P() const { return p;  }

    //Other methods
    float Variance() const;
    float StandardDeviation() const;
    float StandardDeviationOfTheMean() const;
    float StandardDeviationOfStandardDeviation() const;
    float VarianceOfVariance() const;

private:
    const int m,a;
    const float p;
};


#endif //NCAA_C_PCT_H
