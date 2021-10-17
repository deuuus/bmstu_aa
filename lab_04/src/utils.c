#include "utils.h"

void _calc(int size, int threads, FILE *f);
void copy(rotate_args_t *a, rotate_args_t *b);

void analysis(void)
{
    FILE *f = fopen("res.txt", "w");
    _calc(1920000, 1, f);
    _calc(1920000, 2, f);
    _calc(1920000, 4, f);

    for (int i = 6; i <= 24; i+=6)
        _calc(1920000, i, f);

    _calc(1000000, 6, f);
    _calc(1200000, 6, f);
    _calc(1400000, 6, f);
    _calc(1600000, 6, f);
    _calc(1800000, 6, f);
    _calc(1920000, 6, f);
    fclose(f);
}

void _calc(int size, int threads, FILE *f)
{
    uint64_t time[2] = {0}, end, start;
    //clock_t start, end;

    point_t *dots = generate_figure(size);

    figure_t *figure = malloc(sizeof(figure_t));
    figure->dots = dots;
    figure->size = size;

    rotate_args_t *args = malloc(sizeof(args));
    args->figure = figure;
    args->angle = 45;
    point_t point = {100, 100};
    args->center = point;

    rotate_args_t *curr = malloc(sizeof(args));

    curr->figure = malloc(sizeof(figure_t));
    curr->figure->dots = malloc(sizeof(point_t) * size);

    for (int i = 0; i < REPEATS; i++)
    {
        copy(args, curr);
        start = tick();
        rotate(args);
        end = tick();
        time[0] += end - start;

        copy(args, curr);
        start = tick();
        start_threading(args, threads);
        end = tick();
        time[1] += end - start;
        //time[1] += (double) (end - start) / CLOCKS_PER_SEC;
    }

    time[0] /= REPEATS;
    time[1] /= REPEATS;

    fprintf(f, "%d & %ld & %ld\\ \n", threads, time[0], time[1]);
    printf("\nКоличество потоков:          %d\n", threads);
    printf("Размерность массива:         %d\n", size);
    printf("Последовательная реализация: %ld\n", time[0]);// * 100000);
    printf("Распараллеленная реализация: %ld\n", time[1]);// * 100000);
}

void copy(rotate_args_t *a, rotate_args_t *b)
{
    for (int i = 0; i < a->figure->size; i++)
    {
        b->figure->dots[i].x = a->figure->dots[i].x;
        b->figure->dots[i].y = a->figure->dots[i].y;
    }
    b->angle = a->angle;
    b->center = a->center;
}

point_t *generate_figure(int size)
{
    point_t *figure = malloc(sizeof(point_t) * size);
    if (!figure)
        return NULL;

    srand(42);
    for (int i = 0; i < size; i++)
    {
        figure[i].x = rand() % 1600;
        figure[i].y = rand() % 1200;
    }
    return figure;
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

void print_data(figure_t *figure)
{
    FILE *f_out = fopen("result.txt", "w");
    fprintf(f_out, "%d\n", figure->size);
    for (int i = 0; i < figure->size; i++)
        fprintf(f_out, "%d %d\n", figure->dots[i].x, figure->dots[i].y);
    fclose(f_out);
}

FILE *get_file(void)
{
    char str[100];
    if (!fgets(str, sizeof(str), stdin))
        return NULL;

    str[strlen(str) - 1] = '\0';

    FILE *f = fopen(str, "r");
    return f;
}

rotate_args_t *read_figure(FILE *f)
{
    int size;
    if (fscanf(f, "%d", &size) != 1 || size < 1)
    {
        printf("\n\nПроизошла ошибка во время чтения количества точек фигуры.\n");
        return NULL;
    }

    rotate_args_t *rotate_args = (rotate_args_t*) malloc(sizeof(rotate_args_t));
    if (!rotate_args)
        return NULL;

    rotate_args->figure = (figure_t *) malloc(sizeof(figure_t));
    if (!rotate_args->figure)
        return NULL;

    rotate_args->figure->size = size;

    rotate_args->figure->dots = (point_t *) malloc(sizeof(point_t) * size);
    if (!rotate_args->figure->dots)
    {
        free(rotate_args->figure);
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        if (fscanf(f, "%d %d", &rotate_args->figure->dots[i].x, &rotate_args->figure->dots[i].y) != 2)
        {
            printf("\n\nПроизошла ошибка во время считывания координат точек фигуры."
                    "\nКоординаты должны выражаться целым числом.\n");
            free(rotate_args->figure->dots);
            free(rotate_args->figure);
            free(rotate_args);
            return NULL;
        }  
        if (rotate_args->figure->dots[i].x < 0 || rotate_args->figure->dots[i].x > 1600 
         || rotate_args->figure->dots[i].y < 0 || rotate_args->figure->dots[i].y > 1200)
        {
            printf("\n\nПроизошла ошибка во время считывания координат точек фигуры."
                    "\nКоординаты должны не выходить за пределы экранной плоскости размером 1600х1200 пикселей.\n");
            free(rotate_args->figure->dots);
            free(rotate_args->figure);
            free(rotate_args);
            return NULL;
        }
    }

    if (fscanf(f, "%d", &rotate_args->angle) != 1)
    {
        printf("\n\nПроизошла ошибка во время чтения угла поворота.\n");
        return NULL;
    }

    if (fscanf(f, "%d %d", &rotate_args->center.x, &rotate_args->center.y) != 2)
        {
            printf("\n\nПроизошла ошибка во время считывания координат точек фигуры."
                    "\nКоординаты должны выражаться целым числом.\n");
            free(rotate_args->figure->dots);
            free(rotate_args->figure);
            free(rotate_args);
            return NULL;
        }  
        if (rotate_args->center.x < 0 || rotate_args->center.x > 1600 
         || rotate_args->center.y < 0 || rotate_args->center.y > 1200)
        {
            printf("\n\nПроизошла ошибка во время считывания координат точки центра поворота."
                    "\nКоординаты должны не выходить за пределы экранной плоскости размером 1600х1200 пикселей.\n");
            free(rotate_args->figure->dots);
            free(rotate_args->figure);
            free(rotate_args);
            return NULL;
        }

    return rotate_args;
}

int choose_action(int *action)
{
    printf("Выберите действие:\n\n");
    printf("1) Произвести операцию поворота над фигурой.\n");
    printf("2) Показать сравнительный анализ эффективности\n   различных реализаций алгоритма поворота фигуры.\n");
    printf("3) Выход.\n\n");
    printf("Ответ: ");
    if (scanf("%d", action) != 1 || ((*action < 1) || (*action > 3)) || getchar() != '\n')
    {
        printf("\nОшибка: команда должна быть одним из чисел, предложенных в меню.\n");
        return -1;
    }
    return EXIT_SUCCESS;
}

int choose_sub_action(int *action)
{
    printf("\nВыберите способ реализации:\n\n");
    printf("4) Использовать последовательную реализацию алгоритма.\n");
    printf("5) Использовать параллельную реализацию алгоритма.\n");
    printf("6) Отмена.\n\n");
    printf("Ответ: ");
    if (scanf("%d", action) != 1 || ((*action < 4) || (*action > 6)) || getchar() != '\n')
    {
        printf("\nОшибка: команда должна быть одним из чисел, предложенных в меню.\n");
        return -1;
    }
    return EXIT_SUCCESS;
}