#include <stdio.h>
#include "instrumentor.h"
#include "ckpt_instr.h"
#include <stdlib.h>
#include <assert.h>

uint32_t comp_vhpage(uint64_t vhpage, struct list_head *l){
	struct mapping_t *mapping;
	mapping = list_entry(l, struct mapping_t, list);
	return (mapping->vhpage == vhpage);
}

void ckpt_begin(void *meta){

}

void ckpt_instrument(void *meta, struct trace_t *t, int64_t n){

	struct ckpt_meta* m = (struct ckpt_meta*)meta;
	uint64_t vhpage = t->addr / PAGE_SIZE;
	uint64_t offset = t->addr % PAGE_SIZE;
	uint64_t phpage;
	struct list_head *l;
	struct mapping_t *mapping;

	m->seq ++;

	if(m->seq > m->len){
		m->ap_seq ++;
		m->seq = 0;	
	} 
	
	if((l = hash_lookup(m->vtable, vhpage, comp_vhpage))){ 
		mapping = list_entry(l, struct mapping_t, list);
		if((t->di == WRITE) 
				&& (mapping->last_write_ap != m->ap_seq)){
			//re-mapping required
			mapping->phpage = m->max_phpage ++;
			mapping->last_write_ap = m->ap_seq;
			m->remap_count++;
	 	}
		
		if((t->di == READ) && 
				(mapping->last_read_ap != m->ap_seq)){
			mapping->last_read_ap = m->ap_seq;
			m->addr_count++;
	 	} 
		
		phpage = mapping->phpage;
		
	} else{
	 	if(t->di == WRITE){
			mapping = (struct mapping_t*)malloc(sizeof(struct mapping_t)); 
			mapping->vhpage = vhpage;
			mapping->phpage = m->max_phpage ++;
			mapping->last_write_ap = m->ap_seq;
			hash_insert(m->vtable, vhpage, &mapping->list);
			m->remap_count++;
			phpage = mapping->phpage;

		} else{
			// read something that have not been written before
			assert(vhpage < m->vh_size);
			m->addr_count++;
			phpage = vhpage;
		}		
 	} 
	
	(t->di == READ)?m->reads++:m->writes++;
	printf("-->%lld %lld %c %lld\n", n, phpage * PAGE_SIZE + offset, 
			(t->di == READ)?'r':'w', m->ap_seq);

	
}

void ckpt_end(void *meta){
	
	struct ckpt_meta* m = (struct ckpt_meta*)meta;

	printf("active periods: %lld remap: %lld addr translation: %lld reads: %lld writes: %lld\n", 
			m->ap_seq, m->remap_count, m->addr_count,
			m->reads, m->writes);

}


struct instr_func ckpt_func = {ckpt_begin, 
	ckpt_instrument, 
	ckpt_end};

struct instrumentor* ckpt_create(uint64_t ckpt_len, uint64_t vh_size){

	struct instrumentor* i = (struct instrumentor*)malloc(sizeof(struct instrumentor));
	struct ckpt_meta* m = (struct ckpt_meta*)malloc(sizeof(struct ckpt_meta));
	
	m->vtable = hash_create(1000); //eh... looks odd
	m->ap_seq = 0;
	m->len = ckpt_len;
	m->seq = 0;
	m->max_phpage = vh_size;
	m->remap_count = 0;
	m->addr_count = 0;
	m->vh_size = vh_size;
	m->reads=0;
	m->writes=0;
	i->meta = m;
	i->func = &ckpt_func;
	return i;
}

void ckpt_free(struct instrumentor *i){
	
	struct ckpt_meta* m = (struct ckpt_meta*)i->meta;
	hash_destroy(m->vtable);
}
