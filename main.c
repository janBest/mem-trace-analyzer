#include <stdio.h>
#include "zipf.h"

int main(void){

	printf("x=%lf s=%lf N=%lf cdf=%lf\n", 
			5000.0, 0.0, 10000.0, cdf(5000.0, 0.0, 10000.0));
	printf("x=%lf s=%lf N=%lf cdf=%lf\n", 
			9000000.0, 0.5, 10000000.0, cdf(9000000, 0.5, 10000000.0));
	printf("x=%lf s=%lf N=%lf cdf=%lf\n", 
			2.0, 2.0, 1000.0, cdf(2.0, 2.0, 1000.0));
	printf("x=%lf s=%lf N=%lf cdf=%lf\n", 
			101.0, 2.0, 100000.0, cdf(101.0, 2.0, 100000.0));

	printf("p=%lf s=%lf N=%lf x=%lf\n", 
			9.5, 0.0, 10000.0, inverse_cdf(0.5, 0.0, 10000.0));
	printf("p=%lf s=%lf N=%lf x=%lf\n", 
			0.948671, 0.5, 10000000.0, inverse_cdf(0.948671, 0.5, 10000000.0));
	printf("p=%lf s=%lf N=%lf x=%lf\n", 
			0.762958, 2.0, 1000.0, inverse_cdf(0.762958, 2.0, 1000.0));
	printf("p=%lf s=%lf N=%lf x=%lf\n", 
			0.994095, 2.0, 100000.0, inverse_cdf(0.994095, 2.0, 100000.0));

	return 1;
}

