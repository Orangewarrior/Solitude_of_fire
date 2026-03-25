#include "fire_queue.h"

/**
 * @file fire_queue.c
 * @brief Internal implementation of the FIFO queue.
 */

static void *fire_queue_xmalloc_fatal(size_t size)
{
    FIRE_QUEUE_DEBUG("memory allocation failure, size=%zu", size);
    return NULL;
}

void *fire_queue_xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL) {
        return fire_queue_xmalloc_fatal(size);
    }
    return ptr;
}

Fire_Queue fire_init_queue(void)
{
    Fire_Queue qp = (Fire_Queue)fire_queue_xmalloc(sizeof(Fire_QueueType));
    if (qp == NULL) return NULL;
    qp->head = NULL;
    qp->tail = NULL;
    return qp;
}

int fire_queue_empty(const Fire_Queue Q)
{
    return (Q == NULL || Q->head == NULL);
}

int fire_enqueue(Fire_Queue Q, void *d)
{
    Fire_NodePtr_queue np;
    if (Q == NULL) return FIRE_QUEUE_ERR_INVAL;
    np = (Fire_NodePtr_queue)fire_queue_xmalloc(sizeof(Fire_Node_queue));
    if (np == NULL) return FIRE_QUEUE_ERR_NOMEM;
    np->data = d;
    np->next = NULL;

    if (fire_queue_empty(Q)) {
        Q->head = np;
        Q->tail = np;
    } else {
        Q->tail->next = np;
        Q->tail = np;
    }
    return FIRE_QUEUE_OK;
}

int fire_queue_dequeue_data(Fire_Queue Q, void **out_data)
{
    Fire_NodePtr_queue temp;
    if (Q == NULL || out_data == NULL) return FIRE_QUEUE_ERR_INVAL;
    if (fire_queue_empty(Q)) {
        *out_data = NULL;
        return FIRE_QUEUE_ERR_EMPTY;
    }

    temp = Q->head;
    *out_data = temp->data;
    Q->head = Q->head->next;
    if (Q->head == NULL) Q->tail = NULL;
    free(temp);
    return FIRE_QUEUE_OK;
}

void fire_dequeue(Fire_Queue Q, void (*lambda)(void *argvs))
{
    void *data = NULL;
    if (fire_queue_dequeue_data(Q, &data) != FIRE_QUEUE_OK) return;
    if (lambda != NULL) lambda(data);
}

void fire_traversal_queue(const Fire_Queue Q, void (*lambda)(void *argvs))
{
    Fire_NodePtr_queue cur;
    if (Q == NULL || lambda == NULL) return;
    cur = Q->head;
    while (cur != NULL) {
        lambda(cur->data);
        cur = cur->next;
    }
}

void fire_queue_clear(Fire_Queue Q, void (*destroy)(void *))
{
    void *data = NULL;
    if (Q == NULL) return;
    while (fire_queue_dequeue_data(Q, &data) == FIRE_QUEUE_OK) {
        if (destroy != NULL) destroy(data);
    }
}

void fire_queue_destroy(Fire_Queue *Q, void (*destroy)(void *))
{
    if (Q == NULL || *Q == NULL) return;
    fire_queue_clear(*Q, destroy);
    free(*Q);
    *Q = NULL;
}
