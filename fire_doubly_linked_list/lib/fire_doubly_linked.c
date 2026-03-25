#include "fire_doubly_linked.h"

/**
 * @file fire_doubly_linked.c
 * @brief Internal implementation of the doubly linked list.
 */

void fire_dl_xfree(void **ptr)
{
    if (ptr != NULL && *ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }
}

static void *fire_dl_xmalloc_fatal(size_t size)
{
    FIRE_DL_DEBUG("memory allocation failure, size=%zu", size);
    return NULL;
}

void *fire_dl_xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL) return fire_dl_xmalloc_fatal(size);
    return ptr;
}

fire_dl_node *fire_create_dl_node(int position, void *data)
{
    fire_dl_node *n = (fire_dl_node *)fire_dl_xmalloc(sizeof(*n));
    if (n == NULL) return NULL;
    n->data = data;
    n->position = position;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

fire_dl_node *Get_head_dl_position(fire_dl_node *in)
{
    while (in != NULL && in->prev != NULL) in = in->prev;
    return in;
}

fire_dl_node *Insert_dl_in_last(fire_dl_node *in, int position, void *data)
{
    fire_dl_node *new_node = fire_create_dl_node(position, data);
    if (new_node == NULL) return in;
    if (in == NULL) return new_node;
    in = Get_head_dl_position(in);
    while (in->next != NULL) in = in->next;
    in->next = new_node;
    new_node->prev = in;
    return new_node;
}

fire_dl_node *Delete_dl_by_position(int position, fire_dl_node *list, void (*lambda)(void *argvs))
{
    fire_dl_node *head = Get_head_dl_position(list);
    fire_dl_node *cur = head;

    while (cur != NULL) {
        if (cur->position == position) {
            fire_dl_node *next = cur->next;
            if (cur->prev != NULL) cur->prev->next = cur->next;
            else head = cur->next;
            if (cur->next != NULL) cur->next->prev = cur->prev;
            if (lambda != NULL) lambda(cur->data);
            free(cur);
            return next != NULL ? next : head;
        }
        cur = cur->next;
    }
    return head;
}

void fifo_dl_list_dbg(fire_dl_node *in)
{
    fire_dl_node *temp = Get_head_dl_position(in);
    while (temp != NULL) {
        printf("%d ", temp->position);
        temp = temp->next;
    }
    puts(" ");
}

void lifo_dl_list_dbg(fire_dl_node *in)
{
    fire_dl_node *temp = Get_head_dl_position(in);
    if (temp == NULL) return;
    while (temp->next != NULL) temp = temp->next;
    while (temp != NULL) {
        printf("%d ", temp->position);
        temp = temp->prev;
    }
    puts(" ");
}

void inter_fifo_dl_list(fire_dl_node *in, void (*lambda)(void *argvs))
{
    fire_dl_node *temp = Get_head_dl_position(in);
    if (lambda == NULL) return;
    while (temp != NULL) {
        lambda(temp->data);
        temp = temp->next;
    }
}

void inter_lifo_dl_list(fire_dl_node *in, void (*lambda)(void *argvs))
{
    fire_dl_node *temp = Get_head_dl_position(in);
    if (lambda == NULL || temp == NULL) return;
    while (temp->next != NULL) temp = temp->next;
    while (temp != NULL) {
        lambda(temp->data);
        temp = temp->prev;
    }
}

void fire_clear_dl(fire_dl_node *in, void (*lambda)(void *argvs))
{
    fire_dl_node *temp = Get_head_dl_position(in);
    while (temp != NULL) {
        fire_dl_node *next = temp->next;
        if (lambda != NULL) lambda(temp->data);
        free(temp);
        temp = next;
    }
}
