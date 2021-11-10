#include "FileParser.h"
#include "Core.h"

//Gets all the content from a file
char* get_file_content(const char* filename)
{
	//Takes all of the bytes and returns them if the file could be opened
	//Else return NULL;
	FILE* file;
	file = fopen(filename, "r");
	if (file == NULL){
		//printf("Cannot locate file: %s\n", filename);
		return NULL;
	}
	//The string that will be returned
	char* return_content = NULL;

	//Get the number of bytes in the file
	long number_bytes = 0;
	
	fseek(file, 0, SEEK_END);
	number_bytes = ftell(file);
	fseek(file, 0, SEEK_SET);

	//Allocate the space needed for the content
	return_content = (char*)malloc(sizeof(char) * (number_bytes + 1));

	//Check if the malloc has been successful
	if (return_content == NULL) {
		//Bad memory allocation
		return NULL;
	}
	
	//Read the content of the file
	size_t bytes_read = fread(return_content, sizeof(char), number_bytes, file);
	return_content[bytes_read] = '\0';

	/*if (bytes_read != number_bytes) {
		//The read did not complete successfully
		return NULL;
	}*/

	//Close the file
	fclose(file);

	return return_content;
}

//Concatenates two strings and returns a new string
char* concatenate_strings(const char* string1,const char* string2) {
	char* new_string = (char*)malloc(sizeof(char) * (strlen(string1) + strlen(string2) + 1));
	MALLOC_ASSERT(new_string);
	memccpy(new_string, string1, sizeof(char), strlen(string1));
	memccpy(new_string + strlen(string1), string2, sizeof(char), strlen(string2) + 1);

	return new_string;
}

//Print the lines in the ParsedFile structure
void print_lines(ParsedFile* pFile, int debug) {
	for (size_t i = 0; i < pFile->noLines; i++) {
		if(i != pFile->noLines) {
			if(debug == 1)
				printf("Line[%ld]: %s\n", i, pFile->lines[i]);
			else
				printf("%s\n", pFile->lines[i]);
		}
		else {
			if(debug == 1)
				printf("Line[%ld]: %s", i, pFile->lines[i]);
			else
				printf("%s",pFile->lines[i]);
		}
	}
}

//Initialize the ParsedFile struct with the filename
ParsedFile* initialize_pfile_struct(const char* filename) {
	//Prepare the struct that will be returned
	ParsedFile* pFile = (ParsedFile*)malloc(sizeof(ParsedFile));
	MALLOC_ASSERT(pFile);
	
	pFile->filename = NULL;
	pFile->lines = NULL;
	pFile->noLines = 0;

	//If the memory has been alocated
	if (pFile != NULL) {
		if(pFile->filename != NULL) {
			pFile->filename = (char*)malloc(sizeof(char) * (strlen(filename) + 1));
			MALLOC_ASSERT(pFile->filename);
			memccpy(pFile->filename, filename, sizeof(char), strlen(filename) + 1);
		}
		else
			pFile->filename = NULL;
			
		pFile->lines = NULL;
		pFile->noLines = 0;
	}
	else {
		return NULL;
	}

	return pFile;
}

//Break a string into lines
ParsedFile* break_to_lines(char* content){
	if(content == NULL)
		return NULL;
		
	//Inititalize the struct
	ParsedFile* pFile = initialize_pfile_struct("stdin");

	//A NULL memory that can be expanded
	char** lines = (char**)calloc(0, sizeof(char*));
	MALLOC_ASSERT(lines);

	int line_count = 0;
	//Put every line in the lines char matrix
	int char_on_line = 0;
	char* current_position = content;
	char* start_of_line = content;
	while (*current_position != '\0') {
		if (*current_position == '\n') {
			//If it is the end of the line
			//Realloc the memory for the lines pointer
			lines = (char**)realloc(lines, sizeof(char*) * (line_count + 1));
			MALLOC_ASSERT(lines);

			lines[line_count] = (char*)malloc(sizeof(char) * (char_on_line + 1));
			MALLOC_ASSERT(lines[line_count]);
			
			if (lines != NULL) {
				//Copy the content of the line in the coresponding current line position
				memccpy(lines[line_count], start_of_line, sizeof(char), (current_position - start_of_line));

				//Put the '\0' value at the end of the string
				lines[line_count][char_on_line] = '\0';
			}


			//Reset the number of characters in the line
			char_on_line = 0;
			current_position++;
			start_of_line = current_position;
			line_count++;

			continue;
		}
		char_on_line++;
		current_position++;
	}
	//Copy the last line
	lines = (char**)realloc(lines, sizeof(char*) * (line_count + 1));
	MALLOC_ASSERT(lines);
	lines[line_count] = (char*)malloc(sizeof(char) * (char_on_line + 1));
	MALLOC_ASSERT(lines[line_count]);

	//Copy the content of the line in the coresponding current line position
	memccpy(lines[line_count], start_of_line, sizeof(char), (current_position - start_of_line));

	//Put the '\0' value at the end of the string
	lines[line_count][char_on_line] = '\0';

	//Set the number of lines
	pFile->noLines = line_count + 1;
	pFile->lines = lines;

	//Delete the initial content holder
	free(content);

	return pFile;
}

