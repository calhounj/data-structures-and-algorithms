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
    Queue inbox;
} Process;

/* Initialize a process. Return -1 on failure and 0 on success. */
int process_init(Process *p);
/* Send a message. Return -1 on failure, 0 success */
int send_message(Process *sender, Process *receiver, MessageType type);

#endif
