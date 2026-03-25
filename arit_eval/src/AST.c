#include "AST.h"
#include "secure_operation.h"
#include <stdlib.h>

void free_expr(Expr *expr)
{
    if (expr == NULL) return;
    if (expr->op != 'x') {
        free_expr(expr->data.expr[0]);
        free_expr(expr->data.expr[1]);
    }
    free(expr);
}

int Run_exp(Expr *expr, int *out)
{
    int left = 0, right = 0;
    if (expr == NULL || out == NULL) return -1;
    switch (expr->op) {
        case '*': if (Run_exp(expr->data.expr[0], &left) != 0 || Run_exp(expr->data.expr[1], &right) != 0) return -1; return safe_mul(left, right, out);
        case '/': if (Run_exp(expr->data.expr[0], &left) != 0 || Run_exp(expr->data.expr[1], &right) != 0) return -1; return safe_div(left, right, out);
        case '+': if (Run_exp(expr->data.expr[0], &left) != 0 || Run_exp(expr->data.expr[1], &right) != 0) return -1; return safe_add(left, right, out);
        case '-': if (Run_exp(expr->data.expr[0], &left) != 0 || Run_exp(expr->data.expr[1], &right) != 0) return -1; return safe_sub(left, right, out);
        case 'x': *out = expr->data.partinal; return 0;
        default: return -1;
    }
}