//Breaks the content into lines
ParsedFile* break_into_lines(const char* filename) {
	//Get the content of the file
	char* content = get_file_content(filename);

	//Check if the file was found
	if (content == NULL) {
		return NULL;
	}

	//Inititalize the struct
	ParsedFile* pFile = initialize_pfile_struct(filename);

	//A NULL memory that can be expanded
	char** lines = (char**)calloc(0, sizeof(char*));
	MALLOC_ASSERT(lines);

	int line_count = 0;
	//Put every line in the lines char matrix
	int char_on_line = 0;
	char* current_position = content;
	char* start_of_line = content;
	while (*current_position != '\0') {
		if (*current_position == '\n') {
			//If it is the end of the line
			//Realloc the memory for the lines pointer
			lines = (char**)realloc(lines, sizeof(char*) * (line_count + 1));
			MALLOC_ASSERT(lines);

			lines[line_count] = (char*)malloc(sizeof(char) * (char_on_line + 1));
			MALLOC_ASSERT(lines[line_count]);
			
			if (lines != NULL) {
				//Copy the content of the line in the coresponding current line position
				memccpy(lines[line_count], start_of_line, sizeof(char), (current_position - start_of_line));

				//Put the '\0' value at the end of the string
				lines[line_count][char_on_line] = '\0';
			}


			//Reset the number of characters in the line
			char_on_line = 0;
			current_position++;
			start_of_line = current_position;
			line_count++;

			continue;
		}
		char_on_line++;
		current_position++;
	}
	//Copy the last line
	lines = (char**)realloc(lines, sizeof(char*) * (line_count + 1));
	MALLOC_ASSERT(lines);
	lines[line_count] = (char*)malloc(sizeof(char) * (char_on_line + 1));
	MALLOC_ASSERT(lines[line_count]);

	//Copy the content of the line in the coresponding current line position
	memccpy(lines[line_count], start_of_line, sizeof(char), (current_position - start_of_line));

	//Put the '\0' value at the end of the string
	lines[line_count][char_on_line] = '\0';

	//Set the number of lines
	pFile->noLines = line_count + 1;
	pFile->lines = lines;

	//Delete the initial content holder
	free(content);

	return pFile;
}

//Frees the memory allocated in the parsed file pointer
void clear_parsed_pointer(ParsedFile* pFile) {
	if (pFile != NULL) {
		for (size_t i = 0; i < pFile->noLines; i++) {
			if(pFile->lines[i] != NULL)
				free(pFile->lines[i]);
		}
		if(pFile->lines != NULL)
			free(pFile->lines);
		if(pFile->filename != NULL)
			free(pFile->filename);
		free(pFile);
	}
	
}

//Removes the line specified in the parameter
ParsedFile* remove_line(ParsedFile* pFile, size_t line_index) {
	ParsedFile* aux = (ParsedFile*)malloc(sizeof(ParsedFile));
	MALLOC_ASSERT(aux);
	
	//Initializations
	aux->filename = NULL;

	aux->lines = (char**)malloc(sizeof(char*) * (pFile->noLines - 1));
	MALLOC_ASSERT(aux->lines);
	
	aux->noLines = pFile->noLines - 1;
	
	if(pFile->filename != NULL){
		aux->filename = (char*)malloc(sizeof(char) * (strlen(pFile->filename) + 1));
		MALLOC_ASSERT(aux->filename);
	}
	else
		aux->filename = NULL;
	
	if(pFile->filename != NULL)
		memccpy(aux->filename, pFile->filename, sizeof(char), strlen(pFile->filename) + 1);

	int count = 0;
	for (size_t i = 0; i < pFile->noLines; i++) {
		if (i != line_index) {
			aux->lines[count] = (char*)malloc(sizeof(char) * (strlen(pFile->lines[i]) + 1));
			MALLOC_ASSERT(aux->lines[count]);

			memccpy(aux->lines[count], pFile->lines[i], sizeof(char), (strlen(pFile->lines[i]) + 1));
			count++;
		}
	}

	clear_parsed_pointer(pFile);
	return aux;
}

