
#ifndef TRACER_H
#define TRACER_H

#include "list.h"
#include "hash.h"
#include "zipf.h"
#include "bmap.h"

struct tgen{
	struct trace_t *traces;
	int64_t tsz;

	BMAP *bm;
	uint64_t N;
	uint64_t M;
	double ss;
	double st;
	struct zipf_handler td_zh;
	struct zipf_handler sd_zh;
};

struct trace_t{
	int64_t seq;
	uint64_t addr;
	struct list_head list;
};


struct tgen* tgen_create(uint64_t N, uint64_t M, double st, double ss);
void tgen_free(struct tgen *g);
void tgen_work(struct tgen *g, int64_t n);
void tgen_replay(struct tgen *g, void (*replay)(void *arg, struct trace_t *, int64_t n), void *arg);

struct trace_t *tgen_get_trace(struct tgen *g, uint64_t n);





#endif
