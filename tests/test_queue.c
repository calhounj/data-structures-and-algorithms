#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "queue.h"

int
main(void)
{
    Queue q;
    int value;

    puts("Initializing queue...");
    if (queue_init(&q) == -1) {
        perror("queue_init");
        exit(EXIT_FAILURE);
    }

    puts("\nInitial queue:");
    queue_print(&q);

    errno = 0;
    size_t n = queue_size(&q);
    if (n == 0 && errno != 0) {
        perror("queue_size");
        exit(EXIT_FAILURE);
    }
    printf("Size: %zu\n", n);

    puts("\nTesting peek on empty queue...");
    errno = 0;
    if (queue_peek(&q, &value) == -1) {
        if (errno == EINVAL)
            perror("queue_peek");
        else
            puts("queue_peek failed: queue is empty");
    }

    puts("\nEnqueue 10, 20, 30...");
    if (enqueue(&q, 10) == -1) {
        perror("enqueue");
        queue_destroy(&q);
        exit(EXIT_FAILURE);
    }
    if (enqueue(&q, 20) == -1) {
        perror("enqueue");
        queue_destroy(&q);
        exit(EXIT_FAILURE);
    }
    if (enqueue(&q, 30) == -1) {
        perror("enqueue");
        queue_destroy(&q);
        exit(EXIT_FAILURE);
    }

    puts("Queue after enqueue:");
    queue_print(&q);

    errno = 0;
    n = queue_size(&q);
    if (n == 0 && errno != 0) {
        perror("queue_size");
        queue_destroy(&q);
        exit(EXIT_FAILURE);
    }
    printf("Size: %zu\n", n);

    puts("\nPeeking at front...");
    errno = 0;
    if (queue_peek(&q, &value) == -1) {
        if (errno == EINVAL)
            perror("queue_peek");
        else
            puts("queue_peek failed: queue is empty");
        queue_destroy(&q);
        exit(EXIT_FAILURE);
    }
    printf("Front value: %d\n", value);

    puts("\nDequeuing one element...");
    errno = 0;
    if (dequeue(&q, &value) == -1) {
        if (errno == EINVAL)
            perror("dequeue");
        else
            puts("dequeue failed: queue is empty");
        queue_destroy(&q);
        exit(EXIT_FAILURE);
    }
    printf("Removed: %d\n", value);

    puts("Queue after one dequeue:");
    queue_print(&q);

    errno = 0;
    n = queue_size(&q);
    if (n == 0 && errno != 0) {
        perror("queue_size");
        queue_destroy(&q);
        exit(EXIT_FAILURE);
    }
    printf("Size: %zu\n", n);

    puts("\nDequeuing remaining elements...");
    while (!queue_is_empty(&q)) {
        errno = 0;
        if (dequeue(&q, &value) == -1) {
            if (errno == EINVAL)
                perror("dequeue");
            else
                puts("dequeue failed unexpectedly");
            queue_destroy(&q);
            exit(EXIT_FAILURE);
        }
        printf("Removed: %d\n", value);
        queue_print(&q);
    }

    puts("\nQueue should now be empty:");
    queue_print(&q);

    errno = 0;
    n = queue_size(&q);
    if (n == 0 && errno != 0) {
        perror("queue_size");
        queue_destroy(&q);
        exit(EXIT_FAILURE);
    }
    printf("Size: %zu\n", n);

    puts("\nTesting dequeue on empty queue...");
    errno = 0;
    if (dequeue(&q, &value) == -1) {
        if (errno == EINVAL)
            perror("dequeue");
        else
            puts("dequeue failed: queue is empty");
    }

    puts("\nDestroying queue...");
    queue_destroy(&q);

    puts("Queue after destroy:");
    queue_print(&q);

    errno = 0;
    n = queue_size(&q);
    if (n == 0 && errno != 0) {
        perror("queue_size");
        exit(EXIT_FAILURE);
    }
    printf("Size: %zu\n", n);

    return 0;
}
