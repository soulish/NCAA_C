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
    std::vector<Pct>::iterator v = pcts->begin();
    while(v != pcts->end()) {
        if (stat == 'a')
            sum += v->A();
        else //stat == 'm'
            sum += v->M();
        v++;
    }
    return sum;
}

double Pcts::p_bar() {
    int sumM = sum('m');
    int sumA = sum('a');
    return sumM/(double)sumA;
}

double Pcts::average(char stat) {
    int s = sum(stat);
    return s/(double)length();
}

double Pcts::weighted_average() {
    double sum_weights = 0.0;
    std::vector<Pct>::iterator v = pcts->begin();
    while(v != pcts->end()) {
        sum_weights += 1/ v->Variance();
        v++;
    }

    double s = 0.0;
    v = pcts->begin();
    while(v != pcts->end()) {
        s += (v->P() / v->Variance()) / sum_weights;
        v++;
    }

    return s;
}

double Pcts::weighted_average_alt() {
    int sum_weights = 0;
    std::vector<Pct>::iterator v = pcts->begin();
    while(v != pcts->end()) {
        sum_weights += v->A();
        v++;
    }

    double s = 0.0;
    v = pcts->begin();
    while(v != pcts->end()) {
        s += (v->A() * v->P()) / sum_weights;
        v++;
    }

    return s;
}

double Pcts::weighted_std_dev() {
    double p_bar = this->p_bar();

    double sum_weights = 0;
    std::vector<Pct>::iterator v = pcts->begin();
    while(v != pcts->end()) {
        sum_weights += 1/ v->Variance();
        v++;
    }

    double sum = 0;
    v = pcts->begin();
    while(v != pcts->end()) {
        sum += ( pow(v->P() - p_bar, 2) / v->Variance()) / sum_weights;
        v++;
    }

    return sqrt(sum);
}
