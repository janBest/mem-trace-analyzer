#ifndef ZIPF_H
#define ZIPF_H

/*
 * A non-uniform zipfian random number generator 
 * directly port code from https://medium.com/@jasoncrease/zipf-54912d5651cc
 */


#include <math.h>
#include <stdint.h>
#include <stdlib.h>

struct zipf_handler{
	uint64_t N;
	double s;
};

double cdf(double x, double s, double N);
uint64_t inverse_cdf(double p, double s, double N);

void zipf_init(struct zipf_handler *zh, double s, uint64_t N);
uint64_t zipf_generator(struct zipf_handler *zh);


#endif
