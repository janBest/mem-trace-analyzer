
#ifndef CPU_INSTR_H
#define CPU_INSTR_H


struct cpu_meta{
	struct hash_t *history;
	uint64_t *df;
	uint64_t N;
	uint64_t L1_size;
};

struct instrumentor* cpu_create(uint64_t N, uint64_t L1_sz);
void cpu_free(struct instrumentor *i);

#endif

