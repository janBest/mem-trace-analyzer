
#ifndef LOGGING_INSTR_H
#define LOGGING_INSTR_H


struct logging_meta{ 
	struct hash_t *line_track;
	uint64_t tr_seq;
	uint64_t len;
	uint64_t seq;
	uint64_t flushes;
	uint64_t reads;
	uint64_t writes;
};

struct instrumentor* logging_create(uint64_t tr_len);

void logging_free(struct instrumentor *i);

#endif

