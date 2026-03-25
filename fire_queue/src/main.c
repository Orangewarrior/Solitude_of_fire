#define _POSIX_C_SOURCE 200809L

/**
 * @file main.c
 * @brief Full example program for the fire_queue module.
 *
 * This example preserves the original style:
 * - allocate a custom payload structure
 * - enqueue multiple records
 * - traverse all records
 * - dequeue records and explicitly free payloads
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/fire_queue.h"

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
    data *in = fire_queue_xmalloc(sizeof(data));
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
    Fire_Queue S = fire_init_queue();
    void *data = NULL;
    if (S == NULL) return 1;

    puts("\nInsert elements with custom data\n");
    fire_enqueue(S, form_data("hulk", 2));
    fire_enqueue(S, form_data("batman", 4));
    fire_enqueue(S, form_data("spiderman", 8));
    fire_enqueue(S, form_data("Dredd", 32));
    fire_enqueue(S, form_data("Spock", 16));

    puts("List elements");
    fire_traversal_queue(S, inter_data);

    puts("\nRemove elements / free all heap\n");
    while (fire_queue_dequeue_data(S, &data) == FIRE_QUEUE_OK) {
        inter_data(data);
        free_data(data);
    }

    fire_queue_destroy(&S, NULL);
    return 0;
}
