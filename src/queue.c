/* Implementations for Queue */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "queue.h"

/* Initialize an already-declared Queue */
int queue_init(Queue *q) {
    /* Input validation */
    if (q == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* An initialized queue needs to have nothing in it at first */
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;

    return 0; /* Successful initialization */
}

/* Test for an empty queue. Return 1 (TRUE) if it is empty,
 * 0 (FALSE) if it is not empty, and -1 for invalid input. */
int queue_is_empty(const Queue *q) {
    /* Input validation */
    if (q == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (q->size == 0)
        return 1;
    else
        return 0;
}

/* Return current count of nodes in queue.*/
size_t queue_size(const Queue *q) {
    /* Input validation */
    /* In main, the user needs to check errno to make sure zero means zero */
    if (q == NULL) {
        errno = EINVAL;
        return 0;
    }

    return q->size;
}

/* Print function for display and debugging. */
void queue_print(const Queue *q) {
    if (q == NULL) {
        printf("Invalid queue -- no display\n");
        return;
    }

    if (q->size == 0)
        printf("[]\n");
    else {
        printf("front [");
        const QueueNode *curr = q->front;   /* Remember const! */
        while (curr != q->rear) {
            printf("%d -> ", curr->value);
            curr = curr->next;
        }
        printf("%d] rear\n", curr->value);
    }
}

/* Classic enqueue function: add to the rear.*/
int enqueue(Queue *q, int val) {
    /* Validate input */
    if (q == NULL) {
        errno = EINVAL;
        return -1;
    }

    QueueNode *node = malloc(sizeof(*node));
    if (node == NULL)
        return -1;

    node->value = val;
    node->next = NULL;

    if (q->size == 0) {
        q->front = node;
        q->rear = node;
    }
    else {
        q->rear->next = node;
        q->rear = node;
    }

    q->size++;
    return 0;
}
