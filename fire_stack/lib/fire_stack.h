#ifndef __FIRE_STACK_H__
#define __FIRE_STACK_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @file fire_stack.h
 * @brief LIFO stack API for opaque pointers.
 */

#define FIRESTACK_BUGVIEW 1
#define FIRESTACK_DEBUG(fmt, ...) do { \
 if (!FIRESTACK_BUGVIEW) { break; } \
 time_t t = time(NULL); \
 char *d = ctime(&t); \
 fprintf(stderr, "\n---Fire Stack DEBUG-START ---\n\n %.*s %s[%d] %s(): \n", \
 (int)strlen(d) - 1, d, __FILE__, __LINE__, __func__); \
 fprintf(stderr, fmt, ##__VA_ARGS__); \
 fprintf(stderr, "\n\n---Fire Stack DEBUG-END ---\n"); \
} while (0)

typedef enum fire_stack_status {
    FIRE_STACK_OK = 0,
    FIRE_STACK_ERR_INVAL = -1,
    FIRE_STACK_ERR_EMPTY = -2,
    FIRE_STACK_ERR_NOMEM = -3
} fire_stack_status;

typedef struct firestack_node {
    void *data;
    struct firestack_node *next;
} firestack_Node, *firestack_NodePtr;

typedef struct firestackType {
    firestack_NodePtr top;
} FireStackType, *FireStack;

void *firestack_xmalloc(size_t size);
FireStack init_fire_stack(void);
int fire_stack_empty(const FireStack S);
int fire_stack_push(FireStack S, void *in);
int fire_stack_pop_data(FireStack S, void **out_data);
void *fire_stack_pop(FireStack S, void (*lambda)(void *argvs));
void fire_stack_inter_reverse_traversal(const FireStack S, void (*lambda)(void *argvs));
void fire_stack_clear(FireStack S, void (*destroy)(void *));
void fire_stack_destroy(FireStack *S, void (*destroy)(void *));

#endif