//Adds the lines to the back of the parsed file pointer
ParsedFile* add_lines_back(ParsedFile* pFile, ParsedFile* pFileHeader) {
	//Reserve the space needed for the lines
	ParsedFile* aux = initialize_pfile_struct(NULL);
	if (pFile != NULL) {
		aux->lines = (char**)malloc(sizeof(char*) * (pFile->noLines + pFileHeader->noLines));
		MALLOC_ASSERT(aux->lines);
		aux->noLines = (pFile->noLines + pFileHeader->noLines);
		if(pFile->filename != NULL) {
			aux->filename = (char*)malloc(sizeof(char) * (strlen(pFile->filename)));
			MALLOC_ASSERT(aux->filename);
		}
		else
			aux->filename = NULL;
	}
	else {
		aux->lines = (char**)malloc(sizeof(char*) * (pFileHeader->noLines));
		MALLOC_ASSERT(aux->lines);
		aux->noLines = (pFileHeader->noLines);
		aux->filename = (char*)malloc(sizeof(char) * 10);
		MALLOC_ASSERT(aux->filename);
	}

	//Copy the lines from the main file
	if (pFile != NULL) {
		for (size_t i = 0; i < pFile->noLines; i++) {
			aux->lines[i] = (char*)malloc(sizeof(char) * (strlen(pFile->lines[i]) + 1));
			MALLOC_ASSERT(aux->lines[i]);
			memccpy(aux->lines[i], pFile->lines[i], sizeof(char), strlen(pFile->lines[i]) + 1);
		}
	}

	//Copy the lines from header file
	if (pFile != NULL) {
		for (size_t i = pFile->noLines; i < pFile->noLines + pFileHeader->noLines; i++) {
			aux->lines[i] = (char*)malloc(sizeof(char) * (strlen(pFileHeader->lines[i - pFile->noLines]) + 1));
			MALLOC_ASSERT(aux->lines[i]);
			memccpy(aux->lines[i], pFileHeader->lines[i - pFile->noLines], sizeof(char), strlen(pFileHeader->lines[i - pFile->noLines]) + 1);
		}
	}
	else {
		for (size_t i = 0; i < pFileHeader->noLines; i++) {
			aux->lines[i] = (char*)malloc(sizeof(char) * (strlen(pFileHeader->lines[i]) + 1));
			MALLOC_ASSERT(aux->lines[i]);
			memccpy(aux->lines[i], pFileHeader->lines[i], sizeof(char), strlen(pFileHeader->lines[i]) + 1);
		}
	}

	clear_parsed_pointer(pFile);
	clear_parsed_pointer(pFileHeader);
	return aux;
}

