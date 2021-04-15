//  SIC_components.h

#ifndef SIC_components_h
#define SIC_components_h

#define SIC_MAX_ADDRESS 32767
#define SICXE_MAX_ADDRESS 1048575

//External symbol structure a generic struct for any externally seen symbol, could be a control section symbol(C) or a
//standard external symbol(S). Control section symbols will make use of the length field, standard external symbols will not.
typedef struct external_symbol
{
	char type;
	int address;
	int length;
	char name[7];
} external_symbol;



#endif /* SIC_components_h */
