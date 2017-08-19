
#ifndef TRACER_H
#define TRACER_H

#include "list.h"
#include "hash.h"
#include "zipf.h"

struct trace_generator{
	struct hash_t *n_ht;
	struct hash_t *a_ht;
	uint64_t N;
	uint64_t M;
	double ss;
	double st;
	struct zipf_handler td_zh;
	struct zipf_handler sd_zh;
};

struct trace_t{
	uint64_t n;
	uint64_t addr;
	struct list_head n_hash_list;
	struct list_head a_hash_list;
};

struct trace_t *create_trace(uint64_t n, uint64_t addr);
struct trace_generator* create_generator(uint64_t N, uint64_t M, double st, double ss);
void free_generator(struct trace_generator *g);
struct trace_t *generate_one_trace(struct trace_generator *g, struct trace_t *t);

uint32_t comp_n(uint64_t n, struct list_head *l);
uint32_t comp_a(uint64_t addr, struct list_head *l);

#endif
