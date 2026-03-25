#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include "global.h"

/**
 * @file parser.h
 * @brief Reentrant parser context and parsing entry point.
 */

typedef struct parser_ctx {
    const char *input;
    size_t pos;
    int error;
    const char *message;
} parser_ctx;

int parse_expression_string(const char *string, Expr **out_expr, parser_ctx *out_ctx);

#endif
