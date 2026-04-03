#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>


/* QueueNode */
typedef struct queuenode {
    void *data;
    struct queuenode *next;
} QueueNode;

/* Queue */
typedef struct queue {
    QueueNode *front;
    QueueNode *rear;
    size_t size;
} Queue;

/* Inititalize an already-declared Queue */
int queue_init(Queue *q);
/* Add a new data pointer to the rear of the queue */
int enqueue(Queue *q, void *data);
/* Remove the node at the front of the queue */
/* Optionally store the data in *out */
int dequeue(Queue *q, void **out);
int queue_peek(const Queue *q, void **out);
/* Test whether queue has zero elements */
int queue_is_empty(const Queue *q);
size_t queue_size(const Queue *q);
void queue_print(const Queue *q);
/* Generic Destroy function */
void queue_destroy_full(Queue *q, void (*free_fn)(void *));


#endif
