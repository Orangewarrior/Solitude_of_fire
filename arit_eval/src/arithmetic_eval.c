#define _POSIX_C_SOURCE 200809L

#include "arithmetic_eval.h"
#include "AST.h"
#include "parser.h"
#include <string.h>

static size_t fire_bounded_strlen(const char *s, size_t maxlen)
{
    size_t i = 0;
    if (s == NULL) return 0;
    while (i < maxlen && s[i] != '\0') {
        i++;
    }
    return i;
}

int arit_eval_ex(const char *in, int *out)
{
    Expr *expr = NULL;
    int result = 0;
    if (in == NULL || out == NULL) return -1;
    if (fire_bounded_strlen(in, 1025) > 1024) return -1;
    if (parse_expression_string(in, &expr, NULL) != 0 || expr == NULL) return -1;
    if (Run_exp(expr, &result) != 0) { free_expr(expr); return -1; }
    free_expr(expr);
    *out = result;
    return 0;
}

int arit_eval(char *in)
{
    int out = 0;
    if (arit_eval_ex(in, &out) != 0) return 0;
    return out;
}
