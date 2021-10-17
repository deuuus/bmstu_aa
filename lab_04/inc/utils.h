#ifndef UTILS_H
#define UTILS_H

#include "rotate.h"
#include <time.h>
#include <string.h>
#include <inttypes.h>

#define REPEATS 2

int choose_action(int *action);
int choose_sub_action(int *action);
FILE *get_file(void);
rotate_args_t *read_figure(FILE *f);
void print_data(figure_t *figure);

void analysis(void);
point_t *generate_figure(int size);
uint64_t tick(void);

#endif