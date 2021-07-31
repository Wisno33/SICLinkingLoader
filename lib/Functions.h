//Functions.h


#ifndef Functions_h
#define Functions_h

//Removes extra white space from a string. Used to remove ending whitespace on lines.
char* remove_begin_whitespace(char* string);

//Removes extra white space from a string. Used to remove ending whitespace on lines.
void remove_end_whitespace(char* string);

//Checks if all characters of a string are digits i.e. the string is a number. Return 0 if false 1 if true.
int is_integer(char* num_string);

//Checks if all characters of a string are valid hexadecimal. Return 0 if false 1 if true.
int is_hex(char* hex_string);

//Removes the extension of a file.
void remove_file_extension(char* file_name);

//Gets a specified section of a string, given a starting index and length to retrieve.
char* substring(int start, int length, char* string);

#endif /* Functions_h */
