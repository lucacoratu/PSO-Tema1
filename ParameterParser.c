#include "ParameterParser.h"
#include "Core.h"

//Functions
//Create a name mapping structure
NameMapping* create_name_mapping(char* option, char* name, char* mapping) {
	NameMapping* nm = (NameMapping*)malloc(sizeof(NameMapping));
	MALLOC_ASSERT(nm);
	//Initialization
	nm->option = NULL;
	nm->name = NULL;
	nm->mapping = NULL;
	
	nm->option = (char*)malloc(sizeof(char) * (strlen(option) + 1));
	MALLOC_ASSERT(nm->option);
	memccpy(nm->option, option, sizeof(char), strlen(option) + 1);

	if(name == NULL) {
		//It is an include which does not have a name and it has a mapping		
	}
	else {
		nm->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
		MALLOC_ASSERT(nm->name);
		memccpy(nm->name, name, sizeof(char), strlen(name) + 1);
	}

	if (mapping == NULL) {
		//It is a define which does not have a mapping, it only has a name
	}
	else {
		nm->mapping = (char*)malloc(sizeof(char) * (strlen(mapping) + 1));
		MALLOC_ASSERT(nm->mapping);
		memccpy(nm->mapping, mapping, sizeof(char), strlen(mapping) + 1);
	}

	return nm;
}

//Add the NameMapping stucture to the list in ParsedParameters stucture
ParsedParameters* add_name_mapping_into_parsed_parameters(NameMapping* nm, ParsedParameters* pParameters) {
	if (pParameters->size_list == 0) {
		pParameters->name_mapping_list = (NameMapping**)malloc(sizeof(NameMapping*) * (pParameters->size_list + 1));
		MALLOC_ASSERT(pParameters->name_mapping_list);

		pParameters->name_mapping_list[pParameters->size_list] = nm;
		pParameters->size_list++;
	}
	else {
		pParameters->name_mapping_list = (NameMapping**)realloc(pParameters->name_mapping_list, sizeof(NameMapping*) * (pParameters->size_list + 1));
		MALLOC_ASSERT(pParameters->name_mapping_list);
		
		pParameters->name_mapping_list[pParameters->size_list] = nm;
		pParameters->size_list++;
	}

	return pParameters;
}

int find_last_slash_or_backslash(char* string) {
	for(int i = strlen(string) - 1; i >=0; i--) {
		if(string[i] == '/' || string[i] == '\\') {
			//printf("%d ",i);
			return i;
		}
	}
	return -1;
}

