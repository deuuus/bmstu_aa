#include "matrix.h"

int read_dimensions(int *a, int *b)
{
    printf("Введите количество строк матрицы: ");
    if (scanf("%d", a) != 1 || *a <= 0)
    {
        printf("Ошибка ввода количества строк матрицы: должно быть введено целое положительное число.");
        return EXIT_FAILURE;
    }
    printf("Введите количество строк матрицы: ");
    if (scanf("%d", b) != 1 || *b <= 0)
    {
        printf("Ошибка ввода количества столбцов матрицы: должно быть введено целое положительное число.");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
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

int read(int **matrix, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        printf("Введите %d-ую строку матрицы: ", i + 1);
        for (int j = 0; j < n; j++)
            if (scanf("%d", &matrix[i][j]) != 1)
            {
                printf("Ошибка ввода элемента матрицы: должно быть введено целое число.");
                return EXIT_FAILURE;
            }
    }
    return EXIT_SUCCESS;
}

void fill(matrix_t *M, int **ptr, int a, int b)
{
    M->ptr = ptr;
    M->dim1 = a;
    M->dim2 = b;
}

void print(const matrix_t *M)
{
    _print(M->ptr, M->dim1, M->dim2);
}

void _print(int **matrix, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++) 
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

void free_matrix(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
        free(matrix[i]);
    free(matrix);
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

void generate_matrix(int **a, int m, int n)
{
    srand(42);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            a[i][j] = rand() % 5;
        }
}