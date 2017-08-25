
#ifndef CPU_INSTR_H
#define CPU_INSTR_H

struct access_t{
	uint64_t addr;
	struct list_head lru_list;
	struct list_head hash_list;
};

struct cpu_meta{
	struct hash_t *hash;
	struct list_head lru_head;
	uint64_t L1_size;
	uint64_t count;
	uint64_t misses;
	uint64_t total;
};

struct instrumentor* cpu_create(uint64_t L1_sz);
void cpu_free(struct instrumentor *i);

#endif

