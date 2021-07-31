//  pass2.c

//STD C libraries.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//Helper functions
#include "Functions.h"

//Data Structures
#include "Hash_Table.h"
#include "Queue.h"
 
//SIC libraries.
#include "SIC_components.h"


#include "pass2.h"

//Loads memory array contents into memory (file) and sets the program counter to the first instruction.
void load_in_memory(int execution_address, unsigned char memory[], int address_size, int memory_format_length)
{
	FILE* memory_file;
	memory_file = fopen("memory.txt", "w+");
	
	// Set program counter.
	fprintf(memory_file, "PC -> %X\n\n", execution_address);
	
	// Load physical memory.
	fprintf(memory_file, "MEMORY:\n");
	int a = 0;
	for(; a < address_size; a+=16)
	{
		fprintf(memory_file, "%0*X:\t", memory_format_length, a);
		for(int b = 0; b < 16; b++)
		{
			fprintf(memory_file, "%02X\t", memory[a+b]);
		}
		fprintf(memory_file, "\n");
	}
}

/*
 * Pass 2 of the SIC/XE linking loader, input is a list of object files and a external symbol table. Once done the object code will be relocated
 * and loaded into memory (file).
 */
int pass2(hash_table* exsym_tab, int program_load_address, char* file_names[], FILE* inputs[], int file_count, int memory_size)
{
	//Get the address size in hex digit length for formatting.
	int memory_address_byte_len = 0;
	if (memory_size == 32768)
	{
		memory_address_byte_len = 4;
	}
	
	if (memory_size == 1048576)
	{
		memory_address_byte_len = 6;
	}
	
	//Zero the SIC/XE memory.
	unsigned char memory[memory_size];
	memset(memory, 0X00, memory_size);
	

	//Set the loader address information.
	int control_section_address = program_load_address;
	int control_section_assembled_address = 0;
	int execution_address = program_load_address;
	int location_counter = control_section_address;
	
	//Loop through all the object files for the second pass.
	int n = 0;
	for(; n < file_count; n++)
	{
		
		//Get the next file.
		FILE* current_file = inputs[n];
		char line[128];
		memset(line, '\0', 128);
		
		//Control section length, start at invalid length until retrieved from file.
		int control_section_length = -1;
		
		//Begin reading of object file n.
		while(fgets(line, 128, current_file))
		{
			
			//Header (H) record.
			if(line[0] == 'H')
			{
				control_section_assembled_address = (int) strtol(substring(7, 6, line), NULL, 16);
				
				//Retrieve the substring with the control section length.
				control_section_length = (int) strtol(substring(13, 6, line), NULL, 16);
			}
			
			//Text (T) records.
			else if(line[0] == 'T')
			{
				//Get the records start address and compute the offset of the program, relative to old vs new starting address.
				int record_start_address = ((int) strtol(substring(1, 6, line), NULL, 16)) + (control_section_address - control_section_assembled_address);
				
				//Set the location counter to the new address.
				location_counter = record_start_address;
				
				//Get the record length.
				int record_length = (int) strtol(substring(7, 2, line), NULL, 16);
				
				//Error if program cannot be loaded from specified starting address.
				if(location_counter >= memory_size)
				{
					printf("ERROR! During program loading max address was exceeded.\n");
					return 1;
				}
				
				//Loop through the record and set object file contents into memory.
				int i = 0;
				for(; i < record_length*2; i+=2)
				{
					//Get the two char byte and convert to a single char hex value.
					char object_byte[3];
					memset(object_byte, '\0', 3);
					object_byte[0] = line[i+9];
					object_byte[1] = line[i+10];
					int hex_value = (int) strtol(object_byte, NULL, 16);
					
					//Load memory array.
					memory[location_counter] = (unsigned char) hex_value;
					
					location_counter ++;
				}
			}
			
			//Modification (M) record.
			else if(line[0] == 'M')
			{
				//Retrieve the symbol used for modification.
				char* modification_symbol_name = substring(10, 6, line);
				remove_end_whitespace(modification_symbol_name);
				
				//Get the symbol data from the external symbol table.
				external_symbol* modifying_symbol = hash_table_get(exsym_tab, modification_symbol_name);
				
				//If the symbol does not exist error.
				if(modifying_symbol == NULL)
				{
					printf("ERROR! Undefined modification symbol %s\n", modification_symbol_name);
					return 1;
				}
				
				//Symbol exists.
				else
				{
					//Get the modification data: address, length, number of bytes, and type (+ or -).
					int modification_address = (int) strtol(substring(1, 6, line), NULL, 16) + (control_section_address - control_section_assembled_address);
					int modification_length = (int) strtol(substring(7, 2, line), NULL, 16);
					int modification_type = (int) line[9];
					int num_bytes_to_modify = (int) ceil(modification_length/2.0);
					
					//Storage for code to be modified.
					unsigned char object_code_to_modify_bytes[num_bytes_to_modify+1];
					memset(object_code_to_modify_bytes, '\0', num_bytes_to_modify+1);
					
					//Retrieve m number of bytes to be modified, from memory for modification.
					int m = 0;
					for(; m < num_bytes_to_modify; m++)
					{
						object_code_to_modify_bytes[m] = memory[modification_address+m];
					}
					
					//Used to build the object code hex string.
					char* object_code_to_modify = malloc((modification_length + 1) * sizeof(char));
					char* object_code_builder = calloc(3, sizeof(char));
					
					//Build the string, as shown in memory. Pad and low hex characters in the string representation.
					m = 0;
					for(; m < num_bytes_to_modify; m++)
					{
						sprintf(object_code_builder, "%02X", object_code_to_modify_bytes[m]);
						strcat(object_code_to_modify, object_code_builder);
					}
					
					//Numerical value to be modified.
					int value_to_modify = (int) strtol(object_code_to_modify, NULL, 16);
					int modified_value = 0;
					
					//Modification add.
					if(modification_type == 43)
					{
						modified_value = value_to_modify + modifying_symbol->address;
					}
					
					//Modification subtract.
					else if(modification_type == 45)
					{
						modified_value = value_to_modify - modifying_symbol->address;
					}
					
					//Convert the modified object code back to hex.
					char* modified_object_code = calloc((num_bytes_to_modify*2) + 1, sizeof(char));
					sprintf(modified_object_code, "%04X", modified_value);
					
					//Load the appropriate bytes in memory.
					int byte_index = (int) strlen(modified_object_code) - 1;
					int bytes_left = (int) strlen(modified_object_code);
					m = (int) ceil(strlen(modified_object_code)/2.0) - 1;
					for(; m >= 0; m--)
					{
						//Create a hex byte.
						char object_byte[3];
						memset(object_byte, '\0', 3);
						
						//Create a full hex byte two chars from the string.
						object_byte[1] = modified_object_code[byte_index--];
						object_byte[0] = modified_object_code[byte_index--];
						bytes_left -= 2;
						
						//Load hex bytes into memory.
						int hex_value = (int) strtol(object_byte, NULL, 16);
						memory[modification_address+m] = (unsigned char) hex_value;
					}
					
					//Free used memory.
					free(object_code_to_modify);
					free(object_code_builder);
					free(modified_object_code);
				}
			}
			
			//End (E) Record.
			else if (line[0] == 'E')
			{
				//If a starting instruction address is specified set the execution address to it with proper relocation.
				if(strlen(line) == 7)
				{
					int first_instruction_address = (int) strtol(substring(1, 6, line), NULL, 16);
					execution_address = first_instruction_address + (control_section_address - control_section_assembled_address);;
				}
			
				//End record encountered end file processing.
				break;
			}
		}
		
		//Add the length of the finished control section to the start address to begin next control section.
		control_section_address += control_section_length;
		
		//Set location counter for new control address.
		location_counter = control_section_address;
	}
	
	//Load memory array contents into physical memory.
	load_in_memory(execution_address, memory, memory_size, memory_address_byte_len);

	return 0;
}

