//  pass1.h

#ifndef pass1_h
#define pass1_h

//Public Functions

//Pass 1 of the SIC/XE linking loader, input is a list of object files out put will be an external symbol table.
int pass1(hash_table* exsym_tab, int program_load_address,char* file_names[], FILE* inputs[], int num_object_files);

#endif /* pass1_h */
