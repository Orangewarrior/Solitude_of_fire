#ifndef FIRE_VECTOR_MEM_OPS_H
#define FIRE_VECTOR_MEM_OPS_H

#include <stddef.h>

/**
 * @file mem_ops.h
 * @brief Allocation helpers for the vector module.
 */

#define FIRE_XFREE(x) fire_xfree((void **)(x))
#define MUL_NO_OVERFLOW ((size_t)1 << (sizeof(size_t) * 4))

void *fire_xmalloc(size_t size);
void *fire_xcalloc(size_t mem, size_t size);
void *fire_xrealloc(void *ptr, size_t size);
void *fire_xmallocarray(size_t nmemb, size_t size);
void *fire_xreallocarray(void *ptr, size_t nmemb, size_t size);
void fire_xfree(void **ptr);

#endif
