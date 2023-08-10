#include "FIR.hpp"

FIR::FIR(FIRCoefficients *coefficients)
{
    this->coefficients = coefficients;
    length = coefficients->size();
    values = new double[length];
    for (int i = 0; i < length; i++)
    {
        values[i] = 0;
    }
}

FIR::~FIR()
{
    delete[] values;
}

double FIR::compute(double currentInput)
{
    double result = 0;
    for (int i = length - 1; i > 0; i--)
    {
        values[i] = values[i - 1];
        result += values[i] * (*coefficients)[i];
    }
    values[0] = currentInput;
    result += values[0] * (*coefficients)[0];
    return result;
}