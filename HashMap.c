#include "HashMap.h"
#include "Core.h"

//Entry creation functions
HashMapEntry* create_entry(const char* name, const char* value) {
	HashMapEntry* entry = (HashMapEntry*)malloc(sizeof(HashMapEntry));
	MALLOC_ASSERT(entry);
	//Check if the malloc was succesful
	if (entry != NULL) {
		//If the malloc succeeds then return the entry after copying the name and value 
		entry->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
		MALLOC_ASSERT(entry->name);
		entry->value = (char*)malloc(sizeof(char) * (strlen(value) + 1));
		MALLOC_ASSERT(entry->value);

		memccpy(entry->name, name, sizeof(char), strlen(name) + 1);
		memccpy(entry->value, value, sizeof(char), strlen(value) + 1);

		return entry;
	}

	//If the malloc fails then return a NULL pointer
	return NULL;
}


//Hashmap functions
void init_hash_map(HashMapEntry** map, size_t max_size)
{
	for (size_t i = 0; i < max_size; i++) {
		map[i] = NULL;
	}
}

void print_hash_map(HashMapEntry** map, size_t max_size)
{
	//Prints the content of the hash table
	for (size_t i = 0; i < max_size; i++) {
		if (map[i] == NULL) {
			//If there is nothing in i spot
			printf("\t%ld\t---\t%s\n", i, "NULL");
		}
		else {
			//If there is something in i spot
			printf("\t%ld\t---\tName:%s,Value:%s\n", i, map[i]->name, map[i]->value);
		}
	}
}

int get_hash_code(const char* name, size_t max_size)
{
	//A simple hash funciton to generate a random position based on the name parameter
	int hash_code = 0;
	for (size_t i = 0; i < strlen(name); i++) {
		hash_code = (hash_code * (name[i] - 'A')) + name[i];
	}
	return hash_code % max_size;
}

int insert_into_hash_map(HashMapEntry** map, HashMapEntry* entry, size_t max_size)
{
	//Inserts and element into the hash map
	//This function returns 0 if the insertion couldn't be completed
	//And returns 1 if the insertion has been successful;

	
	//Get the position the entry should be inserted
	int code = get_hash_code(entry->name, max_size);

	//If the entry is already occupied then search for the first available space and put the entry there
	if (map[code] != NULL) {
		//Search fot the first available slot
		for (size_t i = 0; i < max_size; i++) {
			//Start the list over to search the elements before the initial given code
			int new_code = (code + i) % max_size;

			//The slot is available
			if (map[new_code] == NULL) {
				//The insertion can be made
				map[new_code] = entry;
				return 1;
			}
		}

		//The hash map is full
		return 0;
	}

	//If the entry is empty then assign the entry to the slot
	map[code] = entry;

	return 1;
}

HashMapEntry* find_entry(HashMapEntry** map, const char* name, size_t max_size)
{
	//Search for the entry with the name (name)
	//If the entry is found in the hash map then return a pointer to the structure
	//Else return NULL;

	int code = get_hash_code(name, max_size);
	
	//The entry that is searched does not exist
	if (map[code] == NULL) {
		return NULL;
	}

	if (map[code] != NULL) {
		//There is an entry on the code slot but we don't know if it is the right one
		//Check if the map[code]->name == name
		if (strcmp(map[code]->name, name) == 0) {
			return map[code];
		}
		else {
			//Search the rest of the slots
			for (size_t i = 0; i < max_size; i++) {
				//Get the rest of the slots in the hash map
				int new_code = (code + i) % max_size;

				//Check if the name corresponds
				if (map[new_code] != NULL) {
					if (strcmp(map[new_code]->name, name) == 0) {
						return map[new_code];
					}
				}
			}

			//It is not found
			return NULL;
		}
	}
	//It is not found
	return NULL;
}

//Remove an element from the hash_table
int remove_entry(HashMapEntry** map, const char* name, size_t max_size) {
	int code = get_hash_code(name, max_size);
	
	//The entry that is searched does not exist
	if (map[code] == NULL) {
		return 1;
	}

	if (map[code] != NULL) {
		//There is an entry on the code slot but we don't know if it is the right one
		//Check if the map[code]->name == name
		if (strcmp(map[code]->name, name) == 0) {
			free(map[code]);
			map[code] = NULL;
			return 0;
		}
		else {
			//Search the rest of the slots
			for (size_t i = 0; i < max_size; i++) {
				//Get the rest of the slots in the hash map
				int new_code = (code + i) % max_size;

				//Check if the name corresponds
				if (map[new_code] != NULL) {
					if (strcmp(map[new_code]->name, name) == 0) {
						free(map[code]);
						map[code] = NULL;
						return 0;
					}
				}
			}

			//It is not found
			return 1;
		}
	}
	//It is not found
	return 1;
}

//Clear the hash map
void clear_hash__map(HashMapEntry** map, size_t max_size) {
	//Clears the hash map but does not delete the entries (just sets the pointers to NULL)
	for (size_t i = 0; i < max_size; i++) {
		map[i] = NULL;
	}
}

//Deallocate the hash map
void deallocate_hash_map(HashMapEntry** map, size_t max_size) {
	for (size_t i = 0; i < max_size; i++) {
		if(map[i] != NULL){
			free(map[i]->name);
			free(map[i]->value);
			free(map[i]);
		}
	}
	//free(map);
}

