#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#define _GNU_SOURCE

#include "levenshtein.h"

int choose_action(int *action)
{
    printf("Выберите действие:\n\n");
    printf("1) Ввести две строки и посчитать расстояние для них;\n");
    printf("2) Показать сравнительный анализ эффективности алгоритмов;\n");
    printf("3) Выход.\n\n");
    printf("Ответ: ");
    if (scanf("%d", action) != 1 || ((*action < 1) || (*action > 3)) || getchar() != '\n')
    {
        printf("\nОшибка: команда должна быть одним из чисел, предложенных в меню.\n");
        return -1;
    }
    return EXIT_SUCCESS;
}

int main(void)
{
    int action = 0, rc;
    char s1[NMAX], s2[NMAX];
    char ch;
    int i;

    while (action != 3)
    {
        if ((rc = choose_action(&action)))
            return rc;

        if (action == 1)
        {
            printf("\nПервая строка: ");
            i = 0;
            while ((ch = getchar()) != '\n' && i < (NMAX - 1))
                s1[i++] = ch;
            s1[i] = '\0';

            printf("\nВторая строка: ");
            i = 0;
            while ((ch = getchar()) != '\n' && i < (NMAX - 1))
                s2[i++] = ch;
            s2[i] = '\0';

            printf("\nРасстояние Левенштейна: рекурсивный алгоритм: %d\n\n", lev_rec(s1, s2));
            printf("Расстояние Левенштейна: рекурсивный алгоритм с кешированием: %d\n\n", lev_rec_matrix(s1, s2));
            printf("Расстояние Левенштейна: матричный(итеративный) алгоритм: %d\n\n", lev_iter(s1, s2));
            printf("Расстояние Дамерау-Левенштейна: %d\n\n", dam_lev_iter(s1, s2));

            printf("Матрица по Левенштейну:\n\n");
            output_lev_iter(s1, s2);

            printf("Матрица по Дамерау-Левенштейну:\n\n");
            output_dam_lev_iter(s1, s2);
        }

        if (action == 2)
        {
            lev_time();
        }
    }

    return EXIT_SUCCESS;
}