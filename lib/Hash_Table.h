//  Hash_Table.h

#ifndef Hash_Table_h
#define Hash_Table_h

//Constants

//Maximum size of the hash-table.
//#define MAX_SIZE 128;

//Structures.

//Structure of a single hash-table element.
typedef struct hash_table_element
{
    void* value;
    char* key;
    
} hash_table_element;

//Hash-table structure
typedef struct hash_table
{
    int capacity;
    int size;
    hash_table_element** table;
    
} hash_table;

//Public functions.

//Creates and initializes the hash-table. Capacity is n, size is 0, and all slots are set to NULL upon creation.
hash_table* hash_table_init(int n);

/*
 * Sets a key value pair in the hash-table. Collisions are handled using linear probing. If the size of the hash-table is equal
 * to the capacity this function returns failure. Returns 0 on success and 1 on failure. Avg O(1) Worst O(n)
 */
int hash_table_set(hash_table* h_t, const char* key, hash_table_element* value);

//Retrieves an element given its key, if a key is not found on initial lookup the table will be searched. Avg O(1) Worst O(n)
void* hash_table_get(hash_table* h_t, char* key);

//Removes and returns a given element provided its key. Memory is also freed. Avg O(1) Worst O(n)
void* hash_table_remove(hash_table* h_t, char* key);

//Frees all memory used by the hash-table structure and its elements.
void hash_table_destroy(hash_table* h_t);

//Prints the hash-table info and the entries in the table.
void hash_table_dump(hash_table* h_t);

#endif /* Hash_Table_h */
