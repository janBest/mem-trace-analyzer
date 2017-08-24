#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "tracer.h"
#include "instrumentor.h"
#include "checker.h"

extern char *optarg;


void rreplay(void *arg, struct trace_t *t, int64_t n){
	
	struct instrumentor *checker = (struct instrumentor *)arg;

//	printf("%lld %lld\n", n, t->addr);
	checker->func->instrument(checker->meta, t, n);
}



int main(int argc, char **argv){

	double st, ss;
	uint64_t N, M;
	int i = 0;
	struct tgen* g;
	struct instrumentor *checker;
	int c;


	N = 4000000;
	M = 4*1024*1024ll;
	st = 0;
	ss = 0;
	
	while ((c = getopt (argc, argv, "N:M:t:s:")) != -1){ 
		switch(c){
			case 'N':
				N = strtoull(optarg, 0, 10);
				break;
			case 'M':
				M = strtoull(optarg, 0, 10);
				M = M * 1024 * 1024ll; // MB -> B
				break;
			case 't':
				st = strtod(optarg, 0);
				break;
			case 's':
				ss = strtod(optarg, 0);
				break;
			default:
				return 0;
		
		}
	}

	printf("N:%lld M:%lld ss:%lf st:%lf\n", N, M, ss, st);

	srand(time(NULL));
	checker = checker_create(N, st, M, ss);

	g = tgen_create(N, M, st, ss);
	tgen_work(g, 2 * N);
	tgen_replay(g, rreplay, (void *)checker);
	
	checker->func->end(checker->meta);

	tgen_free(g);
	

	return 1;
}



