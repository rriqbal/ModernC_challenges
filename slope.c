#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define eps 0x1P-024

double slope(double x, double (*f)(double))
{
	return (f(x + eps) - f(x))/eps;
}

double slope_sin(double x)
{
	return slope(x, sin);
}
