#include <stdlib.h>
#include "hash.h"



struct hash_t* hash_create(uint64_t sz){
	struct hash_t *ht = (struct hash_t*)malloc(sizeof(struct hash_t));
	struct list_head *bl = (struct list_head*) malloc(sizeof(struct list_head) * sz);
	int i;
	for(i = 0;i < sz; i++){
		INIT_LIST_HEAD(&bl[i]);
	}
	ht->bucket_list = bl;
	ht->count = 0;
	ht->sz = sz;
	return ht;
}


void hash_insert(struct hash_t *ht, uint64_t key, struct list_head *bucket){
	struct list_head *bh = &ht->bucket_list[key % ht->sz];
	list_add(bucket, bh);
	ht->count ++;
}

struct list_head* hash_lookup(struct hash_t *ht, uint64_t key, 
		uint32_t (*comparator)(uint64_t, struct list_head *)){

	struct list_head *pos = NULL;
	struct list_head *bh = &ht->bucket_list[key % ht->sz];

	list_for_each(pos, bh){
			if (comparator(key, pos)){
				return pos;
 			}
 	}
	return NULL;
} 



void hash_remove(struct hash_t *ht, uint64_t key, 
		uint32_t (*comparator)(uint64_t, struct list_head *)){
		
	struct list_head *pos = NULL;
	struct list_head *bh = &ht->bucket_list[key % ht->sz];

	list_for_each(pos, bh){
			if(comparator(key, pos)){
				list_del(pos);
				ht->count --;
				return;
  			}
  	}
 
}

void hash_destroy(struct hash_t *ht){

	free(ht->bucket_list);
	free(ht);
}

