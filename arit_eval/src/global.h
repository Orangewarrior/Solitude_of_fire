#ifndef GLOBAL_H
#define GLOBAL_H

typedef union {
  int partinal;
  struct expression* expr[2];
} Data;

typedef struct expression {
  char op;
  Data data;
} Expr;

#endif
