#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>


/* QueueNode */
typedef struct queuenode {
    int value;
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
/* Add a new value to the rear of the queue */
int enqueue(Queue *q, int val);
/* Remove the node at the front of the queue */
/* Optionally store the value in *out */
int dequeue(Queue *q, int *out);
int queue_peek(const Queue *q, int *out);
/* Test whether queue has zero elements */
int queue_is_empty(const Queue *q);
size_t queue_size(const Queue *q);
void queue_print(const Queue *q);
void queue_destroy(Queue *q);


#endif
