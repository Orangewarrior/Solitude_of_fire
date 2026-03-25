#ifndef __FIRE_QUEUE_H__
#define __FIRE_QUEUE_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @file fire_queue.h
 * @brief FIFO queue API for opaque pointers.
 */

#define FIRE_QUEUE_BUGVIEW 1
#define FIRE_QUEUE_DEBUG(fmt, ...) do { \
 if (!FIRE_QUEUE_BUGVIEW) { break; } \
 time_t t = time(NULL); \
 char *d = ctime(&t); \
 fprintf(stderr, "\n---Fire Queue DEBUG-START ---\n\n %.*s %s[%d] %s(): \n", \
 (int)strlen(d) - 1, d, __FILE__, __LINE__, __func__); \
 fprintf(stderr, fmt, ##__VA_ARGS__); \
 fprintf(stderr, "\n\n---Fire Queue DEBUG-END ---\n"); \
} while (0)

typedef enum fire_queue_status {
    FIRE_QUEUE_OK = 0,
    FIRE_QUEUE_ERR_INVAL = -1,
    FIRE_QUEUE_ERR_EMPTY = -2,
    FIRE_QUEUE_ERR_NOMEM = -3
} fire_queue_status;

typedef struct fire_queue_node {
    void *data;
    struct fire_queue_node *next;
} Fire_Node_queue, *Fire_NodePtr_queue;

typedef struct fire_queueType {
    Fire_NodePtr_queue head, tail;
} Fire_QueueType, *Fire_Queue;

void *fire_queue_xmalloc(size_t size);
Fire_Queue fire_init_queue(void);
int fire_queue_empty(const Fire_Queue Q);
int fire_enqueue(Fire_Queue Q, void *d);
int fire_queue_dequeue_data(Fire_Queue Q, void **out_data);
void fire_dequeue(Fire_Queue Q, void (*lambda)(void *argvs));
void fire_traversal_queue(const Fire_Queue Q, void (*lambda)(void *argvs));
void fire_queue_clear(Fire_Queue Q, void (*destroy)(void *));
void fire_queue_destroy(Fire_Queue *Q, void (*destroy)(void *));

#endif
