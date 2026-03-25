#include "parser.h"
#include "AST.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static void skip_ws(parser_ctx *ctx)
{
    while (ctx->input[ctx->pos] != '\0' && isspace((unsigned char)ctx->input[ctx->pos])) {
        ctx->pos++;
    }
}

static void parse_error(parser_ctx *ctx, const char *msg)
{
    if (!ctx->error) {
        ctx->error = 1;
        ctx->message = msg;
    }
}

static int get_char(parser_ctx *ctx, char c)
{
    skip_ws(ctx);
    if (ctx->input[ctx->pos] != c) {
        parse_error(ctx, "unexpected character");
        return -1;
    }
    ctx->pos++;
    return 0;
}

static int get_int(parser_ctx *ctx, int *out)
{
    char *endptr = NULL;
    long v;

    if (out == NULL) {
        return -1;
    }

    skip_ws(ctx);
    if (!isdigit((unsigned char)ctx->input[ctx->pos])) {
        parse_error(ctx, "expected integer");
        return -1;
    }

    errno = 0;
    v = strtol(ctx->input + ctx->pos, &endptr, 10);
    if (errno != 0 || endptr == ctx->input + ctx->pos || v > INT_MAX || v < INT_MIN) {
        parse_error(ctx, "integer out of range");
        return -1;
    }

    *out = (int)v;
    ctx->pos = (size_t)(endptr - ctx->input);
    return 0;
}

static Expr *parse_expression(parser_ctx *ctx);

static Expr *alloc_expr(parser_ctx *ctx)
{
    Expr *expr = (Expr *)calloc(1, sizeof(Expr));
    if (expr == NULL) {
        parse_error(ctx, "out of memory");
    }
    return expr;
}

static Expr *parse_factor(parser_ctx *ctx)
{
    Expr *expr = NULL;
    int value = 0;

    skip_ws(ctx);
    if (ctx->error) return NULL;

    if (ctx->input[ctx->pos] == '(') {
        if (get_char(ctx, '(') != 0) return NULL;
        expr = parse_expression(ctx);
        if (expr == NULL) return NULL;
        if (get_char(ctx, ')') != 0) {
            free_expr(expr);
            return NULL;
        }
        return expr;
    }

    if (get_int(ctx, &value) != 0) return NULL;

    expr = alloc_expr(ctx);
    if (expr == NULL) return NULL;
    expr->op = 'x';
    expr->data.partinal = value;
    return expr;
}

static Expr *parse_term(parser_ctx *ctx)
{
    Expr *left = parse_factor(ctx);

    while (!ctx->error && left != NULL) {
        Expr *node = NULL;
        char op;

        skip_ws(ctx);
        op = ctx->input[ctx->pos];
        if (op != '*' && op != '/') break;

        ctx->pos++;
        node = alloc_expr(ctx);
        if (node == NULL) {
            free_expr(left);
            return NULL;
        }

        node->op = op;
        node->data.expr[0] = left;
        node->data.expr[1] = parse_factor(ctx);
        if (ctx->error || node->data.expr[1] == NULL) {
            free_expr(node);
            return NULL;
        }
        left = node;
    }

    return left;
}

static Expr *parse_expression(parser_ctx *ctx)
{
    Expr *left = parse_term(ctx);

    while (!ctx->error && left != NULL) {
        Expr *node = NULL;
        char op;

        skip_ws(ctx);
        op = ctx->input[ctx->pos];
        if (op != '+' && op != '-') break;

        ctx->pos++;
        node = alloc_expr(ctx);
        if (node == NULL) {
            free_expr(left);
            return NULL;
        }

        node->op = op;
        node->data.expr[0] = left;
        node->data.expr[1] = parse_term(ctx);
        if (ctx->error || node->data.expr[1] == NULL) {
            free_expr(node);
            return NULL;
        }
        left = node;
    }

    return left;
}

int parse_expression_string(const char *string, Expr **out_expr, parser_ctx *out_ctx)
{
    parser_ctx ctx;
    Expr *expr = NULL;

    if (string == NULL || out_expr == NULL) return -1;

    ctx.input = string;
    ctx.pos = 0;
    ctx.error = 0;
    ctx.message = NULL;

    expr = parse_expression(&ctx);
    skip_ws(&ctx);

    if (!ctx.error && ctx.input[ctx.pos] != '\0') {
        parse_error(&ctx, "trailing input");
    }

    if (ctx.error || expr == NULL) {
        free_expr(expr);
        *out_expr = NULL;
        if (out_ctx != NULL) *out_ctx = ctx;
        return -1;
    }

    *out_expr = expr;
    if (out_ctx != NULL) *out_ctx = ctx;
    return 0;
}
