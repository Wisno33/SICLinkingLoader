//Functions.c

//STD C libraries.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Header for function prototypes.
#include "Functions.h"

//Removes extra white space from a string. Used to remove ending whitespace on lines.
char* remove_begin_whitespace(char* string)
{
	if(string == NULL)
	{
		return NULL;
	}
	
	int pointer_increment = 0;
	
	int i = 0;
	for(; i<strlen(string); i++)
	{
		if(string[i] < 33 || string[i] == 127)
		{
			pointer_increment += sizeof(string[i]);
		}
		else
		{
			break;
		}
		
	}
	
	//The whole string is whitespace return null as the string would be NULL minus the whitespace.
	if((pointer_increment / sizeof(char)) == strlen(string))
	{
		return NULL;
	}
	
	//Pointer arithmetic to point the pointer to the first non-whitespace char.
	string += pointer_increment;
	
	return string;
}

//Removes extra white space from a string. Used to remove ending whitespace on lines.
void remove_end_whitespace(char* string)
{
	if(string == NULL)
	{
		return;
	}
	
	//Finds the first whitespace in a string and sets it to the null char essentially terminating the string.
	int i = 0;
	for(; i<strlen(string); i++)
	{
		if(string[i] == 32 || string[i] == 9 || string[i] == 13)
		{
			string[i] = '\0';
			return;
		}
	}
	
	return;
}

//Checks if all characters of a string are digits i.e. the string is a number. Return 0 if false 1 if true.
int is_integer(char* num_string)
{
	if(num_string == NULL)
	{
		return 0;
	}
	
	int i = 0;
	for(; i<strlen(num_string); i++)
	{
		if(!isdigit(num_string[i]))
		{
			return 0;
		}
	}
	
	return 1;
}

//Checks if all characters of a string are valid hexadecimal. Return 0 if false 1 if true.
int is_hex(char* hex_string)
{
	if(hex_string == NULL)
	{
		return 0;
	}
	
	int i = 0;
	for(; i < strlen(hex_string); i++)
	{
		char ch = hex_string[i];
		
		if ((ch < '0' || ch > '9') && (ch < 'A' || ch > 'F'))
		{
			return 0;
		}
	}
	
	return 1;
}

//Removes the extension of a file.
void remove_file_extension(char* file_name)
{
	if(file_name == NULL)
	{
		return;
	}
	
	//Finds the first dot in a string and sets it to the null char essentially terminating the string.
	int i = (int) strlen(file_name) - 1;
	for(; i>-1; i--)
	{
		if(file_name[i] == 46)
		{
			file_name[i] = '\0';
			return;
		}
	}
	
	return;
}

//Gets a specified section of a string, given a starting index and length to retrieve.
char* substring(int start, int length, char* string)
{
	//Checks if string is NULL.
	if(string == NULL)
	{
		return NULL;
	}
	
	//Allocate space for a new string that is a substring of the argument string.
	char* sub_string = calloc(length, sizeof(char));
	
	//Build the substring from the given section of the argument string.
	int i = 0;
	for(; i < length; i++)
	{
		sub_string[i] = string[i+start];
	}
	
	return sub_string;
}
