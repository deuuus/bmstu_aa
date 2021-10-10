#include "operations.h"

int choose_action(int *action)
{
    printf("Выберите действие:\n\n");
    printf("1) Ввести две матрицы и перемножить их тремя различными методами.\n");
    printf("2) Показать сравнительный анализ эффективности алгоритмов умножения матриц.\n");
    printf("3) Выход.\n\n");
    printf("Ответ: ");
    if (scanf("%d", action) != 1 || (*action < 1) || (*action > 3) || getchar() != '\n')
    {
        printf("\nОшибка: команда должна быть одним из чисел, предложенных в меню.\n");
        return -1;
    }
    return EXIT_SUCCESS;
}

int main()
{
    int action = 0, rc;
    int m1, n1, m2, n2;
    matrix_t *C;
    while (action != 3)
    {
        if ((rc = choose_action(&action)))
            return rc;

        if (action == 1)
        {
            printf("\nВведите размерность матрицы А:\n\n");
            if ((rc = read_dimensions(&m1, &n1)))
                return rc;
            int **ptr_A = allocate_matrix(m1, n1);
            if (!ptr_A)
                return EXIT_FAILURE;
            printf("\nВведите элементы матрицы А:\n\n");
            if ((rc = read(ptr_A, m1, n1)))
            {
                free_matrix(ptr_A, m1);
                return rc;
            }
            matrix_t *A = malloc(sizeof(matrix_t));
            if (!A)
            {
                free(A);
                return EXIT_FAILURE;
            }
            fill(A, ptr_A, m1, n1);

            printf("\nВведите размерность матрицы B:\n\n");
            if ((rc = read_dimensions(&m2, &n2)))
            {
                free(A);
                return rc;
            }
            int **ptr_B = allocate_matrix(m2, n2);
            if (!ptr_B)
            {
                free(A);
                return EXIT_FAILURE;
            }
            printf("\nВведите элементы матрицы B:\n\n");
            if ((rc = read(ptr_B, m2, n2)))
            {
                free(A);
                free_matrix(ptr_B, m2);
                return rc;
            }
            matrix_t *B = malloc(sizeof(matrix_t));
            if (!B)
            {
                free(A);
                free_matrix(ptr_B, m2);
                return EXIT_FAILURE;
            }
            fill(B, ptr_B, m2, n2);

            if (n1 != m2)
            {
                printf("\nНевозможно произвести умножение матриц.");
                continue;
            }

            C = math_mult(A, B);
            printf("\nРезультирующая матрица С\n\n");
            printf("Обычное умножение:\n");
            if (C)
            {
                print(C);
                free(C);
            }

            C = coppersmith_winograd(A, B);
            printf("Умножение по Копперсмиту-Винограду:\n");
            if (C)
            {
                print(C);
                free(C);
            }

            C = upd_coppersmith_winograd(A, B);
            printf("Умножение по Копперсмиту-Винограду(оптимизированная версия):\n");
            if (C)
            {
                print(C);
                free(C);
            }

            free(A);
            free(B);
        }
        
        if (action == 2)
        {
            _analysis();
        }
    }
    return EXIT_SUCCESS;
}