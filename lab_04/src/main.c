#include "rotate.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

int choose_action(int *action);
int choose_sub_action(int *action);
FILE *get_file(void);
rotate_args_t *read_figure(FILE *f);
void print_data(figure_t *figure);

int main()
{
    rotate_args_t *rotate_args;
    int threads;
    int action, rc;
    FILE *f;
    while (action != 3)
    {
        if ((rc = choose_action(&action)))
            return rc;
        
        if (action == 1)
        {
            if ((rc = choose_sub_action(&action)))
                return rc;

            printf("\nВведите название файла с данными о фигуре: ");
            f = get_file();
            if (!f)
            {
                printf("\nОшибка: введено некорректное имя файла.\n"
                    "\nУбедитесь, что файл расположен в директории aa/bmstu_aa_lab_04 "
                    "и указано верное название, включая расширение.\n");
                return EXIT_FAILURE;
            }

            printf("\nФайл успешно открыт. Считывание данных... ");
            rotate_args = read_figure(f);
            if (!rotate_args)
                return EXIT_FAILURE;
            printf("Данные успешно считаны.\n\n");
            fclose(f);

            if (action == 4)
                rotate(rotate_args);
            else
            {
                printf("Введите количество потоков: ");
                if (scanf("%d", &threads) != 1 || threads < 1)
                {
                    printf("\nПроизошла ошибка чтения количества потоков. Введите корректное целое положительное число. \n");
                    free(rotate_args->figure->dots);
                    free(rotate_args->figure);
                    free(rotate_args);
                    return EXIT_FAILURE;
                }

                start_threading(rotate_args, threads);
            }

            print_data(rotate_args->figure);

            free(rotate_args->figure->dots);
            free(rotate_args->figure);
            free(rotate_args);

            printf("\nПоворот осуществлен. Информация записана в файл result.txt.\n\n");
        }

        else if (action == 2)
        {
            analysis();
        }
    }
    return EXIT_SUCCESS;
}