//  main.c
//

/*
 * Two pass SIC Linking Loader.
 */

//STD C libraries.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Helper functions
#include "Functions.h"

//Data Structures
#include "Hash_Table.h"

//SIC Library
#include "SIC_components.h"
#include "pass1.h"
#include "pass2.h"

int main(int argc, const char * argv[]) {
	
	//Check the minimum number of arguments are provided.
	if(argc < 5)
	{
		printf("USAGE: %s <SIC Object File(s)> <Program Load Address> <Output File Name> <Architecture (SIC || SICXE)>\n", argv[0]);
		return 1;
	}
	
	//Check the architecture specified is valid.
	if(strcmp(argv[argc-1],"SIC") && strcmp(argv[argc-1], "SICXE"))
	{
		printf("ERROR! Invalid architecture %s. Available architectures SIC|SICXE.\n", argv[argc-1]);
		return 1;
	}
	
	//Store the architecture.
	char architecture[6];
	strcpy(architecture, argv[argc-1]);
	
	//Store load address for checking and conversion.
	char* program_load_address_string = calloc(strlen(argv[argc-3]) + 1, sizeof(char));
	strcpy(program_load_address_string, argv[argc-3]);
	int program_load_address = -1;
	
	//Validate load address.
	if(is_integer(program_load_address_string))
	{
		program_load_address = (int) strtol(program_load_address_string, NULL, 16);
		
		if(!strcmp(architecture, "SIC") && program_load_address > SIC_MAX_ADDRESS)
		{
			printf("ERROR! Program Load Address exceeds maximum address for the %s architecture.\n", architecture);
			return 1;
		}
		
		else if(strcmp(architecture, "SICXE") && program_load_address > SICXE_MAX_ADDRESS)
		{
			printf("ERROR! Program Load Address exceeds maximum address for the %s architecture.\n", architecture);
			return 1;
		}
	}
	
	//Number of potential object files provided as arguments.
	int obj_files_count = argc - 4;
	
	//Check all files provided are object have object file extension.
	int i = obj_files_count;
	for(; i > 0; i--)
	{
		long length = strlen(argv[i]);
		
		if(argv[i][length-1] != 'o' || argv[i][length-2] != '.')
		{
			printf("Warning! Provided file %s may not be an object file.\n", argv[i]);
		}
		
	}
	
	//Store and open all files for reading, add file names to an array for error notifications in pass 1 and 2.
	char* obj_file_names[obj_files_count];
	FILE* obj_files[obj_files_count];
	i = 0;
	for(; i < obj_files_count; i++)
	{
		char* obj_file_name = calloc(strlen(argv[i+1]+1), sizeof(char));
		strcpy(obj_file_name, argv[i+1]);
		obj_file_names[i] = obj_file_name;
		obj_files[i] = fopen(argv[i+1], "r");
		
		//Check if file can be read.
		if (obj_files[i] == NULL) {
			printf("ERROR! %s cannot be opened.\n", argv[i+1]);
			return 1;
		}
	}
	
	//Creates an external symbol table.
	hash_table* exsym_tab = hash_table_init();
	
	//Runs pass 1 of the linking loader H and D records are read to define control sections symbols, address, and length and eternal symbols.
	if(pass1(exsym_tab, program_load_address,obj_file_names, obj_files, obj_files_count))
	{
		return 1;
	}
	
	//Move pointer to beginning of all files for second pass.
	i = 0;
	for(; i < obj_files_count; i++)
	{
		rewind(obj_files[i]);
	}
	
	//Runs pass 2
	if(pass2())
	{
		return 1;
	}
	
	return 0;
}
