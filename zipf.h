#ifndef ZIPF_H
#define ZIPF_H

/*
 * A non-uniform zipfian random number generator 
 * directly port code from https://medium.com/@jasoncrease/zipf-54912d5651cc
 */


#include <math.h>
#include <stdint.h>


double cdf(double x, double s, double N);
uint64_t inverse_cdf(double p, double s, double N);


#endif
