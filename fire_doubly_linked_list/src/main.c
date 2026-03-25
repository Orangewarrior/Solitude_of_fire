#define _POSIX_C_SOURCE 200809L

/**
 * @file main.c
 * @brief Full example program for the fire_doubly_linked_list module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/fire_doubly_linked.h"

struct data {
    char *label;
    char *var_name;
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


void *form_data(char *label, char *varname)
{
    data *in = fire_dl_xmalloc(sizeof(data));
    if (in == NULL) return NULL;
    in->label = fire_strdup(label);
    in->var_name = fire_strdup(varname);
    if (in->label == NULL || in->var_name == NULL) {
        free(in->label);
        free(in->var_name);
        free(in);
        return NULL;
    }
    return in;
}

void inter_data(void *argv)
{
    data *in = (data *)argv;
    if (in != NULL)
        printf("Label: %s\n var_name: %s\n", in->label, in->var_name);
}

void inter_free(void *argv)
{
    data *in = (data *)argv;
    if (in != NULL) {
        free(in->label);
        free(in->var_name);
        free(in);
    }
}

int main(void)
{
    fire_dl_node *n = fire_create_dl_node(0, form_data("hulk", "green"));
    Insert_dl_in_last(n, 2, form_data("spider", "red"));
    Insert_dl_in_last(n, 4, form_data("daredevil", "red"));
    Insert_dl_in_last(n, 6, form_data("wolverine", "yellow"));
    Insert_dl_in_last(n, 8, form_data("hellboy", "red"));
    Insert_dl_in_last(n, 10, form_data("deadpool", "dark red"));

    fifo_dl_list_dbg(n);
    lifo_dl_list_dbg(n);

    puts("Delete position 8");
    n = Delete_dl_by_position(8, n, inter_free);

    inter_fifo_dl_list(n, inter_data);
    puts("----------------------------");
    inter_lifo_dl_list(n, inter_data);

    fire_clear_dl(n, inter_free);
    return 0;
}
