#ifndef __FIRE_NARYTREE_H__
#define __FIRE_NARYTREE_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @file fire_narytree.h
 * @brief Small n-ary tree API based on sibling/child representation.
 */

#define FIRETREE_BUGVIEW 1
#define FIRETREE_DEBUG(fmt, ...) do { \
 if (!FIRETREE_BUGVIEW) { break; } \
 time_t t = time(NULL); \
 char *d = ctime(&t); \
 fprintf(stderr, "\n---Fire Tree DEBUG-START ---\n\n %.*s %s[%d] %s(): \n", \
 (int)strlen(d) - 1, d, __FILE__, __LINE__, __func__); \
 fprintf(stderr, fmt, ##__VA_ARGS__); \
 fprintf(stderr, "\n\n---Fire Tree DEBUG-END ---\n"); \
} while (0)

#define XFREE_FIRETREE(x) firetree_xfree((void **)(x))

struct firetreenode {
    void *data;
    struct firetreenode *next;
    struct firetreenode *child;
    int position;
};

typedef struct firetreenode fire_tree_node;

void firetree_xfree(void **ptr);
void *firetree_xmalloc(size_t size);
void traversal_and_destroy(fire_tree_node *root, void (*lambda)(void *argvs));
fire_tree_node *new_node(int position, void *data);
fire_tree_node *append_sibling(fire_tree_node *n, int position, void *data);
fire_tree_node *append_child(fire_tree_node *n, int position, void *data);
fire_tree_node *search_in_tree_per_position(fire_tree_node *root, int position);
fire_tree_node *insert_child_before(fire_tree_node *root, int position, int new_position, void *data);
fire_tree_node *insert_sibling_before(fire_tree_node *root, int position, int new_position, void *data);
void interate_traversal_tree(fire_tree_node *root, void (*lambda)(void *argvs));
void traversal_tree_dbg(fire_tree_node *root);
bool remove_node_childs_by_position(fire_tree_node *root, int position, void (*lambda)(void *argvs));

#endif
