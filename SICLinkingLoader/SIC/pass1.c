//  pass1.c

//STD C libraries.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Helper functions
#include "Functions.h"

//Data Structures
#include "Hash_Table.h"
#include "Queue.h"

//SIC libraries.
#include "SIC_components.h"

//Header for function prototypes.
#include "pass1.h"

//Prints the external symbol table constructed from pass 1 output.
void print_exsym_tab(hash_table* exsym_tab, queue* order)
{
	while(order->size > 0)
		{
			char* exsym_name = queue_dequeue(order);
			
			external_symbol* exsym =  hash_table_get(exsym_tab, exsym_name);
			
			//Control section symbol.
			if(exsym->type == 'C')
			{
				printf("%6s\t\t\t%X\t%X\n", exsym->name, exsym->address, exsym->length);
			}
			//Standard external symbol.
			else
			{
				printf("\t\t%6s\t%X\t\t\n", exsym->name, exsym->address);
			}
		}
		return;
}

//Pass 1 of the SIC/XE linking loader, input is a list of object files out put will be an external symbol table.
int pass1(hash_table* exsym_tab, int program_load_address, char* file_names[], FILE* inputs[], int num_object_files)
{
	//Set initial the control section address to the program load address.
	int control_section_address = program_load_address;
	
	//A queue of encountered symbols in order for console output.
	queue* external_symbol_order = queue_init(64);
	
	//Loop trough all input files.
	int n = 0;
	for(; n < num_object_files; n++)
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
				//Check line length of the header record is 20, if not the object file is invalid.
				int line_length = (int) strlen(line);
				if(line_length != 20)
				{
					printf("ERROR! Invalid H record in %s\n", file_names[n]);
					return 1;
				}
				
				//Get the substring of the record containing the control section name, and remove whitespace if present.
				char* control_section_name = substring(1, 6, line);
				remove_end_whitespace(control_section_name);
				
				//Retrieve the substring with the control section length.
				control_section_length = (int) strtol(substring(13, 6, line), NULL, 16);
				
				//Create an external symbol for the control section symbol.
				external_symbol* exsym = calloc(1, sizeof(external_symbol));
				exsym->type = 'C';
				exsym->address = control_section_address;
				exsym->length = control_section_length;
				strcpy(exsym->name, control_section_name);
				
				//Check if the control section is already defined if so error.
				if(hash_table_get(exsym_tab, exsym->name) != NULL)
				{
					printf("ERROR! Control section name %s already defined.\n", exsym->name);
					return 1;
				}
				
				//Add the control section symbol to the external symbol table.
				hash_table_element* el = calloc(1, sizeof(hash_table_element));
				el->key = exsym->name;
				el->value = exsym;
				
				hash_table_set(exsym_tab, el->key, el);
				
				//Add the control section symbol to the print queue.
				queue_element* q_el = calloc(1, sizeof(queue_element));
									
				q_el->item = el->key;
				
				queue_enqueue(external_symbol_order, q_el);
			}
			
			//Define (D) record.
			else if(line[0] == 'D')
			{
				
				//Check that the line length of the define record is less than the max length 74 and that the symbols present are complete.
				int line_length = (int) strlen(line);
				if (line_length > 74 || (line_length-2) % 12 != 0)
				{
					printf("ERROR! Invalid D record in %s\n", file_names[n]);
					return 1;
				}
				
				//Get the number of symbols in the record.
				int num_symbols = (line_length-2) / 12;
				
				//For each symbol attempt create and add a external symbol to the external symbol table.
				int i = 0;
				for(; i < num_symbols; i++)
				{
					//Retrive the i+1th external symbol from the record, and remove white space if present.
					char* external_symbol_name = substring(1 + (i*12), 6, line);
					remove_end_whitespace(external_symbol_name);
					//Retrive the i+th external symbol address from the record.
					int external_symbol_address = (int) strtol(substring(7 + (i*12), 6, line), NULL, 16);
					
					//Create an external symbol for the defined symbols.
					external_symbol* exsym = calloc(1, sizeof(external_symbol));
					exsym->type = 'S';
					exsym->address = external_symbol_address + control_section_address;
					exsym->length = -1;
					strcpy(exsym->name, external_symbol_name);
					
					//Check if the symbol is already present in the external symbol table if so error.
					if(hash_table_get(exsym_tab, exsym->name) != NULL)
					{
						printf("ERROR! External symbol name %s already defined.\n", exsym->name);
						return 1;
					}
					
					
					//Add the external symbol to the external symbol table.
					hash_table_element* el = calloc(1, sizeof(hash_table_element));
					el->key = exsym->name;
					el->value = exsym;
					
					hash_table_set(exsym_tab, el->key, el);
					
					//Add the external symbol to the print queue.
					queue_element* q_el = calloc(1, sizeof(queue_element));
										
					q_el->item = el->key;
					
					queue_enqueue(external_symbol_order, q_el);
				}
			}
			
			//End the loop for a file when the End (E) record is encountered.
			else if(line[0] == 'E')
			{
				break;
			}
		}
		
		//Set the next control section address to the current control section address plus the length of the current control section.
		control_section_address += control_section_length;

	}
	
	//Prints the external symbol table.
	print_exsym_tab(exsym_tab, external_symbol_order);
	queue_destroy(external_symbol_order);
	
	return 0;
}
