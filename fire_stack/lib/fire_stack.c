#include "fire_stack.h"

/**
 * @file fire_stack.c
 * @brief Internal implementation of the LIFO stack.
 */

static void *firestack_xmalloc_fatal(size_t size)
{
    FIRESTACK_DEBUG("memory allocation failure, size=%zu", size);
    return NULL;
}

void *firestack_xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL) return firestack_xmalloc_fatal(size);
    return ptr;
}

FireStack init_fire_stack(void)
{
    FireStack sp = (FireStack)firestack_xmalloc(sizeof(FireStackType));
    if (sp == NULL) return NULL;
    sp->top = NULL;
    return sp;
}

int fire_stack_empty(const FireStack S)
{
    return (S == NULL || S->top == NULL);
}

int fire_stack_push(FireStack S, void *in)
{
    firestack_NodePtr np;
    if (S == NULL) return FIRE_STACK_ERR_INVAL;
    np = (firestack_NodePtr)firestack_xmalloc(sizeof(firestack_Node));
    if (np == NULL) return FIRE_STACK_ERR_NOMEM;
    np->data = in;
    np->next = S->top;
    S->top = np;
    return FIRE_STACK_OK;
}

int fire_stack_pop_data(FireStack S, void **out_data)
{
    firestack_NodePtr temp;
    if (S == NULL || out_data == NULL) return FIRE_STACK_ERR_INVAL;
    if (fire_stack_empty(S)) {
        *out_data = NULL;
        return FIRE_STACK_ERR_EMPTY;
    }
    temp = S->top;
    *out_data = temp->data;
    S->top = temp->next;
    free(temp);
    return FIRE_STACK_OK;
}

void *fire_stack_pop(FireStack S, void (*lambda)(void *argvs))
{
    void *hold = NULL;
    if (fire_stack_pop_data(S, &hold) != FIRE_STACK_OK) return NULL;
    if (lambda != NULL) lambda(hold);
    return hold;
}

void fire_stack_inter_reverse_traversal(const FireStack S, void (*lambda)(void *argvs))
{
    firestack_NodePtr cur;
    if (S == NULL || lambda == NULL) return;
    cur = S->top;
    while (cur != NULL) {
        lambda(cur->data);
        cur = cur->next;
    }
}

void fire_stack_clear(FireStack S, void (*destroy)(void *))
{
    void *data = NULL;
    if (S == NULL) return;
    while (fire_stack_pop_data(S, &data) == FIRE_STACK_OK) {
        if (destroy != NULL) destroy(data);
    }
}

void fire_stack_destroy(FireStack *S, void (*destroy)(void *))
{
    if (S == NULL || *S == NULL) return;
    fire_stack_clear(*S, destroy);
    free(*S);
    *S = NULL;
}