//Take the list of parameters and construct the ParsedParameters based on it
ParsedParameters* parse_parameters(int argc, char** argv, HashMapEntry** map, size_t map_size) {
	//Allocate the memory for the structure
	ParsedParameters* pParameters = (ParsedParameters*)malloc(sizeof(ParsedParameters) * 1);
	MALLOC_ASSERT(pParameters);
	
	//Initialization
	pParameters->outfile = NULL;
	pParameters->infile = NULL;
	pParameters->size_list = 0;
	pParameters->name_mapping_list = NULL;
	pParameters->in_directory = NULL;

	if (pParameters != NULL) {
		//If there is no parameter then read the file content from stdin
		
		if(argc == 1) {
			//pParameters->infile = (char*)malloc(sizeof(char) * 6);
			//MALLOC_ASSERT(pParameters->infile);
			//memccpy(pParameters->infile, "stdin", sizeof(char), 6);
			return pParameters;
		}
		
		/*
		//If there is only one parameter then it should be the in File;
		if (argc >= 1) {
			pParameters->infile = (char*)malloc(sizeof(char) * (strlen(argv[1]) + 1));
			MALLOC_ASSERT(pParameters->infile);
			if (pParameters->infile != NULL) {
				memccpy(pParameters->infile, argv[1], sizeof(char), strlen(argv[1]) + 1);
				pParameters->size_list = 0;
				
				int pos = find_last_slash_or_backslash(pParameters->infile);
				if(pos != -1) {
					pParameters->in_directory = (char*)malloc(sizeof(char) * (pos + 1));
					MALLOC_ASSERT(pParameters->in_directory);
					memccpy(pParameters->in_directory, pParameters->infile, sizeof(char), pos + 1);
					//printf("In directory: %s\n", pParameters->in_directory);
					char* option = (char*)malloc(sizeof(char) * 3);
					MALLOC_ASSERT(option);
					strncpy(option, "-I", 3);
					option[2] = '\0';

					NameMapping* nm = create_name_mapping(option, NULL, pParameters->in_directory);
					pParameters = add_name_mapping_into_parsed_parameters(nm, pParameters);

					free(option);
				}
				
				return pParameters;
			}
			return NULL;
		}
		*/
		if (argc > 1) {
			//It contains multiple options not just the name of the input file

			pParameters->size_list = 0;

			//if it contains -D option then the argument where -D was found contains the name and the mapping if it is defined
			for (int i = 1; i < argc; i++) {
				if (strncmp(argv[i], "-D", 2) == 0) {
					//verify if the argument is longer than 2 characters
					//If it is then take the name and the mapping and add it to the hash table
					if (strlen(argv[i]) > 2) {
						char* name_and_mapping = argv[i] + 2;
						char* name = NULL;
						char* value = NULL;
						char* equals_sign = strchr(name_and_mapping, '=');
						if (equals_sign == NULL) {
							//It does not have a mapping 
							name = (char*)malloc(sizeof(char) * (strlen(name_and_mapping) + 1));
							MALLOC_ASSERT(name);
							memccpy(name, name_and_mapping, sizeof(char), strlen(name_and_mapping));
							
							//printf("Define no value: %s", name);

							value = (char*)malloc(sizeof(char) * 1);
							MALLOC_ASSERT(value);
							value[0] = '\0';
							

							HashMapEntry* entry = create_entry(name, value);
							int res = insert_into_hash_map(map, entry, map_size);
							if(res == 0) {
								//TO DO... When insert returns 0 it means the map is full so extend the size
								return NULL;
							}
							
							free(name);
							free(value);
						}
						//It has a mapping
						else {
							int name_length = equals_sign - name_and_mapping;
							name = (char*)malloc(sizeof(char) * (name_length + 1));
							MALLOC_ASSERT(name);
							memccpy(name, name_and_mapping, sizeof(char), name_length);
							name[name_length] = '\0';

							value = (char*)malloc(sizeof(char) * (strlen(equals_sign + 1) + 1));
							MALLOC_ASSERT(value);
							memccpy(value, equals_sign + 1, sizeof(char), strlen(equals_sign + 1) + 1);
						
							HashMapEntry* entry = create_entry(name, value);
							int res = insert_into_hash_map(map, entry, map_size);
							if(res == 0){
								//TO DO... When insert returns 0 it means the map is full so extend the size
								return NULL;
							}
							
							free(name);
							free(value);
						}
					}
					//Else take the next argument
					else {
						char* name_and_mapping = argv[i+1];
						char* name = NULL;
						char* value = NULL;
						char* equals_sign = strchr(name_and_mapping, '=');
						if (equals_sign == NULL) {
							//It does not have a mapping
							name = (char*)malloc(sizeof(char) * (strlen(name_and_mapping) + 1));
							MALLOC_ASSERT(name);
							memccpy(name, name_and_mapping, sizeof(char), strlen(name_and_mapping));

							value = (char*)malloc(sizeof(char) * 1);
							MALLOC_ASSERT(value);
							value[0] = '\0';

							HashMapEntry* entry = create_entry(name, value);
							int res = insert_into_hash_map(map, entry, map_size);

							if(res == 0){
								//TO DO... When insert returns 0 it means the map is full so extend the size
								return NULL;
							}
							
							free(name);
							free(value);
							//Skip the next parameter
							i++;
						}
						//It has a mapping
						else {
							int name_length = equals_sign - name_and_mapping;
							name = (char*)malloc(sizeof(char) * (name_length + 1));
							MALLOC_ASSERT(name);
							memccpy(name, name_and_mapping, sizeof(char), name_length);
							name[name_length] = '\0';

							value = (char*)malloc(sizeof(char) * (strlen(equals_sign + 1) + 1));
							MALLOC_ASSERT(value);
							memccpy(value, equals_sign + 1, sizeof(char), strlen(equals_sign + 1) + 1);

							HashMapEntry* entry = create_entry(name, value);
							int res = insert_into_hash_map(map, entry, map_size);
							
							if(res == 0){
								//TO DO... When insert returns 0 it means the map is full so extend the size
								return NULL;
							}
							
							free(name);
							free(value);
							
							//Skip the next parameter
							i++;
						}
					}
					continue;
				}
				//Add the output file to the ParsedParameters structure 
				if (strncmp(argv[i], "-o", 2) == 0) {
					//It has the name of the file next to -o
					if (strlen(argv[i]) > 2) {
						pParameters->outfile = NULL;
						char* filename = argv[i] + 2;
						pParameters->outfile = (char*)malloc(sizeof(char) * (strlen(filename) + 1));
						MALLOC_ASSERT(pParameters->outfile);
						
						memccpy(pParameters->outfile, filename, sizeof(char), strlen(filename) + 1);
					}
					//The name of the output file is in the next argument
					else {
						pParameters->outfile = NULL;
						pParameters->outfile = (char*)malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
						MALLOC_ASSERT(pParameters->outfile);
						memccpy(pParameters->outfile, argv[i + 1], sizeof(char), strlen(argv[i + 1]) + 1);
						
						//Skip the next parameter
						i++;
					}

					continue;
				}

				if (strncmp(argv[i], "-I", 2) == 0) {
					//The include directories
					if(strlen(argv[i]) > 2) {
						//It contains the directory that wants to be included in the same argument
						char* mapping = argv[i] + 2;
						char* option = (char*)malloc(sizeof(char) * 4);
						MALLOC_ASSERT(option);
						memccpy(option, argv[i], sizeof(char), 2);
						option[2] = '\0';

						NameMapping* nm = create_name_mapping(option, NULL, mapping);
						pParameters = add_name_mapping_into_parsed_parameters(nm, pParameters);
					
						free(option);
					}
					else {
						//Take the directory from the next argument
						char* mapping = argv[i + 1];
						char* option = (char*)malloc(sizeof(char) * 4);
						MALLOC_ASSERT(option);
						memccpy(option, argv[i], sizeof(char), 2);
						option[2] = '\0';

						NameMapping* nm = create_name_mapping(option, NULL, mapping);
						pParameters = add_name_mapping_into_parsed_parameters(nm, pParameters);

						//Skip the next argument
						free(option);
						i++;
					}

					continue;
				}
				
				if(strncmp(argv[i],"-",1) == 0){
					//Invalid option given to the program
					return NULL;
				}

				//If does not have any option before so it must be the infile
				//printf("Infile!\n");
				free(pParameters->infile);
				
				pParameters->infile = (char*)malloc(sizeof(char) * (strlen(argv[i]) + 1));
				MALLOC_ASSERT(pParameters->infile);
				memccpy(pParameters->infile, argv[i], sizeof(char), strlen(argv[i]) + 1);
				int pos = find_last_slash_or_backslash(pParameters->infile);
				if(pos != -1) {
					pParameters->in_directory = (char*)malloc(sizeof(char) * (pos + 1));
					MALLOC_ASSERT(pParameters->in_directory);
					memccpy(pParameters->in_directory, pParameters->infile, sizeof(char), pos + 1);
					//printf("In directory: %s\n", pParameters->in_directory);
					char* option = (char*)malloc(sizeof(char) * 3);
					MALLOC_ASSERT(option);
					strncpy(option, "-I", 3);
					option[2] = '\0';

					NameMapping* nm = create_name_mapping(option, NULL, pParameters->in_directory);
					pParameters = add_name_mapping_into_parsed_parameters(nm, pParameters);

					free(option);
				}
			}

			return pParameters;
		}
	}
	
	return NULL;

}

