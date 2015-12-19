//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_PCTS_H
#define NCAA_C_PCTS_H

#include <vector>
#include "Pct.h"
#include "math.h"

class Pcts {
public:
    //Constructor
    Pcts() : pcts(new std::vector<Pct>) {  }
    //Destructor
    virtual ~Pcts(){
        delete pcts;
    }
    //Copy Constructor
    //Copy Assignment

    //Getters
    //Note, no getter for pcts because I don't want people interacting with it

    //Other methods
    void add_pct(Pct pct);
    int length();
    Pct last();
    int sum(char);
    double p_bar();
    double average(char);
    double weighted_average();
    double weighted_average_alt();
    double weighted_std_dev();
    double std_dev();

private:
    std::vector<Pct> *pcts;
};


#endif //NCAA_C_PCTS_H
