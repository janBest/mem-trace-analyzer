
#include "tracer.h"

#include <stdio.h>





struct tgen* tgen_create(uint64_t N, uint64_t M, 
		double st, double ss, int r){
	struct tgen *g = (struct tgen*)malloc(sizeof(struct tgen));

	g->traces = NULL;
	g->tsz = 0;
	
	g->bm = BMap.New(M);

	g->N = N;
	g->M = M;
	g->ss = ss;
	g->st = st;
	g->read_ratio = r;

	zipf_init(&g->td_zh, st, N);
	zipf_init(&g->sd_zh, ss, M);
	return g;
}

bool is_addr_valid(int64_t addr, uint64_t M){

	return (addr >= 0) && (addr <= (M-1));
}

void tgen_work(struct tgen *g, int64_t n){
	int64_t i, j, td, sd, try, c, laddr;
	uint64_t tried = 0;
	uint64_t chosen = 0;

	g->traces = (struct trace_t*) calloc(n, sizeof(struct trace_t));
	g->tsz = n;
	
	g->traces[0].seq = 0;
	g->traces[0].addr = rand() % g->M;
	g->traces[0].di = ((rand() % 100) < g->read_ratio) ? READ: WRITE;
	BMap.set(g->bm, g->traces[0].addr);
	laddr = g->traces[0].addr;
//	printf("n %d addr %lld\n", 0, g->traces[0].addr);
	
	for(i = 1; i < g->tsz; i++){
		td = zipf_generator(&g->td_zh);	
//		printf("n %lld td %lld\n", i, td);

		if((i - td) < 0){
			c = 1;
			while(1){
				sd = (c <= 3) ? zipf_generator(&g->sd_zh): (rand() % g->M);
				sd = ((rand()%100) < 50)? sd : -sd;
				
				for(j = 1; j  <= 2; j++){
					try = laddr + sd;
//					printf("n %lld last-addr %lld sd %lld cur-addr %lld : try %lld\n", 
//							i, laddr, sd, try, c);
				 
					if(is_addr_valid(try, g->M)
	 			 			&&!BMap.check(g->bm, try)){
						goto gen_sd_finished;
					}
					sd = sd * (-1);
					c++;
	 			} 
				
	 		}
gen_sd_finished:
			tried += c;
			chosen ++;
			g->traces[i].seq = i;
			g->traces[i].addr = laddr + sd;
			BMap.set(g->bm, g->traces[i].addr);
			laddr = g->traces[i].addr;
		} else {
//			printf("n %lld re-access %lld addr %lld\n", i, i - td, g->traces[i - td].addr);
			g->traces[i].seq = i;
			g->traces[i].addr = g->traces[i - td].addr;
	 	}
		g->traces[i].di = ((rand() % 100) < g->read_ratio) ? READ: WRITE;
	}
//   printf("tried %lld sd chosen %lld avg %0.2f\n", tried, chosen, (float)tried/chosen);	
	return;
}


void tgen_replay(struct tgen *g, void(*replay)(void *arg, struct trace_t *, int64_t), void *arg){
	int64_t i = 0;

	for(i = 0; i < g->tsz; i++){
		replay(arg, &g->traces[i], i);
	}

	return;
}

void tgen_free(struct tgen *g){
	free(g->traces);
	BMap.free( g->bm );
	return;
}


struct trace_t *tgen_get_trace(struct tgen *g, uint64_t n){

	return NULL;
}



