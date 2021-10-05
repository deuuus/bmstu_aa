#include "utils.h"

int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

int min_from_three(int a, int b, int c)
{
    return min(a, min(b, c));
}

int min_from_four(int a, int b, int c, int d)
{
    return min(a, min_from_three(c, b, d));
}

char* substr(const char *str, int start, int len)
{
    char *s;

    s = malloc((len - start + 1) * sizeof(char));

    for (int i = start; i < len; i++)
        s[i] = str[i + start];

    return s;
}

int **allocate_matrix(int m, int n)
{
    int **data = calloc(m, sizeof(int*));
    if (!data)
        return NULL;
    for (int i = 0; i < m; i++)
    {
        data[i] = malloc(n * sizeof(int));
        if (!data[i])
        {
            free_matrix(data, m);
            return NULL;
        }
    }
    return data;
}

void fill_matrix_int_max(int **matrix, int n, int m)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matrix[i][j] = INT_MAX;
}

void free_matrix(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
        free(matrix[i]);
    free(matrix);
}

void print(int **matrix, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++) 
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

uint64_t tick(void)
{
    uint32_t high, low;
    __asm__ __volatile__(
        "rdtsc\n"
        "movl %%edx, %0\n"
        "movl %%eax, %1\n"
        : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");

    uint64_t ticks = ((uint64_t)high << 32) | low;

    return ticks;
}