
#include "tracer.h"

#include <stdio.h>



uint32_t comp_n(uint64_t n, struct list_head *l){
	struct trace_t *t;
	t = list_entry(l, struct trace_t, n_hash_list);
	return (t->n == n);
}


uint32_t comp_a(uint64_t addr, struct list_head *l){
	struct trace_t *t;
	t = list_entry(l, struct trace_t, a_hash_list);
	return (t->addr == addr);
}


struct trace_generator* create_generator(uint64_t N, uint64_t M, 
		double st, double ss){
	struct trace_generator *g = (struct trace_generator*)malloc(sizeof(struct trace_generator));
	g->n_ht = hash_create(N/4);
	g->a_ht = hash_create(N/4);

	g->N = N;
	g->M = M;
	g->ss = ss;
	g->st = st;
	zipf_init(&g->td_zh, st, N);
	zipf_init(&g->sd_zh, ss, M);
	return g;
}

struct trace_t *create_trace(uint64_t n, uint64_t addr){
	struct trace_t *t;
	t = (struct trace_t *)malloc(sizeof(struct trace_t));
	t->n = n;
	t->addr = addr;
	INIT_LIST_HEAD(&t->n_hash_list);
	INIT_LIST_HEAD(&t->a_hash_list);
	return t;
}

struct trace_t *generate_one_trace(struct trace_generator *g, struct trace_t *t){
	struct trace_t *nt, *ft, *tmp;
	struct list_head *l;
	int64_t td, sd;
	uint64_t regen_sd_count = 0;

	if((l = hash_lookup(g->n_ht, t->n + 1, comp_n))){
		nt = list_entry(l, struct trace_t, n_hash_list);
		hash_remove(g->n_ht, nt->n, comp_n);
		hash_remove(g->a_ht, nt->addr, comp_a);
	} else{

regenerate_sd1:
		
		sd = zipf_generator(&g->sd_zh);

//		sd = ((regen_sd_count++) <= 4) ? 
//			zipf_generator(&g->sd_zh):(random()%(g->M/4));


		//sd = zipf_generator(&g->sd_zh);
		sd = ((rand() % 100) < 50)? sd : -sd; //the probability of sd being possitive and negative is equal

regenerate_sd2:
		
		printf("%lld ---generate sd 1 %lld\n", t->n, sd);
		
		if(((int64_t)t->addr + sd) < 0 
				|| (t->addr + sd) >= g->M){
			printf("%lld ---regenerate sd a %lld, %lld\n", t->n, t->addr, sd);
		
			if(((int64_t)t->addr - sd) >= 0 
 				&& (t->addr - sd) < g->M){
				sd = -sd;
			} else {
				goto regenerate_sd1;
 			}
   		} 
 
		printf("%lld ---generate sd 2 %lld\n", t->n, sd);


		if((l = hash_lookup(g->a_ht, t->addr + sd, comp_a))){
			tmp = list_entry(l, struct trace_t, a_hash_list);
			printf("%lld ---regenerate sd b %lld + %lld, %lld\n", t->n, t->addr, sd, tmp->n);
			sd = llabs(sd);
			sd++;
			goto regenerate_sd2;
  		} 

		nt = create_trace(t->n + 1, t->addr + sd);
  	} 

regenate_td:
	td = zipf_generator(&g->td_zh);
	//printf("---generate td %lld\n", td);
	 if(hash_lookup(g->n_ht, nt->n + td, comp_n)){
	//	printf("---regenerate td  %lld + %lld\n", nt->n, td);
	//	td++;
		goto regenate_td;
 	} 
	
	ft = create_trace(nt->n + td, nt->addr);

	printf("---future access %lld %lld\n", ft->n, ft->addr);


	hash_insert(g->n_ht, ft->n, &ft->n_hash_list);
	hash_insert(g->a_ht, ft->addr, &ft->a_hash_list);
	return nt;
}


