#include "MovingAverage.hpp"

FIRCoefficients *createMovingAverageCoefficients(unsigned int length)
{
    FIRCoefficients *coefficients = new FIRCoefficients(length + 1);
    (*coefficients)[0] = 1.0;
    for (int i = 1; i <= length; i++)
    {
        (*coefficients)[i] = -1.0 / (double)length;
    }
    return coefficients;
}
