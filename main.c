#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "tracer.h"
#include "instrumentor.h"
#include "checker.h"
#include "cpu_instr.h"
#include "ckpt_instr.h"
#include "logging_instr.h"

#define INSTR_MAX 10

extern char *optarg;


struct replay_ctx{
	struct instrumentor *instr[INSTR_MAX];
	int count;
};



bool instrumentor_push(struct replay_ctx *ctx, struct instrumentor *i){
	if(ctx->count >= INSTR_MAX)
		return false;
	ctx->instr[ctx->count++] = i;
	return true;
}

struct instrumentor* instrumentor_pop(struct replay_ctx *ctx){
	if(ctx->count <= 0)
		return NULL;
	
	return ctx->instr[--ctx->count];
}



void rreplay(void *arg, struct trace_t *t, int64_t n){
	
	int i;
    struct instrumentor *instr;	
	struct replay_ctx *ctx = (struct replay_ctx *)arg;

	printf("%lld %lld %c\n", n, t->addr,
			(t->di == READ)? 'r':'w');

	for(i = 0; i < ctx->count; i++){
		instr = ctx->instr[i];
		instr->func->instrument(instr->meta, t, n);
	}
}



int main(int argc, char **argv){

	double st, ss;
	uint64_t N, M, n;
	int r;
	int i = 0;
	struct tgen* g;
	struct replay_ctx ctx;
	int c;


	N = 4000000;
	M = 4*1024*1024ll;
	st = 0;
	ss = 0;
	r = 0;
	n = 10;

	while ((c = getopt (argc, argv, "N:M:t:s:r:n:")) != -1){ 
		switch(c){
			case 'N':
				N = strtoull(optarg, 0, 10);
				break;
			case 'M':
				M = strtoull(optarg, 0, 10);
				M = M * 1024 * 1024ll; // MB -> B
				M /= 64; //B -> cacheline
				break;
			case 't':
				st = strtod(optarg, 0);
				break;
			case 's':
				ss = strtod(optarg, 0);
				break;
			case 'r':
				r = atoi(optarg);
				break;
			case 'n':
				n = strtoull(optarg, 0, 10);
				break;
			default:
				return 0;
		
		}
	}

	printf("N:%lld M:%lld ss:%lf st:%lf r:%d n:%lld\n", N, M, ss, st, r, n);

	srand(time(NULL));
	
	memset(&ctx, 0, sizeof(struct replay_ctx));
	
	//instrumentor_push(&ctx, checker_create(N, st, M, ss));
	instrumentor_push(&ctx, logging_create(n));
	instrumentor_push(&ctx, cpu_create(N, 256));
	instrumentor_push(&ctx, ckpt_create(n, M));


	g = tgen_create(N, M, st, ss, r); // MEMORY SIZE IN CACHE SIZE
	tgen_work(g, N);
	tgen_replay(g, rreplay, &ctx);
	
	for(i = 0; i < ctx.count; i++){
		ctx.instr[i]->func->end(ctx.instr[i]->meta);
	}

	tgen_free(g);
//	checker_free(checker);

	return 1;
}



