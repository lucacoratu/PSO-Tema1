#ifndef FILEPARSER_H
#define FILEPARSER_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HashMap.h"
#include "ParameterParser.h"

//Structure to hold the file name, file content split into lines and the number of lines
typedef struct {
	char* filename;
	char** lines;
	size_t noLines;
} ParsedFile;


//Get the whole content of the file
char* get_file_content(const char* filename);

//Concatenates two strings and returns a new string
char* concatenate_strings(const char* string1,const char* string2);

//Print the lines in the ParsedFile structure
void print_lines(ParsedFile* pFile, int debug);

//Initialize the ParsedFile struct with the filename
ParsedFile* initialize_pfile_struct(const char* filename);

//Break the content of the file into lines and save the empty lines as well
ParsedFile* break_into_lines(const char* filename);

//Break a string into lines
ParsedFile* break_to_lines(char* content);

//Frees the memory allocated in the parsed file pointer
void clear_parsed_pointer(ParsedFile* pFile);

//Removes the line specified in the parameter
ParsedFile* remove_line(ParsedFile* pFile, size_t line_index);

//Adds the lines to the back of the parsed file pointer
ParsedFile* add_lines_back(ParsedFile* pFile, ParsedFile* pFileHeader);

//Adds the content of the include file if the file exists
ParsedFile* add_includes(ParsedFile* pFile, ParsedParameters* pParameters);

//Breaks the string into tokens
char** break_string_into_tokens(char* string,int* length, const char delim);

//Checks to see if the define has another define name in the value
char* value_contains_other_define(HashMapEntry** map, size_t map_size, char* value);

//Loads the hashmap with all the defines that are not inside an #if statement
ParsedFile* load_simple_defines(HashMapEntry** map, size_t map_size, ParsedFile* pFile);

//Checks if the found is inside double quotes
int check_name_inside_dquotes(char* string, char* found);

//Undef the variables
ParsedFile* undef_defines(HashMapEntry** map, size_t map_size, ParsedFile* pFile);

//Change the defines names with their values
ParsedFile* replace_define_names_with_values(HashMapEntry** map, size_t map_size, ParsedFile* pFile);

//Loads the code inside ifdef, if the condition is met
ParsedFile* load_ifdefstatements_true(HashMapEntry** map, size_t map_size, ParsedFile* pFile);

//Loads the code inside #if directive if the condition is a value not equal to 0
ParsedFile* load_ifstatements_true(ParsedFile* pFile);

//Prints the lines in the ParsedFile structure in the file specified in the parameter
void print_lines_in_file(ParsedFile* pFile, char* filename);

#endif
