#include "fire_narytree.h"

/**
 * @file fire_narytree.c
 * @brief Internal implementation of the n-ary tree.
 */

void firetree_xfree(void **ptr)
{
    if (ptr != NULL && *ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }
}

static void *firetree_xmalloc_fatal(size_t size)
{
    FIRETREE_DEBUG("memory allocation failure, size=%zu", size);
    return NULL;
}

void *firetree_xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL) return firetree_xmalloc_fatal(size);
    return ptr;
}

void traversal_and_destroy(fire_tree_node *root, void (*lambda)(void *argvs))
{
    if (root == NULL) return;
    traversal_and_destroy(root->child, lambda);
    traversal_and_destroy(root->next, lambda);
    if (lambda != NULL) lambda(root->data);
    free(root);
}

void interate_traversal_tree(fire_tree_node *root, void (*lambda)(void *argvs))
{
    if (root == NULL) return;
    if (lambda != NULL) lambda(root->data);
    interate_traversal_tree(root->child, lambda);
    interate_traversal_tree(root->next, lambda);
}

fire_tree_node *search_in_tree_per_position(fire_tree_node *root, int position)
{
    fire_tree_node *ret;
    if (root == NULL) return NULL;
    if (root->position == position) return root;
    ret = search_in_tree_per_position(root->child, position);
    if (ret != NULL) return ret;
    return search_in_tree_per_position(root->next, position);
}

bool remove_node_childs_by_position(fire_tree_node *root, int position, void (*lambda)(void *argvs))
{
    fire_tree_node *current = search_in_tree_per_position(root, position);
    if (current == NULL) return false;
    traversal_and_destroy(current->child, lambda);
    current->child = NULL;
    return true;
}

fire_tree_node *insert_sibling_before(fire_tree_node *root, int position, int new_position, void *data)
{
    fire_tree_node *current = search_in_tree_per_position(root, position);
    if (current == NULL) return NULL;
    return append_sibling(current, new_position, data);
}

fire_tree_node *insert_child_before(fire_tree_node *root, int position, int new_position, void *data)
{
    fire_tree_node *current = search_in_tree_per_position(root, position);
    if (current == NULL) return NULL;
    if (current->child != NULL) return append_sibling(current->child, new_position, data);
    return (current->child = new_node(new_position, data));
}

void traversal_tree_dbg(fire_tree_node *root)
{
    if (root != NULL) {
        FIRETREE_DEBUG("Node Value : %d", root->position);
        traversal_tree_dbg(root->child);
        traversal_tree_dbg(root->next);
    }
}

fire_tree_node *new_node(int position, void *data)
{
    fire_tree_node *newn = firetree_xmalloc(sizeof(*newn));
    if (newn == NULL) return NULL;
    newn->next = NULL;
    newn->child = NULL;
    newn->data = data;
    newn->position = position;
    return newn;
}

fire_tree_node *append_sibling(fire_tree_node *n, int position, void *data)
{
    if (n == NULL) return NULL;
    while (n->next != NULL) n = n->next;
    return (n->next = new_node(position, data));
}

fire_tree_node *append_child(fire_tree_node *n, int position, void *data)
{
    if (n == NULL) return NULL;
    if (n->child != NULL) return append_sibling(n->child, position, data);
    return (n->child = new_node(position, data));
}
