#include "FIRCoefficients.hpp"

FIRCoefficients::FIRCoefficients(unsigned int length)
{
    this->length = length;
    coefficients = new double[length];
}

FIRCoefficients::FIRCoefficients(unsigned int length, double *coefficients)
{
    this->length = length;
    this->coefficients = coefficients;
}

FIRCoefficients::~FIRCoefficients()
{
    delete[] coefficients;
}

unsigned int FIRCoefficients::size()
{
    return length;
}

double &FIRCoefficients::operator[](unsigned int i)
{
    return coefficients[i];
}