#include "mem_ops.h"
#include <stdlib.h>
#include <stdint.h>

static int mul_overflow(size_t a, size_t b, size_t *out)
{
    if (out == NULL) return -1;
    if ((a >= MUL_NO_OVERFLOW || b >= MUL_NO_OVERFLOW) && a > 0 && SIZE_MAX / a < b) {
        return -1;
    }
    *out = a * b;
    return 0;
}

void *fire_xmalloc(size_t size) { return malloc(size); }
void *fire_xcalloc(size_t mem, size_t size) { return calloc(mem, size); }
void *fire_xrealloc(void *ptr, size_t size) { return realloc(ptr, size); }

void *fire_xmallocarray(size_t nmemb, size_t size)
{
    size_t total = 0;
    if (mul_overflow(nmemb, size, &total) != 0) return NULL;
    return calloc(1, total);
}

void *fire_xreallocarray(void *ptr, size_t nmemb, size_t size)
{
    size_t total = 0;
    if (mul_overflow(nmemb, size, &total) != 0) return NULL;
    return realloc(ptr, total);
}

void fire_xfree(void **ptr)
{
    if (ptr != NULL && *ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }
}
