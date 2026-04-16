#ifndef _DLIST_H
#define _DLIST_H

typedef struct DListNode {
    void *data;
    struct DListNode *prev;
    struct DListNode *next;
} DListNode;

typedef struct DList {
    DListNode *head;
    DListNode *tail;
    size_t size;
} DList;

/* Return value policy:
 *
 * Return 0 on success, -1 on failure and set errno.
 * Pure boolean queries, return 1 or 0.
 * Size queries, return size. */

/* Initialize a doubly linked-list. Return 0 on success and -1 on failure */
int dlist_init(DList *list);
/* Check if a list is empty. Return 1 on true and 0 on false, -1 on failure */
int dlist_is_empty(const DList *list);
/* Report on size of a list. list->size stored in out */
int dlist_size(const DList *list, size_t *out);
/* Push a node to the front of a list */
int dlist_push_front(DList *list, void *data);
/* Append a node to the end of a list */
int dlist_push_back(DList *list, void *data);
/* Peek at the front of the list */
int dlist_peek_front(const DList *list, void **out);
/* Peek at the end of the list */
int dlist_peek_back(const DList *list, void **out);
/* Pop the front of the list and retrieve the data. Free the node, not data */
int dlist_pop_front(DList *list, void **out);
/* Pop the back of the list and retrieve the data. Free the node, not data */
int dlist_pop_back(DList *list, void **out);
/* Destroy the entire list. -1 Fail, 0 Succeed */
int dlist_destroy(DList *list, void (*free_fn)(void *));
/* Remove a designated node from the list. Standard return values. */
int dlist_remove_node(DList *list, DListNode *node, void **out);
/* Wrapper for destroy */
int dlist_clear(DList *list);







#endif