//Adds the content of the include file if the file exists
ParsedFile* add_includes(ParsedFile* pFile, ParsedParameters* pParameters) {
	//Go through every line till you find int main
	//If the line starts with #include "...
	//Take the headername, get it's content, parse the file and concat the lines corresponding to the header with the lines in the current file
	//Remove the line that contantains the include
	ParsedFile* result = initialize_pfile_struct(NULL);
	for (size_t i = 0; i < pFile->noLines; i++) {
		if (strlen(pFile->lines[i]) >= 10) {
			if (strncmp(pFile->lines[i], "#include \"", 10) == 0) {
				//Include has been found
				int header_name_length = strlen(pFile->lines[i]) - 11;
				char* include_name = (char*)malloc(sizeof(char) * header_name_length + 1); 
				MALLOC_ASSERT(include_name);
				memccpy(include_name, pFile->lines[i] + 10, sizeof(char), header_name_length);
				include_name[header_name_length] = '\0';

				//Remove the current line
				pFile = remove_line(pFile, i);

				//First check if the file exists in the current directory
				//If it is in the current directory then add it to the infile
				//Else search it in the other include directories specified by the -I option

				//Get the header file content
				ParsedFile* hFile = break_into_lines(include_name);
				if (hFile != NULL) {
					//printf("Added header: %s\n", include_name);

					ParsedFile* hFileAfterIncludes = add_includes(hFile, pParameters);

					result = add_lines_back(result, hFileAfterIncludes);
					
					free(include_name);
					//clear_parsed_pointer(hFile);
					//clear_parsed_pointer(hFileAfterIncludes);
				}
				else {
					int found = 0;
					for (size_t j = 0; j < pParameters->size_list; j++) {
						char* concat_result = concatenate_strings(pParameters->name_mapping_list[j]->mapping, "/");
						char* concat_result2 = concatenate_strings(concat_result, include_name);

						

						//DEBUG...Remove later
						//printf("Concat result: %s\n", concat_result);
						
						//Check if the file is in the aditional include directory
						ParsedFile* hFile = break_into_lines(concat_result2);
						if (hFile != NULL) {
							//The header file has been found
							found = 1;
							//printf("Added header: %s\n", concat_result);
							ParsedFile* hFileAfterIncludes = add_includes(hFile, pParameters);
							result = add_lines_back(result, hFileAfterIncludes);
						
						}

						//Free the memory allocated by the concatenate_strings function
						free(concat_result);
						free(concat_result2);
					}

					free(include_name);

					if (found == 0) {
						//Return with an error
						exit(-1);
					}
				}
			}
		}
	}

	//Add the lines from the main file
	result = add_lines_back(result, pFile);
	
	//clear_parsed_pointer(pFile);
	return result;
}

//Breaks the string into tokens
char** break_string_into_tokens(char* string, int* length, const char delim) {
	//A NULL memory that can be expanded
	char** lines = (char**)calloc(0, sizeof(char*));

	int line_count = 0;
	//Put every line in the lines char matrix
	int char_on_line = 0;
	char* current_position = string;
	char* start_of_line = string;
	while (*current_position != '\0') {
		if (*current_position == delim) {
			//If it is the end of the line
			//Realloc the memory for the lines pointer
			lines = (char**)realloc(lines, sizeof(char*) * (line_count + 1));
			MALLOC_ASSERT(lines);
			lines[line_count] = (char*)malloc(sizeof(char) * (char_on_line + 1));
			MALLOC_ASSERT(lines[line_count]);
			
			if (lines != NULL) {
				//Copy the content of the line in the coresponding current line position
				memccpy(lines[line_count], start_of_line, sizeof(char), (current_position - start_of_line));

				//Put the '\0' value at the end of the string
				lines[line_count][char_on_line] = '\0';
			}
			else {
				//The realloc failed which means the program should exit
				return NULL;
			}

			//Reset the number of characters in the line
			char_on_line = 0;
			current_position++;
			start_of_line = current_position;
			line_count++;

			continue;
		}
		char_on_line++;
		current_position++;
	}
	//Copy the last line
	lines = (char**)realloc(lines, sizeof(char*) * (line_count + 1));
	MALLOC_ASSERT(lines);
	lines[line_count] = (char*)malloc(sizeof(char) * (char_on_line + 1));
	MALLOC_ASSERT(lines[line_count]);

	//Copy the content of the line in the coresponding current line position
	memccpy(lines[line_count], start_of_line, sizeof(char), (current_position - start_of_line));

	//Put the '\0' value at the end of the string
	lines[line_count][char_on_line] = '\0';

	//Delete the initial content holder
	*length = line_count + 1;
	return lines;
}

//Checks to see if the define has another define name in the value
char* value_contains_other_define(HashMapEntry** map, size_t map_size, char* value) {
	//Go through the not null elements of the hash map
	//Check if the name can be found inside the value string
	//If yes then interchange it with the value and let the rest unchanged

	for (size_t i = 0; i < map_size; i++) {
		if (map[i] != NULL) {
			char* beg = strstr(value, map[i]->name);
			while(beg != NULL) {
				//The name has been found
				char* rest = (char*)malloc(sizeof(char) * (strlen(beg + strlen(map[i]->name)) + 1));
				MALLOC_ASSERT(rest);
				if (rest != NULL) {
					memccpy(rest, beg + strlen(map[i]->name), sizeof(char), strlen(beg + strlen(map[i]->name)) + 1);
					memccpy(beg, map[i]->value, sizeof(char), strlen(map[i]->value) + 1);
					memccpy(beg + strlen(beg), rest, sizeof(char), strlen(rest) + 1);
					//printf("Beg: %s",beg);
					
					free(rest);
				}

				beg += strlen(map[i]->value);
				beg = strstr(beg, map[i]->name);
			}
		}
	}

	return value;
}

