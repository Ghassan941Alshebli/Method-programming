#include "secant.h"

#include <cmath>

double f(double x)
{
    return x * x - 4;
}

double secantMethod(double x0, double x1)
{
    double x2;

    for (int i = 0; i < 10; i++)
    {
        x2 = x1 - (f(x1) * (x1 - x0)) / (f(x1) - f(x0));

        x0 = x1;
        x1 = x2;
    }

    return x2;
}