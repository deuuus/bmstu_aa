#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

#include "utils.h"

int lev_rec(const char *s1, const char *s2);
int lev_rec_matrix(const char *s1, const char *s2);
int lev_iter(const char *s1, const char *s2);

int dam_lev_rec(const char *s1, const char *s2);
int dam_lev_rec_matrix(const char *s1, const char *s2);
int dam_lev_iter(const char *s1, const char *s2);

int get_lev_dist(const char *s1, const char *s2, int **matrix);
int get_dam_lev_dist(const char *s1, const char *s2, int **matrix);

int output_lev_iter(const char *s1, const char *s2);
int output_dam_lev_iter(const char *s1, const char *s2);

#endif