
#include <stdio.h>
#include "instrumentor.h"
#include "cpu_instr.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

uint32_t comp_lru(uint64_t addr, struct list_head *l){
	struct access_t* access;
	access = list_entry(l, struct access_t, hash_list);

	return (access->addr == addr);
}

void cpu_begin(void *meta){
		
}

void cpu_instrument(void *meta, struct trace_t *t, int64_t n){
	struct cpu_meta* m = (struct cpu_meta*)meta;
	struct access_t* access = NULL;
	struct list_head *l;
//	struct trace_t *pt;
	//uint64_t last_addr = 0;
	
//	printf("cpu %lld %lld %c\n", n, t->addr,
//			(t->di == READ)? 'r':'w');
	
	m->total++;
	if((l = hash_lookup(m->hash, (t->addr / CACHELINE_SIZE), comp_lru))){ 
		//hit
		access = list_entry(l, struct access_t, hash_list);
		list_move_tail(&(access->lru_list), &(m->lru_head));
	} else {
		//miss
		m->misses++;
		if(m->count >= m->L1_size){
			//full
			assert(m->count == m->L1_size);
			l = m->lru_head.next;
			access = list_entry(l, struct access_t, lru_list);
			assert(access != NULL);
			hash_remove(m->hash, access->addr, comp_lru);
			list_del(l);
			m->count --;
			free(access);
		}
		assert(m->count < m->L1_size);
		access = (struct access_t *)malloc(sizeof(struct access_t));
		access->addr = (t->addr / CACHELINE_SIZE);

		list_add_tail(&(access->lru_list), &(m->lru_head));
		hash_insert(m->hash, access->addr, &access->hash_list);
		m->count++;
	}

}


void cpu_end(void *meta){
	struct cpu_meta* m = (struct cpu_meta*)meta;

	printf("%s L1_miss_ratio: %0.2f \n", m->name, (float)m->misses/m->total);
}


struct instr_func cpu_func = {cpu_begin, 
	cpu_instrument, 
	cpu_end};

struct instrumentor* cpu_create(const char *name,uint64_t L1_sz){
	struct instrumentor* i = (struct instrumentor*)malloc(sizeof(struct instrumentor));
	struct cpu_meta* m = (struct cpu_meta*)malloc(sizeof(struct cpu_meta));
	
	INIT_LIST_HEAD(&m->lru_head);
	m->hash = hash_create(L1_sz);
	m->L1_size = L1_sz;
	m->count = 0;
	m->total = 0;
	m->misses = 0;
	strcpy(m->name, name);
	i->meta = m;
	i->func = &cpu_func;
	return i;
}



void cpu_free(struct instrumentor *i){
	struct cpu_meta* m = (struct cpu_meta*)i->meta;
	hash_destroy(m->hash);
	free(m);
	free(i);
}



