

#ifndef INSTRUMENTOR_H
#define INSTRUMENTOR_H
#include "tracer.h"

struct instr_func{
	void (*begin)(void *);
	void (*instrument)(void *, struct trace_t *);
	void (*end)(void *);
};

struct instrumentor{
	void *meta;
	struct instr_func *func;
};






#endif 
