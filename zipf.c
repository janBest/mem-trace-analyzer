#include <stdio.h>
#include "zipf.h"

double zipf_cdf(double x, double s, double N){
	
	double a, b;
	
	a = (pow(x, 1 - s) - 1) / (1 - s) + 0.5 
		+ pow(x, -s) / 2 + s / 12 - pow(x, -1 - s) *s / 12;

	b = (pow(N, 1 - s) - 1) / (1 - s) + 0.5 
		+ pow(N, -s) / 2 + s / 12 - pow(N, -1 - s) *s / 12;

	return a/b;
}


uint64_t inverse_cdf(double p, double s, double N){
	double x = N / 2;
	double D = 12 * (pow(N, 1 - s) - 1) / (1 - s) + 6 
		- 6 * pow(N, -s) + s - pow(N, -1 - s) * s;
	double m, mx, mxx, mxxx, a, b, newx;

	D = D * p;
//	printf("p s N: %lf %lf %lf\n", p, s, N);
	while(1){
		m = pow(x, -2 - s);
		mx = m * x;
		mxx = mx *x;
		mxxx = mxx * x;
		
		a = 12 * (mxxx -1) / (1 - s) + 6 * (1 - mxx) + (s - (mx * s)) - D;
		b = 12 * mxx + 6 * (s * mx) + (m * s * (s + 1));
		newx = x - a / b;
		newx = (newx < 1) ? 1 : newx;
//		printf("x newx (nex - x): %lf %lf %lf\n", x, newx, (newx-x));
		if(fabs(newx - x) <= 0.01)
			return (uint64_t)newx;
		x = newx;
	}

}

void zipf_init(struct zipf_handler *zh, double s, uint64_t N){
	zh->N = N;
	zh->s = s;
}

uint64_t zipf_generator(struct zipf_handler *zh){
	double p = (double)rand() / RAND_MAX;
	return inverse_cdf(p, zh->s, (double)zh->N);
}
