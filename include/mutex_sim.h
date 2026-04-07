#ifndef MUTEX_SIM_H
#define MUTEX_SIM_H

#include "queue.h"

typedef enum MessageType {
    REQUEST,
    REPLY,
    RELEASE
} MessageType;

typedef struct message {
    MessageType type;
    int sender;
    int receiver;
    int timestamp;
} Message;

typedef struct process {
    int id;
    int clock;
    int requesting;
    int request_timestamp;
    int replies_needed;
    Queue inbox;
    Queue deferred_replies;
} Process;

/* Print function for a process */
void print_process(Process *p);
/* Initialize a process. Return -1 on failure and 0 on success. */
int process_init(Process *p);
/* Send a message. Return -1 on failure, 0 success */
int send_message(Process *sender, Process *receiver, MessageType type);
/* Helper function to allow the previous more generic send function.
 * Return -1 on failure or 0 on success */
int send_message_with_timestamp(Process *sender, Process *receiver,
                                MessageType type, int timestamp);
/* Receive a message. Return -1 on failure, 0 on success, 1 for empty inbox */
int receive_message(Process *p, Message **out);
/* Broadcast message to other processes */
int request_critical_section(Process processes[], int n, int self_index);

#endif
