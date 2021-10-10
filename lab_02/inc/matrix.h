#ifndef MATRIX_H
#define MATRIX_H

#define NMAX 1002

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct
{
    int **ptr;
    int dim1;
    int dim2;
} matrix_t;

typedef struct
{
    int *ptr;
    int size;
} vector_t;

int read_dimensions(int *a, int *b);
void free_matrix(int **matrix, int size);
int **allocate_matrix(int m, int n);
int **reallocate_matrix(int **data, int m, int n);
int read(int **matrix, int m, int n);
void fill(matrix_t *M, int **ptr, int a, int b);
void print(const matrix_t *M);
void _print(int **matrix, int m, int n);

uint64_t tick(void);
void generate_matrix(int **a, int m, int n);

#endif