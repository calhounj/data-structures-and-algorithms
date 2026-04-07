#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "mutex_sim.h"

/* helper function for testing */
void print_process(Process *p) {
    if (p == NULL)
        return;

    /* Print id */
    printf("Process %d ", p->id);
    /* Print logical clock */
    printf("clock=%d ", p->clock);
    /* Print inbox size */
    printf("inbox size=%zu\n", queue_size(&(p->inbox)));
}


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

    /* Requesting: 0 no, 1 yes */
    p->requesting = 0;

    /* Initial Request timestamp */
    p->request_timestamp = -1;

    /* Initial Replies needed */
    p->replies_needed = 0;

    /* Initialize the inbox Queue */
    if (-1 == queue_init(&(p->inbox)))
        return -1;

    /* Initialize the deferred queue */
    if (-1 == queue_init(&(p->deferred_replies)))
        return -1;

    /* Success */
    return 0;
}

/* send_message_with_timestamp() sends a message of certain type from one
 * simulated process to another. Return -1 on failure and 0 on success.
 * This is a helper function for the more generic send_message */
int send_message_with_timestamp(Process *sender, Process *receiver,
                 MessageType type, int timestamp) {
    /* Input validation */
    if (sender == NULL || receiver == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* Allocate Message */
    Message *msg = malloc(sizeof(*msg));
    if (msg == NULL)
        return -1;      /* malloc sets errno */

    /* Use timestamp and store the msg */
    msg->type = type;
    msg->sender = sender->id;
    msg->receiver = receiver->id;
    msg->timestamp = timestamp;

    /* Enqueue the message into receiver's inbox */
    if ( enqueue(&(receiver->inbox), msg) == -1) {
        free(msg);
        return -1;
    }

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

    sender->clock++;
    if (-1 == send_message_with_timestamp(sender, receiver,
                                          type, sender->clock))
        return -1;

    return 0;
}

/* Given a process, remove one message from its inbox and update the clock.
 * Return 0 on success, -1 on failure, and 1 if process inbox is empty.*/
int receive_message(Process *p, Message **out) {
    /* Input validation */
    if (p == NULL || out == NULL) {
        errno = EINVAL;
        return -1;
    }
    /* ensure *out isn't left with garbage */
    *out = NULL;

    /* dequeue */
    errno = 0;
    void *temp = NULL;
    Message *msg = NULL;
    if (dequeue(&(p->inbox), &temp) == -1) {
        if (errno != 0)
            return -1;
        else
            return 1;       /* Empty queue */
    }
    else
        msg = temp;/* void * casts automatically */

    /* Update process clock Lamport style */
    int p_clock = p->clock;
    int m_timestamp = msg->timestamp;
    if (p_clock >= m_timestamp)
        p->clock = p_clock + 1;
    else
        p->clock = m_timestamp + 1;

    /* Store Message * */
    *out = msg;

    return 0;
}

/* Broadcast a request for the critical section. */
int request_critical_section(Process processes[], int n, int self_index) {
    /* Input validation */
    if (processes == NULL || n <= 0 || self_index < 0 || self_index >= n) {
        errno = EINVAL;
        return -1;
    }

    /* Identify requesting process */
    Process *self = &processes[self_index];
    self->clock++;
    self->requesting = 1;
    self->request_timestamp = self->clock;
    self->replies_needed = n - 1;

    /* Broadcast REQUEST */
    int not_sent = 0;
    for (int i = 0; i < n; i++) {
        if (i == self_index)
            continue;
        not_sent = send_message_with_timestamp(self, &processes[i], REQUEST,
                                               self->request_timestamp);
        if (not_sent)
            return -1;
    }

    return 0;
}































