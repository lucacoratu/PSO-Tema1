#ifndef HASHMAP_H
#define HASHMAP_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* name;
	char* value;
} HashMapEntry;

typedef struct {
	size_t size;
	size_t capacity;
	HashMapEntry** entries;
} HashMap;

//Entry creation functions
HashMapEntry* create_entry(const char* name, const char* value);

//Hash map functions
//Inititalizes the hash map (every entry in the hash table will be NULL)
void init_hash_map(HashMapEntry** map, size_t max_size);

//Prints the hash map(from element 0 to element max_size - 1)
void print_hash_map(HashMapEntry** map, size_t max_size);

//Calculate the hashcode from the name of the HashMapEntry (the define)
int get_hash_code(const char* name, size_t max_size);

//Insert an element into the hash table
int insert_into_hash_map(HashMapEntry** map, HashMapEntry* entry, size_t max_size);

//Find an element from the hash table
HashMapEntry* find_entry(HashMapEntry** map, const char* name, size_t max_size);

//Remove an element from the hash_table
int remove_entry(HashMapEntry** map, const char* name, size_t max_size);

//Clear the hash map
void clear_hash__map(HashMapEntry** map, size_t max_size);

//Deallocate the hash map
void deallocate_hash_map(HashMapEntry** map, size_t max_size);

#endif
