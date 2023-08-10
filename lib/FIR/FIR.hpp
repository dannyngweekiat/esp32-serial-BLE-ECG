#ifndef FIR_HPP
#define FIR_HPP

#include "FIRCoefficients.hpp"

class FIR
{
private:
    double *values;
    unsigned int length;
    FIRCoefficients *coefficients;

public:
    FIR(FIRCoefficients *coefficients);
    ~FIR();

    double compute(double currentInput);
};

#endif