//Loads the hashmap with all the defines that are not inside an #if statement
ParsedFile* load_simple_defines(HashMapEntry** map, size_t map_size, ParsedFile* pFile) {
	for (size_t i = 0; i < pFile->noLines; i++) {
		//Skip the lines until the last #endif if #if was found
		if (strlen(pFile->lines[i]) >= 3) {
			if (strncmp(pFile->lines[i], "#if", 3) == 0) {
				int if_count = 1;
				while (if_count != 0) {
					if(strncmp(pFile->lines[i], "#if", 6) == 0)
						if_count++;
					if(strncmp(pFile->lines[i], "#endif", 6) != 0)
						if_count--;
					i++;
				}
			}
		}
		//Check if the line contains a #define
		if (strlen(pFile->lines[i]) >= 7) {
			if (strncmp(pFile->lines[i], "#define", 7) == 0) {
				//If it starts with the define create an entry with the name and value
				//Then add the entry to the hash table
				//And remove the line with the #define
				char* name = NULL;
				char* value = NULL;
				char* start = strstr(pFile->lines[i], "#define");

				char* space = strchr(start + 8, ' ');
				if (space != NULL) {
					int name_length = space - start - 8;
					name = (char*)malloc(sizeof(char) * (name_length + 1));
					MALLOC_ASSERT(name);
					memccpy(name, (start + 8), sizeof(char), name_length);
					name[name_length] = '\0';
					value = (char*)malloc(sizeof(char) * (strlen(space + 1) + 1));
					MALLOC_ASSERT(value);
					memccpy(value, (space + 1), sizeof(char), strlen(space + 1) + 1);

					pFile = remove_line(pFile, i);

					value = value_contains_other_define(map, map_size, value);
					
				}
				else {
					//Take the name and let the value be an empty string
					int name_length = strlen(start + 8);
					name = (char*)malloc(sizeof(char) * (name_length + 1));
					MALLOC_ASSERT(name);
					memccpy(name, (start + 8), sizeof(char), name_length);
					name[name_length] = '\0';

					value = (char*)malloc(sizeof(char) * 1);
					MALLOC_ASSERT(value);
					value[0] = '\0';

					pFile = remove_line(pFile, i);
				}

				//DEBUG
				//printf("Name: %s - Value: %s\n", name, value);
				HashMapEntry* entry = create_entry(name, value);
				insert_into_hash_map(map, entry, map_size);
				
				free(value);
				free(name);
				
				i--;
			}
		}
	}

	return pFile;
}

//Checks if the found is inside double quotes
int check_name_inside_dquotes(char* string, char* found) {
	int dquotes_count = 0;
	//printf("In check_name_double");
	while (string != found) {
		if (*string == '\"' ) {
			dquotes_count++;
		}
		string++;
	}
	//printf("Am iesit din check_name_double");
	//If there is an even number of double quotes before the start postion of the string we are looking for
	//Then it is outside double quotes
	if (dquotes_count % 2 == 0) {
		return 0;
	}
	//Else it is inside
	return 1;
}

//Undef the variables
ParsedFile* undef_defines(HashMapEntry** map, size_t map_size, ParsedFile* pFile) {
	for(size_t i =0;i < pFile->noLines; i++) {
		//If any if statement is found then skip the lines containing it
		if (strlen(pFile->lines[i]) >= 3) {
			if (strncmp(pFile->lines[i], "#if", 3) == 0) {
				int if_count = 1;
				while (if_count != 0) {
					if(strncmp(pFile->lines[i], "#if", 6) == 0)
						if_count++;
					if(strncmp(pFile->lines[i], "#endif", 6) != 0)
						if_count--;
					i++;
				}
			}
		}
	
		if(strncmp(pFile->lines[i],"#undef", 6) == 0) {
			//Extract the name to be undefined
			char* name = pFile->lines[i] + 7;
			remove_entry(map, name, map_size);
			
			//Remove the #undef from the lines
			pFile = remove_line(pFile,i);
		}
	}
	
	return pFile;
}

