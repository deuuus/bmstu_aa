#ifndef UTILS_H
#define UTILS_H

#define SORT     1
#define ANALYSIS 2
#define EXIT     3

#define NMAX 1000

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

void swap_int(int *a, int *b);
uint64_t tick(void);

int choose_action(int *action);
int read_dimension(int *n);
int read_array(int a[], int n);
void print_array(int a[], int n);
void copy_array(int a[], int copy[], int n);
void generate_array(int a[], int n);
void reverse_array(int a[], int n);

#endif