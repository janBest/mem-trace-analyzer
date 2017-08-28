
#ifndef TRACER_H
#define TRACER_H

#include "list.h"
#include "hash.h"
#include "zipf.h"
#include "bmap.h"

#define READ  1
#define WRITE 2

#define WORD_SIZE 8
//#define PAGE_SIZE (4096 / CACHELINE_SIZE)
#define PAGE_SIZE (4096 / WORD_SIZE)
#define CACHELINE_SIZE (64 / WORD_SIZE)

struct tgen{
	struct trace_t *traces;
	int64_t tsz;
	
	BMAP *bm;
	uint64_t N;
	uint64_t M;
	double ss;
	double st;
	
	int read_ratio;

	struct zipf_handler td_zh;
	struct zipf_handler sd_zh;
};

struct trace_t{
	int64_t seq;
	uint64_t addr;
	uint8_t di; //1 read, 2 write
};

struct wrapper_t{
	struct list_head list;
	uint64_t addr;
	uint64_t seq;
};

struct tgen* tgen_create(uint64_t N, uint64_t M, double st, double ss, int r);
void tgen_free(struct tgen *g);
void tgen_work(struct tgen *g, int64_t n);
void tgen_replay(struct tgen *g, void (*replay)(void *arg, struct trace_t *, int64_t n), void *arg);

struct trace_t *tgen_get_trace(struct tgen *g, uint64_t n);


uint32_t comp_addr(uint64_t addr, struct list_head *l);




#endif
