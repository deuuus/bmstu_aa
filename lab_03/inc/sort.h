#ifndef SORT_H
#define SORT_H

#include "utils.h"

void bubble_sort(int a[], int n);
void gnome_sort(int a[], int n);
void merge_sort(int a[], int n);
void select_sort(int a[], int n);

void random_analysis(void);
void sorted_analysis();
void antisorted_analysis();

#endif