//Change the defines names with their values
ParsedFile* replace_define_names_with_values(HashMapEntry** map, size_t map_size, ParsedFile* pFile) {
	//Search every line in the pFile
	for (size_t i = 0; i < pFile->noLines; i++) {
		if (strlen(pFile->lines[i]) > 0) {
			//The line is not empty
			
			//Check if any undef has been found
			if(strncmp(pFile->lines[i], "#undef", 6) == 0) {
				char* name = pFile->lines[i] + 7;
				//printf("Undef name: %s\n", name);
				int res = remove_entry(map, name, map_size);
				if(res == 0) {
					pFile = remove_line(pFile, i);
				}
			}
			
			//print_hash_map(map, map_size);
			
			//Check if the line contains a define name
			for (size_t j = 0; j < map_size; j++) {
				if (map[j] != NULL) {
						char* beg = strstr(pFile->lines[i], map[j]->name);
						//It contains the define name
						//Check if the string is inside double quotes
						//printf("In replace_define_names");
						while (beg != NULL) {
							if (check_name_inside_dquotes(pFile->lines[i], beg) == 0) {
								//It is no between double quotes
								//Replace the name with the value
								char* rest = (char*)malloc(sizeof(char) * (strlen(beg + strlen(map[j]->name)) + 1));
								MALLOC_ASSERT(rest);
								if (rest != NULL) {
									memccpy(rest, beg + strlen(map[j]->name), sizeof(char), strlen(beg + strlen(map[j]->name)) + 1);
									
									if(strlen(map[j]->value) != 0 && strlen(map[j]->value) > strlen(map[j]->name)) {
										pFile->lines[i] = (char*)realloc(pFile->lines[i], sizeof(char) * (beg - pFile->lines[i] + strlen(map[j]->value) + strlen(rest) + 1));
										MALLOC_ASSERT(pFile->lines[i]);

										beg = strstr(pFile->lines[i], map[j]->name);
									}

									if(strlen(map[j]->value) != 0){
										memccpy(beg, map[j]->value, sizeof(char), strlen(map[j]->value) + 1);
										memccpy(beg + strlen(beg), rest, sizeof(char), strlen(rest) + 1);
									}
									else {
										memccpy(beg, rest, sizeof(char), strlen(rest) + 1);
									}
									//printf("beg: %s",beg);
									
									free(rest);
								}
							}
							
							if(strlen(map[j]->value) != 0)
								beg += strlen(map[j]->value);
							else
								beg += 1;
								
							if(beg != NULL)
								beg = strstr(beg, map[j]->name);
							else
								break;
						}

				}
			}
		}
	}

	return pFile;
}

