//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_PCT_H
#define NCAA_C_PCT_H


class Pct {
public:
    int m,a;
    float p, var, std_dev,sdom, sdosd,vofv;

    Pct(int makes, int attempts);
    Pct(int makes, int attempts, float pct);

};


#endif //NCAA_C_PCT_H
