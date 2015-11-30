//
// Created by soulish on 11/20/15.
//

#ifndef NCAA_C_PCTS_H
#define NCAA_C_PCTS_H

#include <vector>
#include "Pct.h"

using namespace std;

class Pcts {
public:
    //Constructor
    Pcts() : pcts(new vector<Pct>) {  }
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
    float p_bar();
    float average(char);
    float weighted_average();
    float weighted_average_alt();

private:
    vector<Pct> *pcts;
};


#endif //NCAA_C_PCTS_H