//Loads the code inside if if the condition is met
ParsedFile* load_ifdefstatements_true(HashMapEntry** map, size_t map_size, ParsedFile* pFile) {
	for (size_t i = 0; i < pFile->noLines; i++) {
		if (strlen(pFile->lines[i]) > 6) {
			if (strncmp(pFile->lines[i], "#ifdef", 6) == 0) {
				//It contains #ifdef
				//Go through all of the entries in the hash table
				//If the name is after the #ifdef then add the lines to the code adn remove the #ifdef line
				char* symbol = pFile->lines[i] + 7;
				int found = 0;

				int number_ifdefs = 0;
				for (size_t j = 0; j < map_size; j++) {
					HashMapEntry* entry = find_entry(map, symbol, map_size);
					if (entry != NULL) {
						//It is found
						found = 1;
						pFile = remove_line(pFile, i);

						int line_count = i;
						
						while (number_ifdefs != 0) {
							if (strncmp(pFile->lines[line_count], "#ifndef", 7) == 0 || strncmp(pFile->lines[line_count], "#ifdef", 6) == 0) {
								number_ifdefs++;
							}
							if (strncmp(pFile->lines[line_count], "#endif", 6) == 0) {
								number_ifdefs--;
							}
							line_count++;
						}
						
						while (strncmp(pFile->lines[line_count], "#else", 5) != 0 && strncmp(pFile->lines[line_count], "#endif", 6) != 0) {
							line_count++;
						}

						if (strncmp(pFile->lines[line_count], "#else", 5) == 0) {
							while (strncmp(pFile->lines[line_count], "#endif", 6) != 0) {
								pFile = remove_line(pFile, line_count);
							}
						}

						if (strncmp(pFile->lines[line_count], "#endif", 6) == 0) {
							pFile = remove_line(pFile, line_count);
						}

						break;
					}
				}
				if(found == 0) {
					//If the else statement exists do not remove that part
					int line_count = i;
					while (strncmp(pFile->lines[line_count], "#endif", 6) != 0 && strncmp(pFile->lines[line_count], "#else", 5) != 0) {
						pFile = remove_line(pFile, line_count);
					}
					int has_else = 0;
					if (strncmp(pFile->lines[line_count], "#else", 5) == 0) {
						has_else = 1;
					}
					pFile = remove_line(pFile, line_count);

					if (has_else == 1) {
						while (strncmp(pFile->lines[line_count], "#endif", 6) != 0) {
							line_count++;
						}

						pFile = remove_line(pFile, line_count);
					}
				}

				continue;
			}
		}
		if (strlen(pFile->lines[i]) > 7) {
			if (strncmp(pFile->lines[i], "#ifndef", 7) == 0) {
				//It contains #ifndef
				char* symbol = pFile->lines[i] + 8;
				int found = 0;
				int number_ifndefs = 1;
				for (size_t j = 0; j < map_size; j++) {
					HashMapEntry* entry = find_entry(map, symbol, map_size);
					if (entry == NULL) {
						//It is found
						found = 1;
						pFile = remove_line(pFile, i);

						int line_count = i;
						
						//Skip all of the lines till you find the last #else or the last #endif
						//Which means count everytime you find a 
						while (number_ifndefs != 0) {
							if (strncmp(pFile->lines[line_count], "#ifndef", 7) == 0 || strncmp(pFile->lines[line_count], "#ifdef", 6) == 0) {
								number_ifndefs++;
							}
							if (strncmp(pFile->lines[line_count], "#endif", 6) == 0) {
								number_ifndefs--;
							}
							line_count++;
						}

						line_count--;

						while (strncmp(pFile->lines[line_count], "#else", 5) != 0 && strncmp(pFile->lines[line_count], "#endif", 6) != 0) {
							pFile = remove_line(pFile, line_count);
						}

						if (strncmp(pFile->lines[line_count], "#else", 5) == 0) {
							pFile = remove_line(pFile, line_count);
							while (strncmp(pFile->lines[line_count], "#endif", 6) != 0) {
								pFile = remove_line(pFile, line_count);
							}
						}

						if (strncmp(pFile->lines[line_count], "#endif", 6) == 0) {
							pFile = remove_line(pFile, line_count);
						}

						//pFile = load_ifdefstatements_true(map, map_size, pFile);

						break;
					}
				}

				if (found == 0) {
					//If the else statement exists remove that part
					int line_count = i;
					pFile = remove_line(pFile, line_count);

					while (strncmp(pFile->lines[line_count], "#endif", 6) != 0 && strncmp(pFile->lines[line_count], "#else", 5) != 0) {
						line_count++;
					}
					int has_else = 0;
					if (strncmp(pFile->lines[line_count], "#else", 5) == 0) {
						has_else = 1;
					}
					pFile = remove_line(pFile, line_count);

					if (has_else == 1) {
						while (strncmp(pFile->lines[line_count], "#endif", 6) != 0) {
							pFile = remove_line(pFile, line_count);
						}
						pFile = remove_line(pFile, line_count);
					}
					
					//DEBUG...Remove later
					/*for (size_t i = 0; i < pFile->noLines; i++) {
						printf("Line[%d]: %s\n", i, pFile->lines[i]);
					}
					*/
				}

				continue;
			}
		}
	}
	return pFile;
}

