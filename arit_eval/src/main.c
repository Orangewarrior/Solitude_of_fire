/**
 * @file main.c
 * @brief Full example program for the arit_eval module.
 */

#include <stdio.h>
#include "arithmetic_eval.h"

int main(int argc, char **argv)
{
    int value = 0;

    if (argc > 1) {
        if (arit_eval_ex(argv[1], &value) == 0) {
            printf("%d\n", value);
            return 0;
        }
        fprintf(stderr, "invalid arithmetic expression\n");
        return 1;
    }

    return 0;
}
