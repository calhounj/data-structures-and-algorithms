#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

/* Simple test counters */
static int tests_run = 0;
static int tests_passed = 0;

/* Small helper macro */
#define RUN_TEST(fn)                                                   \
    do {                                                               \
        ++tests_run;                                                   \
        printf("Running %s... ", #fn);                                 \
        fn();                                                          \
        ++tests_passed;                                                \
        printf("PASS\n");                                              \
    } while (0)

/* ---------- Traversal test helpers ---------- */

static int visited_keys[128];
static size_t visited_count = 0;

static void reset_visit_buffer(void) {
    visited_count = 0;
}

static void record_key(int key) {
    if (visited_count < 128) {
        visited_keys[visited_count++] = key;
    }
}

/* ---------- Basic tests ---------- */

static void test_bst_init(void) {
    BST tree;
    int rc;

    rc = bst_init(&tree);
    assert(rc == 0);
    assert(tree.root == NULL);
    assert(tree.size == 0);
}

static void test_insert_empty_tree(void) {
    BST tree;
    int rc;

    rc = bst_init(&tree);
    assert(rc == 0);

    rc = bst_insert(&tree, 10);
    assert(rc == 0);

    assert(tree.root != NULL);
    assert(tree.root->key == 10);
    assert(tree.root->left == NULL);
    assert(tree.root->right == NULL);
    assert(tree.size == 1);

    bst_destroy(&tree);
    assert(tree.root == NULL);
    assert(tree.size == 0);
}

static void test_insert_left_and_right(void) {
    BST tree;
    int rc;

    rc = bst_init(&tree);
    assert(rc == 0);

    assert(bst_insert(&tree, 10) == 0);
    assert(bst_insert(&tree, 5) == 0);
    assert(bst_insert(&tree, 20) == 0);

    assert(tree.root != NULL);
    assert(tree.root->key == 10);

    assert(tree.root->left != NULL);
    assert(tree.root->left->key == 5);

    assert(tree.root->right != NULL);
    assert(tree.root->right->key == 20);

    assert(tree.size == 3);

    bst_destroy(&tree);
}

static void test_insert_duplicate(void) {
    BST tree;
    int rc;
    size_t old_size;

    rc = bst_init(&tree);
    assert(rc == 0);

    assert(bst_insert(&tree, 10) == 0);
    old_size = tree.size;

    rc = bst_insert(&tree, 10);
    assert(rc == 0);           /* based on your current duplicate policy */
    assert(tree.size == old_size);

    assert(tree.root != NULL);
    assert(tree.root->key == 10);
    assert(tree.root->left == NULL);
    assert(tree.root->right == NULL);

    bst_destroy(&tree);
}

/* ---------- Search tests ---------- */

static void test_search_found_and_not_found(void) {
    BST tree;
    BSTNode *node;
    int rc;

    rc = bst_init(&tree);
    assert(rc == 0);

    assert(bst_insert(&tree, 10) == 0);
    assert(bst_insert(&tree, 5) == 0);
    assert(bst_insert(&tree, 20) == 0);
    assert(bst_insert(&tree, 15) == 0);

    node = bst_search(&tree, 10);
    assert(node != NULL);
    assert(node->key == 10);

    node = bst_search(&tree, 5);
    assert(node != NULL);
    assert(node->key == 5);

    node = bst_search(&tree, 20);
    assert(node != NULL);
    assert(node->key == 20);

    node = bst_search(&tree, 999);
    assert(node == NULL);

    bst_destroy(&tree);
}

/* ---------- Traversal tests ---------- */

static void test_inorder_sorted(void) {
    BST tree;
    int rc;

    rc = bst_init(&tree);
    assert(rc == 0);

    assert(bst_insert(&tree, 10) == 0);
    assert(bst_insert(&tree, 5) == 0);
    assert(bst_insert(&tree, 20) == 0);
    assert(bst_insert(&tree, 3) == 0);
    assert(bst_insert(&tree, 7) == 0);
    assert(bst_insert(&tree, 15) == 0);
    assert(bst_insert(&tree, 30) == 0);

    reset_visit_buffer();
    bst_inorder(&tree, record_key);

    assert(visited_count == 7);
    assert(visited_keys[0] == 3);
    assert(visited_keys[1] == 5);
    assert(visited_keys[2] == 7);
    assert(visited_keys[3] == 10);
    assert(visited_keys[4] == 15);
    assert(visited_keys[5] == 20);
    assert(visited_keys[6] == 30);

    bst_destroy(&tree);
}

/* ---------- Min / Max tests ---------- */

static void test_min_max(void) {
    BST tree;
    int min;
    int max;
    int rc;

    rc = bst_init(&tree);
    assert(rc == 0);

    assert(bst_insert(&tree, 10) == 0);
    assert(bst_insert(&tree, 5) == 0);
    assert(bst_insert(&tree, 20) == 0);
    assert(bst_insert(&tree, 3) == 0);
    assert(bst_insert(&tree, 30) == 0);

    assert(bst_min(&tree, &min) == 0);
    assert(min == 3);

    assert(bst_max(&tree, &max) == 0);
    assert(max == 30);

    bst_destroy(&tree);
}

/* ---------- Error-handling tests ---------- */

static void test_insert_null_tree(void) {
    errno = 0;
    assert(bst_insert(NULL, 10) == -1);
    assert(errno == EINVAL);
}

/* ---------- main ---------- */

int main(void) {
    RUN_TEST(test_bst_init);
    RUN_TEST(test_insert_empty_tree);
    RUN_TEST(test_insert_left_and_right);
    RUN_TEST(test_insert_duplicate);
    RUN_TEST(test_search_found_and_not_found);
    RUN_TEST(test_inorder_sorted);
    RUN_TEST(test_min_max);
    RUN_TEST(test_insert_null_tree);

    printf("\n%d/%d tests passed\n", tests_passed, tests_run);
    return EXIT_SUCCESS;
}
