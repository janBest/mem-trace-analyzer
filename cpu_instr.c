
#include <stdio.h>
#include "instrumentor.h"
#include "cpu_instr.h"
#include <stdlib.h>

void cpu_begin(void *meta){
		
}

void cpu_instrument(void *meta, struct trace_t *t, int64_t n){
	struct cpu_meta* m = (struct cpu_meta*)meta;
	struct wrapper_t* wrapper;
	struct list_head *l;
	struct trace_t *pt;
	//uint64_t last_addr = 0;
	

	if((l = hash_lookup(m->history, t->addr, comp_addr))){ 
		wrapper = list_entry(l, struct wrapper_t, list);
		pt = (struct trace_t*) wrapper->data;
		hash_remove(m->history, pt->addr, comp_addr);
		m->df[(t->seq - pt->seq)] ++;
	}  
	wrapper = (struct wrapper_t*)malloc(sizeof(struct wrapper_t));
	wrapper->data = (void*) t;
	hash_insert(m->history, t->addr, &wrapper->list);

}


void cpu_end(void *meta){
	struct cpu_meta* m = (struct cpu_meta*)meta;
	uint64_t i;
	uint64_t count = 0;
	uint64_t *cdf = (uint64_t *)calloc(m->N + 1, sizeof(uint64_t));
	uint64_t max_reuse = (m->N > m->L1_size) ? m->L1_size : m->N;

	for(i = 1;i <= m->N; i++){
		cdf[i] = cdf[i-1] + m->df[i];
		count +=  m->df[i];
	}
	
	printf("L1 miss ratio: %0.2f\n", 1 - (float)cdf[max_reuse]/count);
	free(cdf);
}


struct instr_func cpu_func = {cpu_begin, 
	cpu_instrument, 
	cpu_end};

struct instrumentor* cpu_create(uint64_t N, uint64_t L1_sz){
	struct instrumentor* i = (struct instrumentor*)malloc(sizeof(struct instrumentor));
	struct cpu_meta* m = (struct cpu_meta*)malloc(sizeof(struct cpu_meta));
	m->history = hash_create(L1_sz);
	m->df = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
	m->L1_size = L1_sz;
	m->N = N;
	i->meta = m;
	i->func = &cpu_func;
	return i;
}



void cpu_free(struct instrumentor *i){
	struct cpu_meta* m = (struct cpu_meta*)i->meta;
	free(m->df);
	hash_destroy(m->history);
	free(m);
	free(i);
}



