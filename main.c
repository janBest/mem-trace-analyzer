#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "zipf.h"
#include "hash.h"

int main(void){

	double st, ss;
	uint64_t N, M;
	struct zipf_handler td_zh, sd_zh;
	int i = 0;

	N = 100000;
	M = 100000;
	st = 1.001;
	ss = 1.001;
	


	srand(time(NULL));
	zipf_init(&td_zh, st, N);
	zipf_init(&sd_zh, ss, M);

	for(i = 1; i <= N; i++){
		printf("i=%d x=%lld\n", 
			i, zipf_generator(&zh));
	}

	return 1;
}

