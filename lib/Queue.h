//  Queue.h

/*
 * A very simple queue implementation. Elements are of any type. The queue is array based.
 */

#ifndef Queue_h
#define Queue_h

//Structures.

//A element on the queue.
typedef struct queue_element
{
    void* item;
} queue_element;

//The queue data structure.
typedef struct queue
{
    int capacity;
    int size;
    int front;
    int back;
    queue_element** queue;
} queue;

//Public functions.

//Creates a queue with a specified capacity. All space in the queue is set to NULL.
queue* queue_init(int capacity);

//Places an item in the queue. O(1)
void queue_enqueue(queue* q, queue_element* item);

//Returns the item on the front of the queue does not remove.
void* queue_peek(queue* q);

//Removes an item from the queue and returns it. O(1)
void* queue_dequeue(queue* q);

//Deallocates all memory used by a queue structure.
void queue_destroy(queue* q);

//Prints the queue info and the queue in a front to back order.
void queue_print(queue* q);

#endif /* Queue_h */
