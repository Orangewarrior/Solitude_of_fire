#ifndef FIRE_VECTOR_H
#define FIRE_VECTOR_H

#include <stddef.h>

/**
 * @file fire_vector.h
 * @brief Simple dynamic array of `void *`.
 */

#define fire_vector_LIMIT 1
#define FIRE_MAX_LEN_STR_CMP 512

typedef struct vector {
    void **elements;
    int limit;
    int max;
} fire_vector;

void fire_vector_init(fire_vector *v);
int  fire_vector_max(const fire_vector *v);
int  fire_vector_resize(fire_vector *v, int limit);
void fire_vector_swap(fire_vector *v, int index, int index2);
void fire_vector_write(fire_vector *v, int index, void *element);
int  fire_vector_add(fire_vector *v, void *element);
void *fire_vector_view(const fire_vector *v, int index);
void fire_vector_free(fire_vector *v);
void fire_vector_remove(fire_vector *v, int index);
void fire_vector_pop(fire_vector *v);

int fire_cmp_str(const void *a, const void *b);
int fire_cmp_int(const void *a, const void *b);
int fire_cmp_double(const void *a, const void *b);
int fire_cmp_float(const void *a, const void *b);

#endif
