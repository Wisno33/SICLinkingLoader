//  pass2.h

#ifndef pass2_h
#define pass2_h

/*
 * Pass 2 of the SIC/XE linking loader, input is a list of object files and a external symbol table. Once done the object code will be relocated
 * and loaded into memory (file).
 */
int pass2(hash_table* exsym_tab, int program_load_address, char* file_names[], FILE* inputs[], int file_count, int memory_size);

#endif /* pass2_h */