//Veryfies if the parameters given to the application are correct
int verify_parsed_parameters(ParsedParameters* pParameters) {
	if (pParameters->size_list == 0) {
		//Only the name of the input file has been given
		//Check if the file exists
		//If it exists then return 1;
		//Else return 0;
		FILE* file;
		file = fopen(pParameters->infile, "r");
		if (file == NULL)
			return 0;
		fclose(file);
		return 1;
	}
	else {
		FILE* file;
		file = fopen(pParameters->infile, "r");
		if (file == NULL)
			return 0;
		fclose(file);
		return 1;
	}
	
	//Check if the infile is set to NULL in the parsed parameters structure
	return 0;
}

//Print the NameMapping structure
void print_name_mapping(NameMapping* nm) {
	printf("%s\n", nm->option);

	if(nm->name != NULL)
		printf("%s\n", nm->name);
	if(nm->mapping != NULL)
		printf("%s\n", nm->mapping);
}

//Print function...for debug only
void print_parsed_parameters_structure(ParsedParameters* pParameters) {
	if (pParameters != NULL) {
		printf("In file: %s\n", pParameters->infile);
		if(pParameters->outfile != NULL) {
			printf("Out file: %s\n", pParameters->outfile);
		}
		for (size_t i = 0; i < pParameters->size_list; i++) {
			print_name_mapping(pParameters->name_mapping_list[i]);
		}
	}

}

//Deallocate the name mapping structure
void deallocate_name_mapping(NameMapping* nm) {
	if(nm != NULL) {
		if(nm->option != NULL) {
			free(nm->option);
		}
		if(nm->name != NULL) {
			free(nm->name);
		}
		if(nm->mapping != NULL) {
			free(nm->mapping);
		}
		free(nm);
	}
}

//Deallocate the memory used by the structure
void deallocate_structure_memory(ParsedParameters* pParameters) {
	if(pParameters != NULL) {
		if(pParameters->infile != NULL)
			free(pParameters->infile);
		if(pParameters->in_directory != NULL)
			free(pParameters->in_directory);
		if(pParameters->outfile != NULL)
			free(pParameters->outfile);
			
		for(size_t i =0; i < pParameters->size_list; i++) {
			deallocate_name_mapping(pParameters->name_mapping_list[i]);
		}
		
		free(pParameters);
	}
}


