#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "zipf.h"

int main(void){

	double p , s, N;
	uint64_t x;

	int i = 0;

	N = 100000.0;
	s = 1.001;
	
	/*
	for(i = 1; i <= N; i++){
		p = cdf((double)i, s, N);
		x = inverse_cdf(p, s, N);
		printf("x=%lf s=%lf N=%lf p=%lf x=%lld\n", 
			(double)i, s, N, p, x);

	}
	*/

	srand(time(NULL));

	for(i = 1; i <= N; i++){
		p = (double)rand() / RAND_MAX;
		x = inverse_cdf(p, s, N);
		printf("i=%lf s=%lf N=%lf p=%lf x=%lld\n", 
			(double)i, s, N, p, x);
	}

	return 1;
}

