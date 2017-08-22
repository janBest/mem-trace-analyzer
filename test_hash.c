#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "zipf.h"
#include "hash.h"

struct record{
	uint64_t key;
	uint64_t value;
	struct list_head hash_list;
};

uint32_t comp_record(uint64_t key, struct list_head *l){
	struct record *r;
	r = list_entry(l, struct record, hash_list);
	return (r->key == key);
}

int main(void){
	
	struct hash_t *ht = hash_create(64);
	struct record *tmp;
	struct list_head *l;

	struct record *r1 = (struct record*)malloc(sizeof(struct record));
	r1->key = 1;
	r1->value = 1;
	struct record *r2 = (struct record*)malloc(sizeof(struct record));
	r2->key = 2;
	r2->value = 2;
	struct record *r3 = (struct record*)malloc(sizeof(struct record));
	r3->key = 3;
	r3->value = 3;
	struct record *r4 = (struct record*)malloc(sizeof(struct record));
	r4->key = 65;
	r4->value = 65;

	hash_insert(ht, 1, &r1->hash_list);
	hash_insert(ht, 2, &r2->hash_list);
	hash_insert(ht, 3, &r3->hash_list);
	hash_insert(ht, 65, &r4->hash_list);

	l = hash_lookup(ht, 65, comp_record);
	if(l){
		tmp = list_entry(l, struct record, hash_list); 
		printf("%lld, %lld\n", tmp->key, tmp->value);
	} else {
		printf("doest exist\n");
	}

	hash_remove(ht, 65, comp_record);
	hash_remove(ht, 1, comp_record);

	l = hash_lookup(ht, 65, comp_record);
	if(l){
		tmp = list_entry(l, struct record, hash_list); 
		printf("%lld, %lld\n", tmp->key, tmp->value);
	} else {
		printf("doest exist\n");
	} 


	l = hash_lookup(ht, 1, comp_record);
	if(l){
		tmp = list_entry(l, struct record, hash_list); 
		printf("%lld, %lld\n", tmp->key, tmp->value);
	} else {
		printf("doest exist\n");
	}

	return 1;
}

