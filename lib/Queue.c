//  Queue.c

/*
 * A very simple queue implementation. Elements are of any type. The queue is array based.
 */

//STD C libraries.
#include <stdlib.h>
#include <stdio.h>

//Header for function prototypes.
#include "Queue.h"

//Creates a queue with a specified capacity. All space in the queue is set to NULL.
queue* queue_init(int capacity)
{
    queue* q = (queue*) malloc(sizeof(queue));
    
    q->capacity = capacity;
    q->size = 0;
    q->front = 0;
    q->back = 0;
    q->queue = malloc(sizeof(queue_element) * capacity);
    
    int i=0;
    for(; i<q->capacity; i++)
    {
        q->queue[i] = NULL;
    }
    
    return q;
}

//Places an item in the queue. O(1)
void queue_enqueue(queue* q, queue_element* item)
{
    //Checks if queue is full.
    if(q->size == q->capacity)
    {
        return;
    }
        
    q->queue[q->back] = item;
    
    q->back = (q->back + 1) % q->capacity;
    q->size += 1;
}

//Returns the item on the front of the queue does not remove.
void* queue_peek(queue* q)
{
	//Checks if queue is empty.
	if(q->size == 0)
	{
		return NULL;
	}
	
	return q->queue[q->front]->item;
}

//Removes an item from the queue and returns it. O(1)
void* queue_dequeue(queue* q)
{
    //Checks if queue is empty.
    if(q->size == 0)
    {
        return NULL;
    }
    
    int cur_front = q->front;
    
    q->front = (q->front + 1) % q->capacity;
    q->size -= 1;
    
    return q->queue[cur_front]->item;
}

//Deallocates all memory used by a queue structure.
void queue_destroy(queue* q)
{
    int i = 0;
    for(; i<q->capacity; i++)
    {
        free(q->queue[i]);
    }
    
    free(q->queue);
    free(q);
}

//Prints the queue info and the queue in a front to back order.
void queue_print(queue* q)
{
    printf("Size: %d, Capacity: %d\n", q->size, q->capacity);
    
    int i = 0;
    for(; i<q->size ; i++)
    {
        printf("Item at %p\n", q->queue[i]);
    }
}
