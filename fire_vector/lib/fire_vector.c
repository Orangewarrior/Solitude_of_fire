#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fire_vector.h"
#include "mem_ops.h"

void fire_vector_init(fire_vector *v)
{
    if (v == NULL) return;
    v->limit = fire_vector_LIMIT;
    v->max = 0;
    v->elements = fire_xmallocarray((size_t)v->limit, sizeof(void *));
}

int fire_vector_max(const fire_vector *v)
{
    return (v != NULL) ? v->max : 0;
}

int fire_vector_resize(fire_vector *v, int limit)
{
    void **elements;
    if (v == NULL || limit <= 0) return -1;
    elements = fire_xreallocarray(v->elements, (size_t)limit, sizeof(void *));
    if (elements == NULL) return -1;
    v->elements = elements;
    v->limit = limit;
    return 0;
}

void fire_vector_write(fire_vector *v, int index, void *element)
{
    if (v != NULL && index >= 0 && index < v->max) v->elements[index] = element;
}

void fire_vector_swap(fire_vector *v, int index, int index2)
{
    void *tmp;
    if (v == NULL || index < 0 || index >= v->max || index2 < 0 || index2 >= v->max) return;
    tmp = v->elements[index];
    v->elements[index] = v->elements[index2];
    v->elements[index2] = tmp;
}

int fire_vector_add(fire_vector *v, void *element)
{
    if (v == NULL) return -1;
    if (v->limit == v->max && fire_vector_resize(v, v->limit * 2) != 0) return -1;
    v->elements[v->max++] = element;
    return 0;
}

void *fire_vector_view(const fire_vector *v, int index)
{
    if (v != NULL && index >= 0 && index < v->max) return v->elements[index];
    return NULL;
}

void fire_vector_free(fire_vector *v)
{
    if (v == NULL) return;
    FIRE_XFREE(&v->elements);
    v->limit = 0;
    v->max = 0;
}

void fire_vector_remove(fire_vector *v, int index)
{
    int i;
    if (v == NULL || index < 0 || index >= v->max) return;
    for (i = index; i < v->max - 1; i++) {
        v->elements[i] = v->elements[i + 1];
    }
    v->elements[v->max - 1] = NULL;
    v->max--;
    if (v->max > 0 && v->max == v->limit / 4) {
        (void)fire_vector_resize(v, v->limit / 2);
    }
}

void fire_vector_pop(fire_vector *v)
{
    if (v == NULL || v->max == 0) return;
    fire_vector_remove(v, v->max - 1);
}

int fire_cmp_str(const void *a, const void *b)
{
    const char *sa = *(const char * const *)a;
    const char *sb = *(const char * const *)b;
    if (sa == NULL && sb == NULL) return 0;
    if (sa == NULL) return -1;
    if (sb == NULL) return 1;
    return strcmp(sa, sb);
}

int fire_cmp_int(const void *a, const void *b)
{
    const int ia = *(const int *)a;
    const int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

int fire_cmp_double(const void *a, const void *b)
{
    const double ia = *(const double *)a;
    const double ib = *(const double *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

int fire_cmp_float(const void *a, const void *b)
{
    const float ia = *(const float *)a;
    const float ib = *(const float *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}
