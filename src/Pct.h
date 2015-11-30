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
    const int getM() const {   return m;  }
    const int getA() const {   return a;  }
    float getP() const {       return p;  }
    float getVar() const {     return var;}
    float getStd_dev() const { return std_dev; }
    float getSdom() const {    return sdom;}
    float getSdosd() const {   return sdosd;}
    float getVofv() const {    return vofv; }

    //Other methods

private:
    const int m,a;
    float p, var, std_dev,sdom, sdosd,vofv;
};


#endif //NCAA_C_PCT_H
