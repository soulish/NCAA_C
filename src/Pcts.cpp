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
    return sumA > 0 ? sumM/(double)sumA : 0;
}

double Pcts::average(char stat) {
    int s = sum(stat);
    return length() > 0 ? s/(double)length() : 0;
}

double Pcts::weighted_average() {
    double sum_weights = 0.0;
    std::vector<Pct>::iterator v = pcts->begin();
    while(v != pcts->end()) {
        sum_weights += v->Variance() > 0 ? 1/ v->Variance() : 0;
        v++;
    }

    double s = 0.0;
    v = pcts->begin();
    while(v != pcts->end()) {
        s += v->Variance() > 0 ? (v->P() / v->Variance()) / sum_weights : 0;
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
    //based off of formula at:
    //http://www.itl.nist.gov/div898/software/dataplot/refman2/ch2/weightsd.pdf
    double p_bar = this->weighted_average();
    int nonZeroWeights = 0;

    double sum_weights = 0;
    std::vector<Pct>::iterator v = pcts->begin();
    while(v != pcts->end()) {
        if (v->Variance() > 0) {
            sum_weights += 1 / v->Variance();
            nonZeroWeights++;
        }
        v++;
    }

    double sum = 0;
    v = pcts->begin();
    while(v != pcts->end()) {
        sum += pow(v->P() - p_bar, 2) / v->Variance();
        v++;
    }

    if (nonZeroWeights > 0) {
        sum = sum / (((nonZeroWeights - 1) / (double)nonZeroWeights) * sum_weights);
    }

    return sqrt(sum);
}
