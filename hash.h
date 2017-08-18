#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include "list.h"

/*
 * simple implementation of a chaining hash table
 */

struct hash_t{
	uint64_t sz;
	uint64_t count;
	struct list_head *bucket_list;
};

struct hash_t* hash_create(uint64_t sz);
void hash_insert(struct hash_t *ht, uint64_t key, struct list_head *bucket);
struct list_head* hash_lookup(struct hash_t *ht, uint64_t key, uint32_t (*comparator)(uint64_t, struct list_head *));
void hash_remove(struct hash_t *ht, uint64_t key, uint32_t (*comparator)(uint64_t, struct list_head *));
void hash_destroy(struct hash_t *ht);


#endif

