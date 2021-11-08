#ifndef PARAMETERPARSER_H
#define PARAMETERPARSER_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HashMap.h"

typedef struct {
	char* option;
	char* name;
	char* mapping;
} NameMapping;

typedef struct {
	char* infile;
	char* in_directory;
	char* outfile;
	NameMapping** name_mapping_list;
	size_t size_list;
} ParsedParameters;


//Functions
//Create a name mapping structure
NameMapping* create_name_mapping(char* option, char* name, char* mapping);

//Add the NameMapping stucture to the list in ParsedParameters stucture
ParsedParameters* add_name_mapping_into_parsed_parameters(NameMapping* nm, ParsedParameters* pParameters);

//Take the list of parameters and construct the ParsedParameters based on it
ParsedParameters* parse_parameters(int argc, char** argv, HashMapEntry** map, size_t map_size);

//Veryfies if the parameters given to the application are correct
int verify_parsed_parameters(ParsedParameters* pParameters);

//Print the NameMapping structure
void print_name_mapping(NameMapping* nm);

//Print function...for debug only
void print_parsed_parameters_structure(ParsedParameters* pParameters);

int find_last_slash_or_backslash(char* string);


#endif // !PARAMETERPARSER_H

