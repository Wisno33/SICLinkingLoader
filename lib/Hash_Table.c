//  Hash_Table.c

/*
 * A simple hash-table implementation. Elements are key value pairs, where keys a strings (char *) and
 * and values can be of any type. The hash-table is array based and uses linear probing.
 */

//STD C libraries.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Header for function prototypes.
#include "Hash_Table.h"

//The hash function for keys of the hash-table.
size_t hash_table_hash(const char* key, int capacity)
{
    
    size_t hash_value = 0;
    
    int seed = 31;
    int key_len = (int) strlen(key);
    
    int i = 0;
    for(; i<key_len; i++)
    {
        hash_value += seed * key[i];
    }
    
    return hash_value % capacity;
}

//Creates and initializes the hash-table. Capacity is n, size is 0, and all slots are set to NULL upon creation.
hash_table* hash_table_init(int n)
{
    hash_table* h_t = (hash_table*) malloc(sizeof(hash_table));
    
    h_t->capacity = n;
    h_t->size = 0;
    h_t->table = malloc(sizeof(hash_table_element*) * h_t->capacity);
    
    int i = 0;
    for(; i<h_t->capacity; i++)
    {
        h_t->table[i] = NULL;
    }
    
    return h_t;
}

/*
 * Sets a key value pair in the hash-table. Collisions are handled using linear probing. If the size of the hash-table is equal
 * to the capacity this function returns failure. Returns 0 on success and 1 on failure. Avg O(1) Worst O(n)
 */
int hash_table_set(hash_table* h_t, const char* key, hash_table_element* value)
{
    //If full return.
    if(h_t->size == h_t->capacity)
    {
        return 1;
    }
    
    //Computes the hash of the key.
    size_t hash_value = hash_table_hash(key, h_t->capacity);
    
    /*
     * Loops until an empty slot is found. The original hash value is used as the base location, if the base location is free the
     * value is stored there.
     */
    int collision_counter = 0;
    size_t new_location;
    
    while(collision_counter < h_t->capacity - h_t->size)
    {
        //New possible location is the original hash location plus collision spaces.
        new_location = (hash_value + collision_counter) % h_t->capacity;
        
        //If a spot is empty the value is added here and the loop ends.
        if(h_t->table[new_location] == NULL)
        {
            h_t->table[new_location] = value;
            h_t->size += 1;
            
            return 0;
        }
        
        //If the key is already in the table the value is updated.
        else if(!strcmp(key, h_t->table[new_location]->key))
        {
            h_t->table[new_location] = value;
            h_t->size += 1;
            
            return 0;
        }
        
        //Increase counter.
        collision_counter++;
    }
    
    return 1;
}

//Retrieves an element given its key, if a key is not found on initial lookup the table will be searched. Avg O(1) Worst O(n)
void* hash_table_get(hash_table* h_t, char* key)
{
    //Compute hash.
    size_t hash_value = hash_table_hash(key, h_t->capacity);
    
    size_t new_location = hash_value;
    
    //Loops until the key is found if not NULL is returned as the key does not exist in the table. Starts from the base location.
    int i = 0;
    for(; i<h_t->capacity; i++)
    {
        new_location = (hash_value + i) % h_t->capacity;
        
        //Not found.
        if(h_t->table[new_location] == NULL)
        {
            return NULL;
        }
        
        //If the key is found it is returned.
        else if(strcmp(h_t->table[new_location]->key, key) == 0)
        {
            return h_t->table[new_location]->value;
        }
    }
    
    return NULL;
}

//Removes and returns a given element provided its key. Memory is also freed. Avg O(1) Worst O(n)
void* hash_table_remove(hash_table* h_t, char* key)
{
    //Compute hash.
    size_t hash_value = hash_table_hash(key, h_t->capacity);
    
    size_t new_location = hash_value;
    
    /*Loops until the key is found if not NULL is returned as the key does not exist in the table.
     */
    int i = 0;
    for(; i<h_t->capacity; i++)
    {
        new_location = (hash_value + i) % h_t->capacity;
        
        //Not found.
        if(h_t->table[new_location] == NULL)
        {
            return NULL;
        }
        
        //If they key is found, creates a pointer to the element and sets the entry in the table to NULL and returns the value.
        else if(!strcmp(h_t->table[new_location]->key, key))
        {
            hash_table_element* el = h_t->table[new_location];
            
            h_t->table[new_location] = NULL;
            h_t->size -= 1;
            
            return el->value;
        }
    }
    
    return NULL;
}

//Frees all memory used by the hash-table structure and its elements.
void hash_table_destroy(hash_table* h_t)
{
    int i = 0;
    for(; i<h_t->capacity; i++)
        {
            if(h_t->table[i] != NULL)
            {
                hash_table_remove(h_t, h_t->table[i]->key);
            }
            
            free(h_t->table[i]);
        }
    
    free(h_t->table);
    free(h_t);
}

//Prints the hash-table info and the entries in the table.
void hash_table_dump(hash_table* h_t)
{
    printf("Size: %d, Capacity: %d\n", h_t->size, h_t->capacity);
    
    int count = 0;
    int i = 0;
    for(; i < h_t->capacity; i++)
    {
        if(h_t->table[i] != NULL)
        {
            count += 1;
            printf("%d: HT[%d] = Key: %s, Value at %p\n",count, i, h_t->table[i]->key, h_t->table[i]->value);
        }
    }
}
