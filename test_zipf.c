#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "zipf.h"

extern char *optarg;



int main(int argc, char **argv){

	double s;
	uint64_t N;
	uint64_t r;
	uint64_t i = 0;
	int c;
	struct zipf_handler zh;


	N = 4000000;
	s = 0;
	
	while ((c = getopt (argc, argv, "N:s:")) != -1){ 
		switch(c){
			case 'N':
				N = strtoull(optarg, 0, 10);
				break;
			case 's':
				s = strtod(optarg, 0);
				break;
			default:
				return 0;
		
		}
	}

	printf("N:%lld s:%lf\n", N, s);
	
	srand(time(NULL));
	
	zipf_init(&zh, s, N);
	uint64_t *df = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
	uint64_t *cdf = (uint64_t *)calloc(N + 1, sizeof(uint64_t));

 	for(i = 1; i <= N; i++){
		r = zipf_generator(&zh);
		//printf("%lld\n", r);
		df[r]++;		
	} 

	for(i = 1; i<=N; i++){
		cdf[i] = cdf[i-1] + df[i];
		printf("%lld %0.2lf %0.2lf\n", i, (double)cdf[i]/N, zipf_cdf((double)i, s, N));
	}

	return 1;
}



