#include "sort.h"

#define NMAX 1000

void bubble_sort(int a[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                swap_int(&a[j], &a[j + 1]);
            }
        }
    }
}

void gnome_sort(int a[], int n)
{
    int i = 1;
    while (i < n)
    {
        if (a[i - 1] <= a[i])
        {
            i += 1;
        }
        else
        {
            swap_int(&a[i - 1], &a[i]);
            if (i > 1)
            {
                i--;
            }
        }
        
    }
}

void select_sort(int a[], int n)
{
    int min_i;
    for (int i = 0; i < n; i++)
    {
        min_i = i;
        for (int j = i; j < n; j++)
        {
            if (a[j] < a[min_i])
            {
                min_i = j;
            }
        }
        swap_int(&a[i], &a[min_i]);
    }
}

void _merge_sort(int a[], int left, int right);
void _merge(int a[], int left, int right);

void merge_sort(int a[], int n)
{
    _merge_sort(a, 0, n);
}

void _merge_sort(int a[], int left, int right)
{
    if (left == right || (left + 1) == right)
    {
        return;
    }

    int middle = left + (right - left) / 2;

    _merge_sort(a, left, middle);
    _merge_sort(a, middle, right);
    _merge(a, left, right);
}

void _merge(int a[], int left, int right)
{
    int temp[right * 2];
    int middle = left + (right - left) / 2;
    int i_left = left, i_right = middle, i = 0;

    while (i_left < middle || i_right < right)
    {
        if (i_left >= middle)
        {
            temp[i++] = a[i_right++];
        }
        else if (i_right >= right)
        {
            temp[i++] = a[i_left++];
        }
        else if (a[i_left] < a[i_right])
        {
            temp[i++] = a[i_left++];
        }
        else
        {
            temp[i++] = a[i_right++];
        }   
    }

    for (int j = 0; j < i; j++)
    {
        a[left + j] = temp[j]; 
    }
}

void _analysis(int a[], int copy[], int n);

void sorted_analysis()
{
    printf("\n============Анализ сортировок на остортированных данных============\n");
    int a[NMAX], copy[NMAX];
    generate_array(a, NMAX);
    merge_sort(a, NMAX);
    _analysis(a, copy, 10);
    _analysis(a, copy, 50);
    _analysis(a, copy, 100);
    _analysis(a, copy, 250);
    _analysis(a, copy, 500);
    _analysis(a, copy, 750);
    _analysis(a, copy, 1000);
    printf("\n");
}

void antisorted_analysis()
{
    printf("\n============Анализ сортировок на остортированных в обратном порядке данных============\n");
    int a[NMAX], copy[NMAX];
    generate_array(a, NMAX);
    reverse_array(a, NMAX);
    merge_sort(a, NMAX);
    _analysis(a, copy, 10);
    _analysis(a, copy, 50);
    _analysis(a, copy, 100);
    _analysis(a, copy, 250);
    _analysis(a, copy, 500);
    _analysis(a, copy, 750);
    _analysis(a, copy, 1000);
    printf("\n");
}

void random_analysis(void)
{
    printf("\n============Анализ сортировок на наборе случайных чисел============\n");
    int a[NMAX], copy[NMAX];
    generate_array(a, NMAX);
    _analysis(a, copy, 10);
    _analysis(a, copy, 50);
    _analysis(a, copy, 100);
    _analysis(a, copy, 250);
    _analysis(a, copy, 500);
    _analysis(a, copy, 750);
    _analysis(a, copy, 1000);
    printf("\n");
}

void _analysis(int a[], int copy[], int n)
{
    uint64_t time[3] = {0}, start, end;
    int REPEATS = 500;

    for (int i = 0; i < REPEATS; i++)
    {
        copy_array(a, copy, n);
        start = tick();
        bubble_sort(copy, n);
        end = tick();
        time[0] += end - start;

        copy_array(a, copy, n);
        start = tick();
        select_sort(copy, n);
        end = tick();
        time[1] += end - start;

        copy_array(a, copy, n);
        start = tick();
        gnome_sort(copy, n);
        end = tick();
        time[2] += end - start;
    }
    for (int i = 0; i < 3; i++)
        time[i] /= REPEATS;

    printf("\nСортировка пузырьком: %d элементов - %ld тиков.", n, time[0]);
    printf("\nСортировка выбором: %d элементов - %ld тиков.", n, time[1]);
    printf("\nГномья сортировка: %d элементов - %ld тиков.\n", n, time[2]);
}