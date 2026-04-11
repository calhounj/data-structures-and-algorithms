#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "dlist.h"

/* Node allocation helper */
static int create_node(DListNode **node, void *data) {
    /* Requires a valid DListNode pointer */
    if (node == NULL) {
        errno = EINVAL;
        return -1;
    }

    *node = malloc(sizeof(DListNode));
    if (*node == NULL)
        return -1;

    /* Store data and follow single-element invariant */
    (*node)->data = data;
    (*node)->prev = NULL;
    (*node)->next = NULL;

    return 0;
}

/* Initialize a doubly-linked list */
int dlist_init(DList *list) {
    /* Input validation */
    if (list == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* Maintain empty list invariants */
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return 0;
}

/* Query a list to determine if it is empty */
int dlist_is_empty(const DList *list) {
    /* Input validation */
    if (list == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (list->head == NULL)
        return 1;
    else
        return 0;
}

/* Query the size of a list. */
int dlist_size(const DList *list, size_t *out) {
    /* Invalid input */
    if (list == NULL || out == NULL) {
        errno = EINVAL;
        return -1;
    }

    *out = list->size;
    return 0;
}

/* Push a node to the front of the list */
int dlist_push_front(DList *list, void *data) {
    /* Validation */
    if (list == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* Create node for push */
    DListNode *new_node = NULL;
    if (create_node(&new_node, data) == -1)
        return -1;

    /* list is empty, invariants must hold */
    if (dlist_is_empty(list)) {
        list->head = new_node;
        list->tail = new_node;
        list->size++;
        return 0;
    }

    /* list is not empty */
    new_node->next = list->head;
    list->head->prev = new_node;
    /* new_node->prev already points to NULL */
    list->head = new_node;
    list->size++;

    return 0;
}

/* Push to the end of the list */
int dlist_push_back(DList *list, void *data) {
    /* Validation */
    if (list == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* Create node for push */
    DListNode *new_node = NULL;
    if (create_node(&new_node, data) == -1)
        return -1;

    /* list is empty, invariants must hold */
    if (dlist_is_empty(list)) {
        list->head = new_node;
        list->tail = new_node;
        list->size++;
        return 0;
    }

    /* list is not empty */
    list->tail->next = new_node;
    new_node->prev = list->tail;
    /* new_node->next already points to NULL */
    list->tail = new_node;
    list->size++;

    return 0;
}

/* Peek at the front of the list. Store data in an out variable */
int dlist_peek_front(const DList *list, void **out) {
    if (list == NULL || out == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (list->size == 0) {
        errno = ENOENT;
        return -1;
    }

    *out = list->head->data;
    return 0;
}

/* Peek at the back of the list. Store data in an out variable */
int dlist_peek_back(const DList *list, void **out) {
    if (list == NULL || out == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (list->size == 0) {
        errno = ENOENT;
        return -1;
    }

    *out = list->tail->data;
    return 0;
}

/* Pop the first node off the front of the list. Store the data in *out.
 * Return -1 on failure, 0 on success. */
int dlist_pop_front(DList *list, void **out) {
    if (list == NULL || out == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (list->size == 0) {
        errno = ENOENT;
        return -1;
    }

    DListNode *victim = list->head;
    *out = victim->data;
    list->head = victim->next;
    /* Case 1: single element list, head now points to NULL */
    if (list->size == 1)
        list->tail = NULL;
    /* Case 2: multiple elements in list, head points to next node */
    else
        list->head->prev = NULL;

    free(victim);
    list->size--;
    return 0;
}



















