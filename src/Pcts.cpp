//
// Created by soulish on 11/20/15.
//

#include <iostream>
#include "Pcts.h"

void Pcts::add_pct(Pct pct) {
    pcts->push_back(pct);
}

int Pcts::length() {
    return (int)pcts->size();
}

Pct Pcts::last() {
    return pcts->back();
}

int Pcts::sum(char stat) {//this is only the sum over m or a
    if (stat != 'm' && stat != 'a')
        return 0;

    int sum = 0;
    vector<Pct>::iterator v = pcts->begin();
    while(v != pcts->end()) {
        if (stat == 'a')
            sum += v->a;
        else //stat == 'm'
            sum += v->m;
        v++;
    }
    return sum;
}

float Pcts::p_bar() {
    int sumM = sum('m');
    int sumA = sum('a');
    return sumM/(float)sumA;
}

float Pcts::average(char stat) {
    int s = sum(stat);
    return s/(float)length();
}

float Pcts::weighted_average() {
    float sum_weights = 0;
    vector<Pct>::iterator v = pcts->begin();
    while(v != pcts->end()) {
        sum_weights += 1/v->var;
        v++;
    }

    float s = 0;
    v = pcts->begin();
    while(v != pcts->end()) {
        s += (v->p/v->var)/sum_weights;
        v++;
    }

    return s;
}

float Pcts::weighted_average_alt() {
    int sum_weights = 0;
    vector<Pct>::iterator v = pcts->begin();
    while(v != pcts->end()) {
        sum_weights += v->a;
        v++;
    }

    float s = 0;
    v = pcts->begin();
    while(v != pcts->end()) {
        s += (v->a*v->p)/sum_weights;
        v++;
    }

    return s;
}
