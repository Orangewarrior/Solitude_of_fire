/**
 * @file main.c
 * @brief Full example program for the fire_vector module.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../lib/fire_vector.h"

int main(void)
{
    int i = 0;
    fire_vector v;
    char *find = "avenger";
    char **search = NULL;

    fire_vector_init(&v);
    fire_vector_add(&v, "pacman");
    fire_vector_add(&v, "ghost");
    fire_vector_add(&v, "ball");
    fire_vector_add(&v, "triangle");
    fire_vector_add(&v, "square");
    fire_vector_add(&v, "avenger");
    fire_vector_add(&v, "dragon");
    fire_vector_add(&v, "dog");

    puts("First example show all elements");
    while (i < fire_vector_max(&v)) {
        printf("%s \n", (char *)fire_vector_view(&v, i));
        i++;
    }

    puts("\nTest swap element, Edit and remove");
    fire_vector_swap(&v, 0, 1);
    fire_vector_swap(&v, 2, 3);
    fire_vector_remove(&v, 7);
    fire_vector_write(&v, 3, "ball3");

    i = 0;
    while (i < fire_vector_max(&v)) {
        printf("%s \n", (char *)fire_vector_view(&v, i));
        i++;
    }

    i = 0;
    puts("\nTest QuickSort with string");
    qsort(v.elements, (size_t)v.max, sizeof(void *), fire_cmp_str);

    while (i < v.max) {
        printf("%s \n", (char *)v.elements[i]);
        i++;
    }

    puts("\nTest bsearch function");
    search = (char **)bsearch(&find, v.elements, (size_t)v.max, sizeof(void *), fire_cmp_str);
    if (search != NULL)
        printf("Found item in position %s\n", *search);
    else
        printf("Element = %s could not be found\n", find);

    fire_vector_free(&v);
    return 0;
}
