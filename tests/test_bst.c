#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

static int tests_run = 0;
static int tests_passed = 0;

#define RUN_TEST(fn)                                      \
    do {                                                  \
        ++tests_run;                                      \
        printf("Running %s... ", #fn);                    \
        fn();                                             \
        ++tests_passed;                                   \
        printf("PASS\n");                                 \
    } while (0)

/* ------------------------------------------------------------------
 * Traversal callback helpers
 * ------------------------------------------------------------------ */

static int visited[128];
static size_t visited_count = 0;

static void reset_visited(void) {
    visited_count = 0;
}

static void record_key(int key) {
    assert(visited_count < 128);
    visited[visited_count++] = key;
}

/* ------------------------------------------------------------------
 * Basic initialization / size tests
 * ------------------------------------------------------------------ */

static void test_bst_init(void) {
    BST tree;
    int rc;

    rc = bst_init(&tree);
    assert(rc == 0);
    assert(tree.root == NULL);
    assert(tree.size == 0);
    assert(bst_size(&tree) == 0);
}

static void test_bst_init_null(void) {
    errno = 0;
    assert(bst_init(NULL) == -1);
    assert(errno == EINVAL);
}

static void test_bst_size_null(void) {
    assert(bst_size(NULL) == 0);
}

/* ------------------------------------------------------------------
 * Insert tests
 * ------------------------------------------------------------------ */

static void test_insert_empty_tree(void) {
    BST tree;

    assert(bst_init(&tree) == 0);
    assert(bst_insert(&tree, 10) == 0);

    assert(tree.root != NULL);
    assert(tree.root->key == 10);
    assert(tree.root->left == NULL);
    assert(tree.root->right == NULL);
    assert(bst_size(&tree) == 1);

    bst_destroy(&tree);
}

static void test_insert_left_and_right(void) {
    BST tree;

    assert(bst_init(&tree) == 0);

    assert(bst_insert(&tree, 10) == 0);
    assert(bst_insert(&tree, 5) == 0);
    assert(bst_insert(&tree, 20) == 0);

    assert(tree.root != NULL);
    assert(tree.root->key == 10);

    assert(tree.root->left != NULL);
    assert(tree.root->left->key == 5);

    assert(tree.root->right != NULL);
    assert(tree.root->right->key == 20);

    assert(bst_size(&tree) == 3);

    bst_destroy(&tree);
}

static void test_insert_duplicate(void) {
    BST tree;
    size_t old_size;

    assert(bst_init(&tree) == 0);
    assert(bst_insert(&tree, 10) == 0);

    old_size = bst_size(&tree);

    assert(bst_insert(&tree, 10) == 0);
    assert(bst_size(&tree) == old_size);

    assert(tree.root != NULL);
    assert(tree.root->key == 10);
    assert(tree.root->left == NULL);
    assert(tree.root->right == NULL);

    bst_destroy(&tree);
}

static void test_insert_null_tree(void) {
    errno = 0;
    assert(bst_insert(NULL, 42) == -1);
    assert(errno == EINVAL);
}

/* ------------------------------------------------------------------
 * Search tests
 * ------------------------------------------------------------------ */

static void test_search_empty_tree(void) {
    BST tree;

    assert(bst_init(&tree) == 0);
    assert(bst_search(&tree, 10) == NULL);

    bst_destroy(&tree);
}

static void test_search_found_root(void) {
    BST tree;
    BSTNode *node;

    assert(bst_init(&tree) == 0);
    assert(bst_insert(&tree, 10) == 0);

    node = bst_search(&tree, 10);
    assert(node != NULL);
    assert(node->key == 10);

    bst_destroy(&tree);
}

static void test_search_found_internal_and_leaf(void) {
    BST tree;
    BSTNode *node;

    assert(bst_init(&tree) == 0);
    assert(bst_insert(&tree, 10) == 0);
    assert(bst_insert(&tree, 5) == 0);
    assert(bst_insert(&tree, 20) == 0);
    assert(bst_insert(&tree, 15) == 0);
    assert(bst_insert(&tree, 30) == 0);

    node = bst_search(&tree, 20);
    assert(node != NULL);
    assert(node->key == 20);

    node = bst_search(&tree, 15);
    assert(node != NULL);
    assert(node->key == 15);

    node = bst_search(&tree, 999);
    assert(node == NULL);

    bst_destroy(&tree);
}

static void test_search_null_tree(void) {
    assert(bst_search(NULL, 10) == NULL);
}

/* ------------------------------------------------------------------
 * Min / max tests
 * ------------------------------------------------------------------ */

static void test_min_max_single_node(void) {
    BST tree;
    int min;
    int max;

    assert(bst_init(&tree) == 0);
    assert(bst_insert(&tree, 10) == 0);

    assert(bst_min(&tree, &min) == 0);
    assert(min == 10);

    assert(bst_max(&tree, &max) == 0);
    assert(max == 10);

    bst_destroy(&tree);
}

static void test_min_max_multi_node(void) {
    BST tree;
    int min;
    int max;

    assert(bst_init(&tree) == 0);
    assert(bst_insert(&tree, 10) == 0);
    assert(bst_insert(&tree, 5) == 0);
    assert(bst_insert(&tree, 20) == 0);
    assert(bst_insert(&tree, 3) == 0);
    assert(bst_insert(&tree, 7) == 0);
    assert(bst_insert(&tree, 30) == 0);

    assert(bst_min(&tree, &min) == 0);
    assert(min == 3);

    assert(bst_max(&tree, &max) == 0);
    assert(max == 30);

    bst_destroy(&tree);
}

static void test_min_empty_tree(void) {
    BST tree;
    int out = 0;

    assert(bst_init(&tree) == 0);

    errno = 0;
    assert(bst_min(&tree, &out) == -1);
    assert(errno == ENOENT);

    bst_destroy(&tree);
}

static void test_max_empty_tree(void) {
    BST tree;
    int out = 0;

    assert(bst_init(&tree) == 0);

    errno = 0;
    assert(bst_max(&tree, &out) == -1);
    assert(errno == ENOENT);

    bst_destroy(&tree);
}

static void test_min_max_bad_args(void) {
    BST tree;
    int out = 0;

    assert(bst_init(&tree) == 0);

    errno = 0;
    assert(bst_min(NULL, &out) == -1);
    assert(errno == EINVAL);

    errno = 0;
    assert(bst_min(&tree, NULL) == -1);
    assert(errno == EINVAL);

    errno = 0;
    assert(bst_max(NULL, &out) == -1);
    assert(errno == EINVAL);

    errno = 0;
    assert(bst_max(&tree, NULL) == -1);
    assert(errno == EINVAL);

    bst_destroy(&tree);
}

/* ------------------------------------------------------------------
 * Inorder traversal tests
 * ------------------------------------------------------------------ */

static void test_inorder_empty_tree(void) {
    BST tree;

    assert(bst_init(&tree) == 0);

    reset_visited();
    bst_inorder(&tree, record_key);

    assert(visited_count == 0);

    bst_destroy(&tree);
}

static void test_inorder_sorted_output(void) {
    BST tree;

    assert(bst_init(&tree) == 0);
    assert(bst_insert(&tree, 10) == 0);
    assert(bst_insert(&tree, 5) == 0);
    assert(bst_insert(&tree, 20) == 0);
    assert(bst_insert(&tree, 3) == 0);
    assert(bst_insert(&tree, 7) == 0);
    assert(bst_insert(&tree, 15) == 0);
    assert(bst_insert(&tree, 30) == 0);

    reset_visited();
    bst_inorder(&tree, record_key);

    assert(visited_count == 7);
    assert(visited[0] == 3);
    assert(visited[1] == 5);
    assert(visited[2] == 7);
    assert(visited[3] == 10);
    assert(visited[4] == 15);
    assert(visited[5] == 20);
    assert(visited[6] == 30);

    bst_destroy(&tree);
}

static void test_inorder_null_args(void) {
    BST tree;

    assert(bst_init(&tree) == 0);

    reset_visited();
    bst_inorder(NULL, record_key);
    assert(visited_count == 0);

    reset_visited();
    bst_inorder(&tree, NULL);
    assert(visited_count == 0);

    bst_destroy(&tree);
}

/* ------------------------------------------------------------------
 * Destroy tests
 * ------------------------------------------------------------------ */

static void test_destroy_empty_tree(void) {
    BST tree;

    assert(bst_init(&tree) == 0);
    bst_destroy(&tree);

    assert(tree.root == NULL);
    assert(tree.size == 0);
    assert(bst_size(&tree) == 0);
}

static void test_destroy_nonempty_tree(void) {
    BST tree;

    assert(bst_init(&tree) == 0);
    assert(bst_insert(&tree, 10) == 0);
    assert(bst_insert(&tree, 5) == 0);
    assert(bst_insert(&tree, 20) == 0);

    bst_destroy(&tree);

    assert(tree.root == NULL);
    assert(tree.size == 0);
    assert(bst_size(&tree) == 0);
}

static void test_destroy_null_tree(void) {
    bst_destroy(NULL);
}

/* My tests */

static void test_delete_node(void) {
    BST tree;

    assert(bst_init(&tree) == 0);
    /* Delete on empty tree */
    errno = 0;
    assert(bst_delete(&tree, 10) == -1);
    assert(errno = ENOENT);
    assert(tree.root == NULL);
    assert(tree.size == 0);
    /* Delete with missing key */
    assert(bst_insert(&tree, 10) == 0);
    assert(tree.root != NULL);
    assert(tree.root->key == 10);
    assert(tree.root->left == NULL);
    assert(tree.root->right == NULL);
    assert(bst_size(&tree) == 1);
    errno = 0;
    assert(bst_delete(&tree, 20) == -1);
    assert(errno == ENOENT);
    /* Delete leaf node */
    assert(bst_insert(&tree, 20) == 0);
    assert(bst_delete(&tree, 20) == 0);
    assert(tree.root->right == NULL);
    assert(tree.root->left == NULL);
    assert(tree.size == 1);

    /* Delete with immediate right child successor */
    bst_insert(&tree, 5);
    bst_insert(&tree, 20);
    bst_insert(&tree, 15);
    bst_insert(&tree, 12);
    assert(bst_delete(&tree, 10) == 0);

    reset_visited();
    bst_inorder(&tree, record_key);
    assert(visited_count = 4);
    assert(visited[0] == 5);
    assert(visited[1] == 12);
    assert(visited[2] == 15);
    assert(visited[3] == 20);

    bst_destroy(&tree);
}

/* ------------------------------------------------------------------
 * Preorder traversal tests
 * ------------------------------------------------------------------ */

static void test_preorder_empty_tree(void) {
    BST tree;

    assert(bst_init(&tree) == 0);

    reset_visited();
    bst_preorder(&tree, record_key);

    assert(visited_count == 0);

    bst_destroy(&tree);
}

static void test_preorder_sorted_output(void) {
    BST tree;

    assert(bst_init(&tree) == 0);
    assert(bst_insert(&tree, 10) == 0);
    assert(bst_insert(&tree, 5) == 0);
    assert(bst_insert(&tree, 20) == 0);
    assert(bst_insert(&tree, 3) == 0);
    assert(bst_insert(&tree, 7) == 0);
    assert(bst_insert(&tree, 15) == 0);
    assert(bst_insert(&tree, 30) == 0);

    reset_visited();
    bst_preorder(&tree, record_key);

    assert(visited_count == 7);
    assert(visited[0] == 10);
    assert(visited[1] == 5);
    assert(visited[2] == 3);
    assert(visited[3] == 7);
    assert(visited[4] == 20);
    assert(visited[5] == 15);
    assert(visited[6] == 30);

    bst_destroy(&tree);
}

static void test_preorder_null_args(void) {
    BST tree;

    assert(bst_init(&tree) == 0);

    reset_visited();
    bst_preorder(NULL, record_key);
    assert(visited_count == 0);

    reset_visited();
    bst_preorder(&tree, NULL);
    assert(visited_count == 0);

    bst_destroy(&tree);
}


/* ------------------------------------------------------------------
 * main
 * ------------------------------------------------------------------ */

int main(void) {
    RUN_TEST(test_bst_init);
    RUN_TEST(test_bst_init_null);
    RUN_TEST(test_bst_size_null);

    RUN_TEST(test_insert_empty_tree);
    RUN_TEST(test_insert_left_and_right);
    RUN_TEST(test_insert_duplicate);
    RUN_TEST(test_insert_null_tree);

    RUN_TEST(test_search_empty_tree);
    RUN_TEST(test_search_found_root);
    RUN_TEST(test_search_found_internal_and_leaf);
    RUN_TEST(test_search_null_tree);

    RUN_TEST(test_min_max_single_node);
    RUN_TEST(test_min_max_multi_node);
    RUN_TEST(test_min_empty_tree);
    RUN_TEST(test_max_empty_tree);
    RUN_TEST(test_min_max_bad_args);

    RUN_TEST(test_inorder_empty_tree);
    RUN_TEST(test_inorder_sorted_output);
    RUN_TEST(test_inorder_null_args);

    RUN_TEST(test_destroy_empty_tree);
    RUN_TEST(test_destroy_nonempty_tree);
    RUN_TEST(test_destroy_null_tree);

    /* My tests now */
    RUN_TEST(test_delete_node);
    RUN_TEST(test_preorder_empty_tree);
    RUN_TEST(test_preorder_sorted_output);
    RUN_TEST(test_preorder_null_args);

    printf("\n%d/%d tests passed\n", tests_passed, tests_run);
    return EXIT_SUCCESS;
}
