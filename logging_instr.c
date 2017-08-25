#include <stdio.h>
#include "instrumentor.h"
#include "logging_instr.h"
#include <stdlib.h>
#include <assert.h>


void logging_begin(void *meta){

}

void logging_instrument(void *meta, struct trace_t *t, int64_t n){

	struct logging_meta* m = (struct logging_meta*)meta;
	struct wrapper_t* wrapper;

//	printf("log %lld %lld %c\n", n, t->addr,
//			(t->di == READ)? 'r':'w');
	//update transaction info
	m->seq ++;
	if(m->seq > m->len){
		m->tr_seq ++;
		m->seq = 0;
		hash_destroy(m->line_track);
		m->line_track = hash_create(m->len/2);
	}
	
	//track dirty cache line
	if(t->di == WRITE){
		
		if(!hash_lookup(m->line_track, t->addr, comp_addr)){ 
			//new cacheline dirtied in a transaction
			m->flushes ++;
			wrapper = (struct wrapper_t *)malloc(sizeof(struct wrapper_t));
			wrapper->addr = t->addr;
			hash_insert(m->line_track, t->addr, &wrapper->list);
		} 
	}

	(t->di == READ)?m->reads++:m->writes++;

}

void logging_end(void *meta){
	
	struct logging_meta* m = (struct logging_meta*)meta;
	printf("tr: %lld flushes: %lld reads:%lld writes:%lld\n", 
			m->tr_seq, m->flushes, m->reads, m->writes);
}


struct instr_func logging_func = {logging_begin, 
	logging_instrument, 
	logging_end};

struct instrumentor* logging_create(uint64_t tr_len){

	struct instrumentor* i = (struct instrumentor*)malloc(sizeof(struct instrumentor));
	struct logging_meta* m = (struct logging_meta*)malloc(sizeof(struct logging_meta));
	
	m->line_track = hash_create(tr_len / 2);
	m->tr_seq = 0;
	m->len = tr_len;
	m->seq = 0;
	m->reads=0;
	m->writes=0;
	m->flushes=0;
	i->meta = m;
	i->func = &logging_func;
	return i;
}

void logging_free(struct instrumentor *i){
	
	struct logging_meta* m = (struct logging_meta*)i->meta;
	hash_destroy(m->line_track);
}
