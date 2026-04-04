#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "mutex_sim.h"

/* process_init() initialized and already-declared simulated process.
 * Return 0 on success and -1 on failure.*/
int process_init(Process *p) {
    /* Input validation */
    if (p == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* Assign id */
    static int process_id = 0;
    p->id = process_id;
    process_id++;

    /* Zero the clock */
    p->clock = 0;

    /* Initialize the inbox Queue */
    if (-1 == queue_init(&(p->inbox)))
        return -1;

    /* Success */
    return 0;
}

/* send_message() sends a message of certain type from one simulated
 * process to another. Return -1 on failure and 0 on success */
int send_message(Process *sender, Process *receiver, MessageType type) {
    /* Input validation */
    if (sender == NULL || receiver == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* Allocate Message */
    Message *msg = malloc(sizeof(*msg));
    if (msg == NULL)
        return -1;      /* malloc sets errno */

    /* Move forward in time (sender) and store the msg */
    sender->clock++;
    msg->type = type;
    msg->sender = sender->id;
    msg->receiver = receiver->id;
    msg->timestamp = sender->clock;

    /* Enqueue the message into receiver's inbox */
    if ( enqueue(&(receiver->inbox), msg) == -1) {
        free(msg);
        return -1;
    }

    return 0;
}




