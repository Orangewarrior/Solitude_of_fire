#define _POSIX_C_SOURCE 200809L

/**
 * @file main.c
 * @brief Full example program for the fire_stack module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/fire_stack.h"

struct data {
    char *label;
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


void *form_data(char *label, int line)
{
    data *in = firestack_xmalloc(sizeof(data));
    if (in == NULL) return NULL;
    in->label = fire_strdup(label);
    in->line = line;
    if (in->label == NULL) {
        free(in);
        return NULL;
    }
    return in;
}

void inter_data(void *argv)
{
    data *in = (data *)argv;
    if (in != NULL)
        printf("Label: %s\n line: %d\n", in->label, in->line);
}

void free_data(void *argv)
{
    data *in = (data *)argv;
    if (in != NULL) {
        free(in->label);
        free(in);
    }
}

int main(void)
{
    FireStack S = init_fire_stack();
    void *data = NULL;
    if (S == NULL) return 1;

    puts("\nInsert elements with custom data\n");
    fire_stack_push(S, form_data("hulk", 2));
    fire_stack_push(S, form_data("batman", 4));
    fire_stack_push(S, form_data("spiderman", 8));
    fire_stack_push(S, form_data("Dredd", 32));
    fire_stack_push(S, form_data("Spock", 16));

    fire_stack_inter_reverse_traversal(S, inter_data);

    puts("\nRemove elements / free all heap\n");
    while (fire_stack_pop_data(S, &data) == FIRE_STACK_OK) {
        free_data(data);
    }

    fire_stack_destroy(&S, NULL);
    return 0;
}
