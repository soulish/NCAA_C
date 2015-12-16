//
// Created by soulish on 12/15/15.
//

#ifndef NCAA_C_VECTORMINMAX_H
#define NCAA_C_VECTORMINMAX_H

#include <vector>
#include <float.h>

int indexOfVectorMin(const std::vector<double> &input){
    double min = DBL_MAX;
    int indexOfMin = 0;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] < min){
            min = input[i];
            indexOfMin = i;
        }
    }
    return indexOfMin;
}

#endif //NCAA_C_VECTORMINMAX_H
