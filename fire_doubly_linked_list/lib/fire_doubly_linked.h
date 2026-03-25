#ifndef ___FIRE_DOUBLY_LINKED_H__
#define ___FIRE_DOUBLY_LINKED_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @file fire_doubly_linked.h
 * @brief Doubly linked list helpers with positional deletion.
 */

#define FIRE_DL_BUGVIEW 1
#define FIRE_DL_DEBUG(fmt, ...) do { \
 if (!FIRE_DL_BUGVIEW) { break; } \
 time_t t = time(NULL); \
 char *d = ctime(&t); \
 fprintf(stderr, "\n---Fire doubly linked list DEBUG-START ---\n\n %.*s %s[%d] %s(): \n", \
 (int)strlen(d) - 1, d, __FILE__, __LINE__, __func__); \
 fprintf(stderr, fmt, ##__VA_ARGS__); \
 fprintf(stderr, "\n\n---Fire doubly linked list DEBUG-END ---\n"); \
} while (0)

#define XFREE_FIRE_DL(x) fire_dl_xfree((void **)(x))

struct fire_dl_node {
    void *data;
    struct fire_dl_node *next;
    struct fire_dl_node *prev;
    int position;
};

typedef struct fire_dl_node fire_dl_node;

void fire_dl_xfree(void **ptr);
void *fire_dl_xmalloc(size_t size);
fire_dl_node *fire_create_dl_node(int position, void *data);
fire_dl_node *Get_head_dl_position(fire_dl_node *in);
fire_dl_node *Insert_dl_in_last(fire_dl_node *in, int position, void *data);
fire_dl_node *Delete_dl_by_position(int position, fire_dl_node *list, void (*lambda)(void *argvs));
void fifo_dl_list_dbg(fire_dl_node *in);
void lifo_dl_list_dbg(fire_dl_node *in);
void inter_fifo_dl_list(fire_dl_node *in, void (*lambda)(void *argvs));
void inter_lifo_dl_list(fire_dl_node *in, void (*lambda)(void *argvs));
void fire_clear_dl(fire_dl_node *in, void (*lambda)(void *argvs));

#endif
