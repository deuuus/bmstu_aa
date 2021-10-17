#ifndef ROTATE_H
#define ROTATE_H

#define M_PI 3.14159265358979323846
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define OK 0
#define ALLOCATE_ERROR 1

#define N 2000

typedef struct 
{
    int x;
    int y;
} point_t;

typedef struct
{
    point_t *dots;
    int size;
} figure_t;

typedef struct {
    figure_t *figure;
    int angle;
    point_t center;
} rotate_args_t;

typedef struct {
    rotate_args_t *rotate_args;
    int t_id;
    int size;
    int cnt_threads;
} pthread_args_t;

void rotate(rotate_args_t *args);
void *parallel_rotate(void *args);
int start_threading(rotate_args_t *args, const int cnt_threads);

#endif