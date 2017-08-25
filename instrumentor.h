

#ifndef INSTRUMENTOR_H
#define INSTRUMENTOR_H
#include "tracer.h"

struct instr_func{
	void (*begin)(void *);
	void (*instrument)(void *, struct trace_t *, int64_t);
	void (*end)(void *);
//	void (*destroy)(void *); //destroy private data
};

struct instrumentor{
	void *meta;//private data
	struct instr_func *func;
};






#endif 
