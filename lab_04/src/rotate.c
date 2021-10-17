#include "rotate.h"

void rotate(rotate_args_t *args)
{
    int cur_x, cur_y;
    double a = (double) args->angle / 180 * M_PI;

    for (int i = 0; i < args->figure->size; i++)
    {
        cur_x = args->figure->dots[i].x;
        cur_y = args->figure->dots[i].y;

        args->figure->dots[i].x = args->center.x + 
                      (cur_x - args->center.x) * cos(a) +
                      (cur_y - args->center.y) * sin(a);
    
        args->figure->dots[i].y = args->center.y - 
                      (cur_x - args->center.x) * sin(a) +
                      (cur_y - args->center.y) * cos(a);
    }
}

void *parallel_rotate(void *args)
{
    pthread_args_t *argsp = args;

    int i_start = argsp->t_id * (argsp->size / argsp->cnt_threads);
    int i_end = (argsp->t_id + 1) * (argsp->size / argsp->cnt_threads);

    int cur_x, cur_y;
    int angle = argsp->rotate_args->angle;
    double a = (double) angle / 180 * M_PI;

    for (int i = i_start; i < i_end; i++)
    {
        cur_x = argsp->rotate_args->figure->dots[i].x;
        cur_y = argsp->rotate_args->figure->dots[i].y;

        argsp->rotate_args->figure->dots[i].x = argsp->rotate_args->center.x + 
                      (cur_x - argsp->rotate_args->center.x) * cos(a) +
                      (cur_y - argsp->rotate_args->center.y) * sin(a);
    
        argsp->rotate_args->figure->dots[i].y = argsp->rotate_args->center.y - 
                      (cur_x - argsp->rotate_args->center.x) * sin(a) +
                      (cur_y - argsp->rotate_args->center.y) * cos(a);
    }

    return NULL;
}

int start_threading(rotate_args_t *args, const int cnt_threads)
{
    pthread_t *threads = malloc(cnt_threads * sizeof(pthread_t));

    if (!threads)
        return ALLOCATE_ERROR;

    pthread_args_t *p_args = malloc(sizeof(pthread_args_t) * cnt_threads);

    if (!p_args)
    {
        free(threads);
        return ALLOCATE_ERROR;
    }

    for (int i = 0; i < cnt_threads; i++)
    {
        p_args[i].rotate_args = args;
        p_args[i].t_id = i;
        p_args[i].size = args->figure->size;
        p_args[i].cnt_threads = cnt_threads;

        pthread_create(&threads[i], NULL, parallel_rotate, &p_args[i]);
    }

    for (int i = 0; i < cnt_threads; i++)
        pthread_join(threads[i], NULL);

    free(p_args);
    free(threads);

    return OK;
}