#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "matrix.h"

#define REPEATS 20

matrix_t *math_mult(const matrix_t *A, const matrix_t *B);

matrix_t *coppersmith_winograd(const matrix_t *A, const matrix_t *B);
vector_t *make_mh(const matrix_t *A);
vector_t *make_mv(const matrix_t *B);

matrix_t *upd_coppersmith_winograd(const matrix_t *A, const matrix_t *B);
vector_t *upd_make_mh(const matrix_t *A);
vector_t *upd_make_mv(const matrix_t *B);

void _analysis(void);
void _calc(int size, FILE *f);

#endif