//Loads the code inside #if directive if the condition is a value not equal to 0
ParsedFile* load_ifstatements_true(ParsedFile* pFile) {
	//First find the begging #if line
	for (size_t i = 0; i < pFile->noLines; i++) {
		if (strlen(pFile->lines[i]) > 3) {
			if (strncmp(pFile->lines[i], "#if", 3) == 0 && strncmp(pFile->lines[i],"#ifdef",6) != 0 && strncmp(pFile->lines[i], "#ifndef", 7) != 0) {
				//The first #if has been found on line i
				//Check the condition and convert it into a integer
				//If the integer is not 0
				//Take all of the lines until #else is found or #endif
				char* cond = pFile->lines[i] + 4;
				int condition_integer = atoi(cond);
				if (condition_integer != 0) {
					int line_count = i;
					//TODO...Solve imbricated #ifs (the same as in ifdefs/ifndefs)
					pFile = remove_line(pFile, line_count);
					
					while (strncmp(pFile->lines[line_count], "#endif", 6) != 0 && strncmp(pFile->lines[line_count], "#else", 5) != 0 && strncmp(pFile->lines[line_count], "#elif", 5) != 0) {
						line_count++;
					}

					//If the statement has a #elif then remove everything till the last #endif
					int if_count = 0;
					if (strncmp(pFile->lines[line_count], "#elif", 5) == 0 || strncmp(pFile->lines[line_count], "#else", 5) == 0) {
						if_count++;
						pFile = remove_line(pFile, line_count);
						while (if_count != 0) {
							if (strncmp(pFile->lines[line_count], "#endif", 6) == 0) {
								if_count--;
							}
							if (strncmp(pFile->lines[line_count], "#if", 6) == 0 || strncmp(pFile->lines[line_count], "#elif", 5) == 0) {
								if_count++;
							}
							pFile = remove_line(pFile, line_count);
						}
					}
					else {
						if (strncmp(pFile->lines[line_count], "#else", 5) == 0) {
							pFile = remove_line(pFile, line_count);
							while (strncmp(pFile->lines[line_count], "#endif", 6) != 0) {
								pFile = remove_line(pFile, line_count);
							}
						}
						//Remove the #endif line
						pFile = remove_line(pFile, line_count);
					}
				}
				else {
					//The condition is 0 which means everything should be remove until #endif is found or #else or #elif
					//If #elif is found then take the condition of #elif, if it is true then include everything till else, if is found, or #endif
					int line_count = i;
					while (strncmp(pFile->lines[line_count], "#endif", 6) != 0 && strncmp(pFile->lines[line_count], "#elif", 5) != 0 && strncmp(pFile->lines[line_count], "#else", 5) != 0) {
						pFile = remove_line(pFile, line_count);
					}

					if (strncmp(pFile->lines[line_count], "#elif", 5) == 0) {
						ELIF_FOUND: ;
						char* condition = pFile->lines[line_count] + 6;
						int cond_int = atoi(condition);
						pFile = remove_line(pFile, line_count);
						if (cond_int != 0) {
							//Take everything till the #else or #endif or #elif
							while (strncmp(pFile->lines[line_count], "#endif", 6) != 0 && strncmp(pFile->lines[line_count], "#elif", 5) != 0 && strncmp(pFile->lines[line_count], "#else", 5) != 0) {
								line_count++;
							}

							if (strncmp(pFile->lines[line_count], "#else", 5) == 0 || strncmp(pFile->lines[line_count], "#elif", 5) == 0) {
								while (strncmp(pFile->lines[line_count], "#endif", 6) != 0) {
									pFile = remove_line(pFile, line_count);
								}
							}

							if (strncmp(pFile->lines[line_count], "#endif", 6) == 0) {
								pFile = remove_line(pFile, line_count);
							}
						}
						else {
							while (strncmp(pFile->lines[line_count], "#else", 5) != 0 && strncmp(pFile->lines[line_count], "#elif", 5) != 0 && strncmp(pFile->lines[line_count], "#endif", 6) != 0) {
								pFile = remove_line(pFile, line_count);
							}

							if (strncmp(pFile->lines[line_count], "#elif", 5) == 0) {
								goto ELIF_FOUND;
							}

							if (strncmp(pFile->lines[line_count], "#else", 5) == 0) {
								pFile = remove_line(pFile, line_count);
								while (strncmp(pFile->lines[line_count], "#endif", 6) != 0) {
									line_count++;
								}

								if (strncmp(pFile->lines[line_count], "#endif", 6) == 0) {
									pFile = remove_line(pFile, line_count);
								}
							}
							
							if (strncmp(pFile->lines[line_count], "#endif", 5) == 0) {
								pFile = remove_line(pFile, line_count);
							}
						}
					}
					else {
						if (strncmp(pFile->lines[line_count], "#else", 5) == 0) {
							pFile = remove_line(pFile, line_count);
							while (strncmp(pFile->lines[line_count], "#endif", 6) != 0) {
								line_count++;
							}
						}
						pFile = remove_line(pFile, line_count);
					}
				}
			}
		}
	}

	return pFile;
}


//Prints the lines in the ParsedFile structure in the file specified in the parameter
void print_lines_in_file(ParsedFile* pFile, char* filename) {
	//Open the file
	FILE* file = NULL;
	file = fopen(filename, "w");
	if (file != NULL) {
		for (size_t i = 0; i < pFile->noLines; i++) {
			fprintf(file, "%s\n", pFile->lines[i]);
		}
	}
	else {
		//The file was not created
	}
}
