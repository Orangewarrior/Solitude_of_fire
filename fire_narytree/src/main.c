#define _POSIX_C_SOURCE 200809L

/**
 * @file main.c
 * @brief Full example program for the fire_narytree module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../lib/fire_narytree.h"

struct data {
    bool taint;
    char *label;
    char *exp;
    char *var_name;
    int line;
};

typedef struct data data;

static char *fire_strdup(const char *s)
{
    size_t len;
    char *copy;

    if (s == NULL) {
        return NULL;
    }

    len = strlen(s) + 1;
    copy = (char *)malloc(len);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, s, len);
    return copy;
}


void *form_data(bool taint, char *label, char *exp, char *varname, int line)
{
    data *in = firetree_xmalloc(sizeof(data));
    if (in == NULL) return NULL;
    in->label = fire_strdup(label);
    in->var_name = fire_strdup(varname);
    in->exp = fire_strdup(exp);
    in->line = line;
    in->taint = taint;
    if (in->label == NULL || in->var_name == NULL || in->exp == NULL) {
        free(in->label);
        free(in->var_name);
        free(in->exp);
        free(in);
        return NULL;
    }
    return in;
}

void inter_data(void *argv)
{
    data *in = (data *)argv;
    if (in != NULL) {
        printf("Label: %s\n var_name: %s\n Expression: %s\n", in->label, in->var_name, in->exp);
        printf("Line: %d\n ", in->line);
    }
}

void inter_free_treeleafs(void *argv)
{
    data *in = (data *)argv;
    if (in != NULL) {
        free(in->label);
        free(in->var_name);
        free(in->exp);
        free(in);
    }
}

int main(void)
{
    fire_tree_node *root = new_node(0, form_data(false, "attribute", "x=0", "x", 10));
    fire_tree_node *ret = NULL;

    append_child(root, 1, form_data(false, "attribute", "x+=2", "x", 11));
    append_child(root, 2, form_data(false, "attribute", "x+=8", "x", 12));
    append_sibling(root, 3, form_data(false, "attribute", "z+=8", "z", 14));
    append_child(root, 4, form_data(false, "attribute", "z+=12", "z", 15));

    traversal_tree_dbg(root);

    ret = search_in_tree_per_position(root, 3);
    if (ret != NULL)
        printf("\nResult found: %d \n", ret->position);

    insert_child_before(root, 2, 5, form_data(false, "sum", "y+=2", "y", 16));
    insert_child_before(root, 3, 6, form_data(false, "sum", "k+=2", "y", 17));

    if (remove_node_childs_by_position(root, 2, inter_free_treeleafs))
        puts("its ok item remove item 12 and childs like 16\n");

    interate_traversal_tree(root, inter_data);
    traversal_and_destroy(root, inter_free_treeleafs);
    return 0;
}
