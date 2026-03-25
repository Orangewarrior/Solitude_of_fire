#ifndef AST_H
#define AST_H

#include "global.h"

/**
 * @file AST.h
 * @brief AST execution and cleanup for arithmetic expressions.
 */

int Run_exp(Expr *expr, int *out);
void free_expr(Expr *expr);

#endif
