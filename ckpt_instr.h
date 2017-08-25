
#ifndef CKPT_INSTR_H
#define CKPT_INSTR_H




struct mapping_t{
	uint64_t vhpage;
	uint64_t phpage;
	uint64_t last_read_ap; //last active period that read the mapping
	uint64_t last_write_ap; //last active period that update the mapping
	struct list_head list;
};


struct ckpt_meta{ 
	struct hash_t *vtable;
	uint64_t ap_seq;
	uint64_t len;
	uint64_t seq;
	uint64_t vh_size;
	uint64_t max_phpage;
	uint64_t remap_count;
	uint64_t addr_count;
	uint64_t reads;
	uint64_t writes;
};

struct instrumentor* ckpt_create(uint64_t ckpt_len, uint64_t vh_size);

void ckpt_free(struct instrumentor *i);

#endif

