#ifndef SECURE_OPERATION_H
#define SECURE_OPERATION_H

/**
 * @file secure_operation.h
 * @brief Checked integer arithmetic helpers.
 */

int safe_add(int a, int b, int *out);
int safe_sub(int a, int b, int *out);
int safe_mul(int a, int b, int *out);
int safe_div(int a, int b, int *out);

#endif
