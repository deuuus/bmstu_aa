#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <limits.h>
#include "inttypes.h"

#include <math.h>

#define NMAX 2000

int min(int a, int b);
int max(int a, int b);
int min_from_three(int a, int b, int c);
int min_from_four(int a, int b, int c, int d);

char* substr(const char *str, int start, int len);

int **allocate_matrix(int m, int n);
void print(int **matrix, int m, int n);
void free_matrix(int **matrix, int size);
void fill_matrix_int_max(int **matrix, int n, int m);

uint64_t tick(void);
void lev_time();

#endif