#include "utils.h"

void generate_array(int a[], int n)
{
    srand(42);
    for (int i = 0; i < n; i++)
        a[i] = rand();
}

void reverse_array(int a[], int n)
{
    int tmp;
    for(int i = 0; i < n / 2; i++) 
    {
        tmp = a[i];
        a[i] = a[n - i - 1]; 
        a[n - i - 1] = tmp;
    }
}

int choose_action(int *action)
{
    printf("Выберите действие:\n\n");
    printf("1) Ввести массив и отсортировать его тремя разными сортировками;\n");
    printf("2) Показать сравнительный анализ эффективности алгоритмов сортировки;\n");
    printf("3) Выход.\n\n");
    printf("Ответ: ");
    if (scanf("%d", action) != 1 || ((*action < 1) || (*action > 3)) || getchar() != '\n')
    {
        printf("\nОшибка: команда должна быть одним из чисел, предложенных в меню.\n");
        return -1;
    }
    return EXIT_SUCCESS;
}

int read_dimension(int *n)
{
    printf("\nВведите размерность массива: ");
    int temp_n;
    int rc = scanf("%d", &temp_n) == 1 && temp_n >= 0 && temp_n < NMAX;
    if (!rc)
    {
        printf("\nОшибка ввода: размерность массива должна являться "
               "целым положительным числом, не больше чем %d.\n", NMAX);
        return EXIT_FAILURE;
    }
    *n = temp_n;
    return EXIT_SUCCESS;
}

int read_array(int a[], int n)
{
    printf("\nВведите элементы массива:\n");
    for (int i = 0; i < n; i++)
    {
        printf("\nA[%d] = ", i);
        if (scanf("%d", &a[i]) != 1)
        {
            printf("\nОшибка ввода. Элемент массива должен являться целым числом.\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

void print_array(int a[], int n)
{
    printf("\nA = [");
    for (int i = 0; i < n; i++)
    {
        printf("%d", a[i]);
        if (i != n - 1)
        {
            printf(", ");
        }
    }
    printf("]\n\n");
}

void copy_array(int a[], int copy[], int n)
{
    for (int i = 0; i < n; i++)
        copy[i] = a[i];
}

void swap_int(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
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