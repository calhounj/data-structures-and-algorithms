#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "mutex_sim.h"

int main(void) {

    /* Create processes */
    printf("Initial processes:\n");
    Process p1, p2, p3;
    if (process_init(&p1) == -1) {
        perror("process_init");
        return -1;
    }

    if (process_init(&p2) == -1) {
        perror("process_init");
        return -1;
    }

    if (process_init(&p3) == -1) {
        perror("process_init");
        return -1;
    }

    /* Processes initialized, print */
    print_process(&p1);
    print_process(&p2);
    print_process(&p3);

    /* Send requests from p1 to p2 and p3 */
    if (send_message(&p1, &p2, REQUEST) == -1) {
        perror("send_message");
        return -1;
    }
    printf("After p1 sends first message:\n");
    print_process(&p1);

    if (send_message(&p1, &p3, REQUEST) == -1) {
        perror("send_message");
        return -1;
    }
    printf("After p1 sends second message:\n");
    print_process(&p1);

    /* Let processes receive messages */
    Message *msg = NULL;
    int result = 0;
    if ( (result = receive_message(&p2, &msg)) == -1) {
        perror("receive_message");
        return -1;
    }
    else if (result) {
        printf("empty inbox\n");
    }
    else {
        printf("Process after receiving:\n");
        print_process(&p2);
    }

    if ( (result = receive_message(&p3, &msg)) == -1) {
        perror("receive_message");
        return -1;
    }
    else if (result) {
        printf("empty inbox\n");
    }
    else {
        printf("Process after receiving:\n");
        print_process(&p3);
    }



    printf("Successful demonstration of Lamport clocks\n");

    return 0;
}
