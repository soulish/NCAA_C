//
// Created by soulish on 12/1/15.
//

#ifndef NCAA_C_DOUBLEFORMATTER_H
#define NCAA_C_DOUBLEFORMATTER_H

#include <string>
#include <sstream>

std::string doubleFormatter(const double x, const int decDigits) {
    std::stringstream ss;
    ss << std::fixed;
    ss.precision(decDigits); // set # places after decimal
    ss << x;
    return ss.str();
}

#endif //NCAA_C_DOUBLEFORMATTER_H
