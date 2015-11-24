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
    vector<Pct> *pcts;

    Pcts() {
        pcts = new vector<Pct>();
    }

    void add_pct(Pct pct);
    int length();
    Pct last();
    int sum(char);
    float p_bar();
    float average(char);
    float weighted_average();
    float weighted_average_alt();
};


#endif //NCAA_C_PCTS_H
