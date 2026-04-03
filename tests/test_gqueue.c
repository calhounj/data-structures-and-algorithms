#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "queue.h"

int main(void) {

    /* Dynamically allocated integers */
    int *a = NULL, *b = NULL, *c = NULL;
    char errmsg[256];
    errno = 0;
    if ( (a = malloc(sizeof(*a))) == NULL ) {
        strcpy(errmsg, "malloc");
        goto cleanup;
    }
    if ( (b = malloc(sizeof(*a))) == NULL ) {
        strcpy(errmsg, "malloc");
        goto cleanup;
    }
    if ( (c = malloc(sizeof(*a))) == NULL ) {
        strcpy(errmsg, "malloc");
        goto cleanup;
    }

    /* Store values */
    *a = 7; *b = 77; *c = 777;

    /* Testing enqueue */
    puts("Initializing queue...");
    Queue q = {NULL, NULL, 0};

    puts("\nEnqueue 7, 77, 777...");
    if (enqueue(&q, a) == -1) {
        strcpy(errmsg, "enqueue");
        goto cleanup;
    }
    if (enqueue(&q, b) == -1) {
        strcpy(errmsg, "enqueue");
        goto cleanup;
    }
    if (enqueue(&q, c) == -1) {
        strcpy(errmsg, "enqueue");
        goto cleanup;
    }

    puts("Queue after enqueue:");
    queue_print(&q);

    errno = 0;
    size_t n = queue_size(&q);
    if (n == 0 && errno != 0) {
        strcpy(errmsg, "queue_size");
        goto cleanup;
    }
    printf("Size: %zu\n", n);

    puts("\nPeeking at Queue:");
    errno = 0;
    void *out = NULL;
    if (queue_peek(&q, &out) == -1) {
        if (errno == 0)
            puts("queue is empty");
        else {
            strcpy(errmsg, "queue_peek");
            goto cleanup;
        }
    }
    int *data = out;
    printf("Front value: %d\n", *data);

    /* Testing dequeue */
    out = NULL;
    errno = 0;
    if (dequeue(&q, &out) == -1) {
        if (errno != 0) {
            strcpy(errmsg, "dequeue");
            goto cleanup;
        }
        else
            puts("dequeue on empty queue, nothing removed");
    }
    else {
        data = out;
        printf("data removed: %d\n", *data);
        puts("Queue after dequeue:");
        queue_print(&q);
    }

    out = NULL;
    errno = 0;
    if (dequeue(&q, &out) == -1) {
        if (errno != 0) {
            strcpy(errmsg, "dequeue");
            goto cleanup;
        }
        else
            puts("dequeue on empty queue, nothing removed");
    }
    else {
        data = out;
        printf("data removed: %d\n", *data);
        puts("Queue after dequeue:");
        queue_print(&q);
    }

    out = NULL;
    errno = 0;
    if (dequeue(&q, &out) == -1) {
        if (errno != 0) {
            strcpy(errmsg, "dequeue");
            goto cleanup;
        }
        else
            puts("dequeue on empty queue, nothing removed");
    }
    else {
        data = out;
        printf("data removed: %d\n", *data);
        puts("Queue after dequeue:");
        queue_print(&q);
    }

    int empty = queue_is_empty(&q);
    if (empty == -1) {
        strcpy(errmsg, "queue_is_empty");
        goto cleanup;
    }
    if (empty)
        puts("The queue is now empty");
    else
        puts("The queue is not empty");

    out = NULL;
    errno = 0;
    if (dequeue(&q, &out) == -1) {
        if (errno != 0) {
            strcpy(errmsg, "dequeue");
            goto cleanup;
        }
        else
            puts("dequeue on empty queue, nothing removed");
    }
    else {
        data = out;
        printf("data removed: %d\n", *data);
        puts("Queue after dequeue:");
        queue_print(&q);
    }

    goto cleanup;

cleanup:

    if (errno != 0) {
        fprintf(stderr, "%s: %s\n", errmsg, strerror(errno));
        queue_destroy_full(&q, free);
        exit(EXIT_FAILURE);
    }
    else {
        queue_destroy_full(&q, free);
        exit(EXIT_SUCCESS);
    }
}
