#ifndef FIRCOEFFICIENT_HPP
#define FIRCOEFFICIENT_HPP

class FIRCoefficients
{
private:
    unsigned int length;
    double *coefficients;

public:
    FIRCoefficients(unsigned int length);
    FIRCoefficients(unsigned int length, double *coefficients);
    ~FIRCoefficients();
    unsigned int size();
    double &operator[](unsigned int i);
};

#endif