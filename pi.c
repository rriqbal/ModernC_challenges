#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

#include "pi.h"

#define eps 0x1P-024
#define PI pi_double_nsq

// TODO use derivative of a function to optimize 
double pi_double_pt25n(unsigned pr)
{
	return 0;
}

// TODO does not work
double pi_double_n_2(unsigned pr, unsigned i, double ld, double ld6y)
{
	unsigned a = pow(i*2 - 1, 2);
	unsigned na = pow((i+1)*2 - 1, 2);
	unsigned n = na / 6.0;

	// TODO find derivative without losing precision
	unsigned cpr = 0;
	while (ld6y < 0.1)
	{
		ld6y *= 10.0;
		++cpr;
	}
	if (cpr >= pr)
		return 0;

	// chain rule
	double d6 = fabs(ld * (-1.0 * a / pow(6.0, 2)));
	double d6y = d6 * n;

	double dn = fabs(ld * (-1.0 * a / pow(6.0 + n, 2)));

	//fprintf(stderr, "%ld: %f\n", i, mdy);

	return a/(6 + pi_double_n_2(pr, ++i, dn, d6y));
}

double pi_double_n(unsigned pr)
{
	return 3 + pi_double_n_2(pr, 1, 1.0, 1.0);
}

/*
 * j = # of itir, call i with 1
 */
double pi_double_nsq_4(unsigned i, unsigned j)
{
	if (i == j)
		return pow((2*i - 1), 2)/(6);

	double d = (6 + pi_double_nsq_4(i + 1, j));

	return pow((2*i - 1), 2)/d;
}

double pi_double_nsq_3(unsigned i)
{
	return pi_double_nsq_4(1, i);
}

double pi_double_nsq_2(signed pr, double last, unsigned i)
{
	double api = pi_double_nsq_3(i);
	double dif = fabs(api - last);

	unsigned j = 0;
	while (dif < 0.1)
	{
		j += 1;
		dif *= 10;
	}

	if (j >= pr)
		return api;

	return pi_double_nsq_2(pr, api, ++i);
}

double pi_double_nsq(signed n)
{
	return 3 + pi_double_nsq_2(n, 0, 1);
}
