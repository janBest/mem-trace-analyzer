
#include <stdio.h>
#include "instrumentor.h"
#include "checker.h"
#include <stdlib.h>


uint32_t comp(uint64_t addr, struct list_head *l){
	struct trace_t *t;
	t = list_entry(l, struct trace_t, list);
	return (t->addr == addr);
}

void ck_begin(void *meta){
	//struct ck_meta* m = (struct ck_meta*)meta;
		
}

void ck_instrument(void *meta, struct trace_t *t, int64_t n){
	struct ck_meta* m = (struct ck_meta*)meta;
	struct list_head *l;
	struct trace_t *pt;
	//uint64_t last_addr = 0;
	

	if((l = hash_lookup(m->history, t->addr, comp))){ 
		pt = list_entry(l, struct trace_t, list);
		hash_remove(m->history, pt->addr, comp);
		m->df[(t->seq - pt->seq)] ++;
	} else {
		m->dd[llabs((int64_t)t->addr - (int64_t)m->last_addr)]++;
		m->last_addr = t->addr;
	} 

	hash_insert(m->history, t->addr, &t->list);

}


void ck_end(void *meta){
	struct ck_meta* m = (struct ck_meta*)meta;
	uint64_t i;
	uint64_t count = 0;
	uint64_t *cdf1 = (uint64_t *)calloc(m->N + 1, sizeof(uint64_t));
	uint64_t *cdf2 = (uint64_t *)calloc(m->M + 1, sizeof(uint64_t));

	for(i = 1;i <= m->N; i++){
		cdf1[i] = cdf1[i-1] + m->df[i];
		count +=  m->df[i];
	}
	printf("-------checker td------------\n");
	for(i = 1; i <= m->N; i++){
		printf("%lld %0.2lf %0.2lf\n", i, (double)cdf1[i]/count, zipf_cdf((double)i, m->st, m->N));
	}

	count = 0;

	for(i = 1;i <= m->M; i++){
		cdf2[i] = cdf2[i-1] + m->dd[i];
		count +=  m->dd[i];
	}

	printf("-------checker sd------------\n");
	for(i = 1; i <= m->M; i++){
		printf("%lld %0.2lf %0.2lf\n", i, (double)cdf2[i]/count, zipf_cdf((double)i, m->ss, m->M));
	}
	
	free(cdf1);
	free(cdf2);
}


struct instr_func ck_func = {ck_begin, 
	ck_instrument, 
	ck_end};

struct instrumentor* checker_create(uint64_t N, double st, uint64_t M, double ss){
	struct instrumentor* checker = (struct instrumentor*)malloc(sizeof(struct instrumentor));
	struct ck_meta* m = (struct ck_meta*)malloc(sizeof(struct ck_meta));
	m->history = hash_create(N/4);
	m->df = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
	m->dd = (uint64_t *)calloc(M + 1, sizeof(uint64_t));
	m->N = N;
	m->st = st;
	m->M = M;
	m->ss = ss;
	m->last_addr = 0;
	checker->meta = m;
	checker->func = &ck_func;
	return checker;
}



void checker_free(struct instrumentor *checker){
	struct ck_meta* m = (struct ck_meta*)checker->meta;
	free(m->df);
	free(m->dd);
	hash_destroy(m->history);
	free(m);
	free(checker);
}



