#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "tracer.h"
#include "instrumentor.h"
#include "checker.h"

extern char *optarg;



int main(int argc, char **argv){

	double st, ss;
	uint64_t N, M;
	int i = 0;
	struct trace_t* t, *nt;
	struct trace_generator* g;
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

	printf("N:%lld M:%lld ss:%lf st:%lf\n", N, M, st, ss);

	srand(time(NULL));
	checker = checker_create(N, st, M, ss);
	g = create_generator(N, M, st, ss);
	t = create_trace(1, 0);

	for(i = 2; i <= 2 * N; i++){
//		printf("%lld %lld\n", 
//			t->n, t->addr);
		checker->func->instrument(checker->meta, t);
		nt = generate_one_trace(g, t);
		//free(t);
		t = nt;
	} 
//	printf("%lld %lld\n", 
//		t->n, t->addr);
	checker->func->instrument(checker->meta, t);
	checker->func->end(checker->meta);


	return 1;
}



