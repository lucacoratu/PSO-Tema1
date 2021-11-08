#include "HashMap.h"
#include "FileParser.h"
#include "ParameterParser.h"

#define MAP_MAX_SIZE 30
HashMapEntry* hmap[MAP_MAX_SIZE];

int main(int argc, char** argv) {
	//First of all, the parameters received must be put accordingly in the parsed parameters structure
	ParsedParameters* pParameters = NULL;
	pParameters = parse_parameters(argc, argv, hmap, MAP_MAX_SIZE);
	
	if(pParameters == NULL) {
		printf("Invalid option given, usage: ./so-cpp ");
		return -1;
	}
	ParsedFile* pfile = NULL;
	char* stdinContent = (char*)malloc(sizeof(char) * 1);
	
	if(strncmp(pParameters->infile, "stdin", 5) != 0){
	//Parse all of the parameters and check if they are valid
		int correct = verify_parsed_parameters(pParameters);
		if (correct == 0) {
			//Exit the program with error
			return -1;
		}
		
		 pfile = break_into_lines(pParameters->infile);
	}
	else{
		//Read the file from standard input
		int content_size = 0;
		char ch = fgetc(stdin);
		while(ch != EOF){
			stdinContent = (char*)realloc(stdinContent, (content_size + 1) * sizeof(char));
			stdinContent[content_size] = ch;
			content_size++;
			ch = fgetc(stdin);
		}
		stdinContent[content_size] = '\0';
		
		pfile = break_to_lines(stdinContent);
	}

	//print_parsed_parameters_structure(pParameters);

	//print_hash_map(hmap, MAP_MAX_SIZE);
	
	if (pfile != NULL) {
		//Print the lines
		//print_lines(pfile,1);

		//Add the includes
		pfile = add_includes(pfile,pParameters);

		//Print the lines
		//print_lines(pfile, 1);

		pfile = load_simple_defines(hmap, MAP_MAX_SIZE, pfile);
	
		//print_hash_map(hmap, MAP_MAX_SIZE);

		pfile = load_ifdefstatements_true(hmap, MAP_MAX_SIZE, pfile);
		
		//print_hash_map(hmap, MAP_MAX_SIZE);

		//print_lines(pfile, 1);
		pfile = replace_define_names_with_values(hmap, MAP_MAX_SIZE, pfile);
		//print_lines(pfile, 1);

		//Print the lines
		//print_lines(pfile,1);

		pfile = load_ifstatements_true(pfile);

		//Print the lines
		//print_lines(pfile);
		

		//Print the lines
		//print_lines(pfile);

		pfile = load_simple_defines(hmap, MAP_MAX_SIZE, pfile);

		//print_hash_map(hmap, MAP_MAX_SIZE);

		pfile = replace_define_names_with_values(hmap, MAP_MAX_SIZE, pfile);

		//Print the lines
		//print_lines(pfile);

		//Put the preprocessed lines in the output file if it exists, else print them on stdout
		if (pParameters->outfile != NULL) {
			print_lines_in_file(pfile, pParameters->outfile);
		}
		else {
			print_lines(pfile, 0);
		}
	}

	return 0;
}
