
#ifndef CHECKER_H
#define CHECKER_H


struct ck_meta{
	struct hash_t *history;
	uint64_t *df;
	uint64_t *dd;
	int64_t last_addr;
	uint64_t N;
	double st;
	uint64_t M;
	double ss;
};

struct instrumentor* checker_create(uint64_t N, double st, uint64_t M, double ss);
void checker_free(struct instrumentor *checker);

#endif

