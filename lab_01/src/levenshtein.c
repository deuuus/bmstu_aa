#include "levenshtein.h"

//Функция нахождения расстояния Левенштейна с помощью рекурсии
int lev_rec(const char *s1, const char *s2)
{
    int i = strlen(s1), j = strlen(s2);

    if (i == 0 || j == 0)
        return max(i, j);

    int match = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

    return min_from_three(lev_rec(s1, substr(s2, 0, j - 1)) + 1,
                          lev_rec(substr(s1, 0, i - 1), s2) + 1, 
                          lev_rec(substr(s1, 0, i - 1), substr(s2, 0, j - 1))
                          + match);
}

//Функция нахождения расстояния Дамерау-Левенштейна с помощью рекурсии
int dam_lev_rec(const char *s1, const char *s2)
{
    int i = strlen(s1), j = strlen(s2);

    if (i == 0 || j == 0)
        return max(i, j);

    int match = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

    int res = min_from_three(dam_lev_rec(s1, substr(s2, 0, j - 1)) + 1,
                             dam_lev_rec(substr(s1, 0, i - 1), s2) + 1, 
                             dam_lev_rec(substr(s1, 0, i - 1), substr(s2, 0, j - 1))
                             + match);

    if (i > 1 && j > 1 && s1[i] == s2[j - 1] && s1[i - 1] == s2[j])
    {
        res = min(dam_lev_rec(substr(s1, 0, i - 2), substr(s2, 0, j - 2)) + 1, res);
    }

    return res;
}

//Функция нахождения расстояния Левенштейна с кешированием
int lev_rec_matrix(const char *s1, const char *s2)
{
    int i = strlen(s1), j = strlen(s2);

    int **matrix = allocate_matrix(i + 1, j + 1);
    fill_matrix_int_max(matrix, i + 1, j + 1);

    int dist = get_lev_dist(s1, s2, matrix);
    free_matrix(matrix, i + 1);

    return dist;
}

//Функция нахождения расстояния Дамерау-Левенштейна с кешированием
int dam_lev_rec_matrix(const char *s1, const char *s2)
{
    int i = strlen(s1), j = strlen(s2);

    int **matrix = allocate_matrix(i  + 1, j + 1);
    fill_matrix_int_max(matrix, i + 1, j + 1);

    int dist = get_dam_lev_dist(s1, s2, matrix);
    free_matrix(matrix, i + 1);

    return dist;
}

//Функция нахождения расстояния Левенштейна итерационно
int lev_iter(const char *s1, const char *s2)
{
    int n = strlen(s1), m = strlen(s2);

    int **matrix = allocate_matrix(n + 1, m + 1);
    fill_matrix_int_max(matrix, n + 1, m + 1);

    for (int i = 0; i < n + 1; i++)
        matrix[i][0] = i;

    for (int j = 0; j < m + 1; j++)
        matrix[0][j] = j;

    int insert_dist, delete_dist, match_dist, match;

    for (int i = 1; i < n + 1; i++)
        for (int j = 1; j < m + 1; j++)
        {
            match = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

            insert_dist = matrix[i][j - 1] + 1;
            delete_dist = matrix[i - 1][j] + 1;
            match_dist = matrix[i - 1][j - 1] + match;

            matrix[i][j] = min_from_three(insert_dist, delete_dist, match_dist);
        }

    int dist = matrix[n][m];

    free_matrix(matrix, n + 1);

    return dist;
}

//Функция нахождения расстояния Дамерау-Левенштейна итерационно
int dam_lev_iter(const char *s1, const char *s2)
{
    int n = strlen(s1), m = strlen(s2);

    int **matrix = allocate_matrix(n  + 1, m + 1);
    fill_matrix_int_max(matrix, n + 1, m + 1);

    for (int i = 0; i < n + 1; i++)
        matrix[i][0] = i;

    for (int j = 0; j < m + 1; j++)
        matrix[0][j] = j;

    int insert_dist, delete_dist, match_dist, exchange_dist;
    int match;

    for (int i = 1; i < n + 1; i++)
        for (int j = 1; j < m + 1; j++)
        {
            match = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            
            exchange_dist = INT_MAX;
            if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1])
                exchange_dist = matrix[i - 2][j - 2] + 1;
            
            insert_dist = matrix[i][j - 1] + 1;
            delete_dist = matrix[i - 1][j] + 1;
            match_dist = matrix[i - 1][j - 1] + match;

            matrix[i][j] = min_from_four(insert_dist, delete_dist,
                                         match_dist, exchange_dist);
        }

    int dist = matrix[n][m];
    free_matrix(matrix, n + 1);

    return dist;
}

//Вспомогательная функция нахождения расстояния Левенштейна с кешированием
int get_lev_dist(const char *s1, const char *s2, int **matrix)
{
    int i = strlen(s1), j = strlen(s2);

    if (matrix[i][j] != INT_MAX)
        return matrix[i][j];

    if (i == 0)
    {
        matrix[i][j] = j;
        return matrix[i][j];
    }

    if (i > 0 && j == 0)
    {
        matrix[i][j] = i;
        return matrix[i][j];
    }

    int match = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

    matrix[i][j] = min_from_three(
                get_lev_dist(s1, substr(s2, 0, j - 1), matrix) + 1,
                get_lev_dist(substr(s1, 0, i - 1), s2, matrix) + 1, 
                get_lev_dist(substr(s1, 0, i - 1), substr(s2, 0, j - 1), matrix)
                + match);
    return matrix[i][j];
}

//Вспомогательная функция нахождения расстояния Дамерау-Левенштейна с кешированием
int get_dam_lev_dist(const char *s1, const char *s2, int **matrix)
{
    int i = strlen(s1), j = strlen(s2);

    if (matrix[i][j] != INT_MAX)
        return matrix[i][j];

    if (i == 0)
    {
        matrix[i][j] = j;
        return matrix[i][j];
    }

    if (i > 0 && j == 0)
    {
        matrix[i][j] = i;
        return matrix[i][j];
    }

    int match = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

    int res = min_from_three(
                get_dam_lev_dist(s1, substr(s2, 0, j - 1), matrix) + 1,
                get_dam_lev_dist(substr(s1, 0, i - 1), s2, matrix) + 1, 
                get_dam_lev_dist(substr(s1, 0, i - 1), substr(s2, 0, j - 1), matrix)
                + match);

    if (i > 1 && j > 1 && s1[i] == s2[j - 1] && s1[i - 1] == s2[j])
        res = min(get_dam_lev_dist(substr(s1, 0, i - 2),
                                   substr(s2, 0, j - 2), matrix), res);

    matrix[i][j] = res;

    return matrix[i][j];
}

//Функция нахождения расстояния Левенштейна итерационно с печатью
int output_lev_iter(const char *s1, const char *s2)
{
    int n = strlen(s1), m = strlen(s2);

    int **matrix = allocate_matrix(n + 1, m + 1);
    fill_matrix_int_max(matrix, n + 1, m + 1);

    for (int i = 0; i < n + 1; i++)
        matrix[i][0] = i;

    for (int j = 0; j < m + 1; j++)
        matrix[0][j] = j;

    int insert_dist, delete_dist, match_dist, match;

    for (int i = 1; i < n + 1; i++)
        for (int j = 1; j < m + 1; j++)
        {
            match = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

            insert_dist = matrix[i][j - 1] + 1;
            delete_dist = matrix[i - 1][j] + 1;
            match_dist = matrix[i - 1][j - 1] + match;

            matrix[i][j] = min_from_three(insert_dist, delete_dist, match_dist);
        }

    int dist = matrix[n][m];

    if (n != 0 && m != 0)
        print(matrix, n + 1, m + 1);

    free_matrix(matrix, n + 1);

    return dist;
}

//Функция нахождения расстояния Дамерау-Левенштейна итерационно с печатью
int output_dam_lev_iter(const char *s1, const char *s2)
{
    int n = strlen(s1), m = strlen(s2);

    int **matrix = allocate_matrix(n  + 1, m + 1);
    fill_matrix_int_max(matrix, n + 1, m + 1);

    for (int i = 0; i < n + 1; i++)
        matrix[i][0] = i;

    for (int j = 0; j < m + 1; j++)
        matrix[0][j] = j;

    int insert_dist, delete_dist, match_dist, exchange_dist;
    int match;

    for (int i = 1; i < n + 1; i++)
        for (int j = 1; j < m + 1; j++)
        {
            match = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            
            exchange_dist = INT_MAX;
            if (i > 1 && j > 1 && s1[i - 1] == s2[j - 2] && s1[i - 2] == s2[j - 1])
                exchange_dist = matrix[i - 2][j - 2] + 1;
            
            insert_dist = matrix[i][j - 1] + 1;
            delete_dist = matrix[i - 1][j] + 1;
            match_dist = matrix[i - 1][j - 1] + match;

            matrix[i][j] = min_from_four(insert_dist, delete_dist,
                                         match_dist, exchange_dist);
        }

    int dist = matrix[n][m];

    if (n != 0 && m != 0)
        print(matrix, n + 1, m + 1);

    free_matrix(matrix, n + 1);

    return dist;
}

void lev_time()
{
    char s1[NMAX], s2[NMAX];

    int REPEATS = 100;

    for (int i = 0; i <= 1000; i++)
    {
        s1[i] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';
        s2[i] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';
    }

    s1[1001] = '\0';
    s2[1001] = '\0';

    uint64_t res_time[6] = {0}, start, end;

    for (int len = 5; len < 6; len += 1)
    {
        s1[len + 1] = '\0';
        s2[len + 1] = '\0';

        for (int times = 0; times < REPEATS; times++)
        {
            start = tick();
            lev_rec(s1, s2);
            end = tick();
            res_time[0] += end - start;
        }

        res_time[0] /= REPEATS;

        s1[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
        s2[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
    }

    printf("Левенштейн(Рекурсия):\n(Длина 5) %ld тиков.\n\n", res_time[0]);

    printf("Левенштейн(Кэширование):\n");
    for (int len = 5; len <= 320; len *= 2)
    {
        s1[len + 1] = '\0';
        s2[len + 1] = '\0';
        res_time[1] = 0;
        for (int times = 0; times < REPEATS; times++)
        {
            start = tick();
            lev_rec_matrix(s1, s2);
            end = tick();
            res_time[1] += end - start;
        }
        res_time[1] /= REPEATS;
        printf("(Длина %d) %ld тиков.\n", len, res_time[1]);
        s1[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
        s2[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
    }

    printf("\nЛевенштейн(Матричный):\n");
    for (int len = 5; len <= 320; len *= 2)
    {
        s1[len + 1] = '\0';
        s2[len + 1] = '\0';
        res_time[2] = 0;
        for (int times = 0; times < REPEATS; times++)
        {
            start = tick();
            lev_iter(s1, s2);
            end = tick();
            res_time[2] += end - start;
        }
        res_time[2] /= REPEATS;
        printf("(Длина %d) %ld тиков.\n", len, res_time[2]);
        s1[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
        s2[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
    }

    printf("\nДамерау-Левенштейн(Рекурсия):\n");
    for (int len = 5; len < 6; len += 1)
    {
        s1[len + 1] = '\0';
        s2[len + 1] = '\0';
        res_time[3] = 0;
        for (int times = 0; times < REPEATS; times++)
        {
            start = tick();
            dam_lev_rec(s1, s2);
            end = tick();
            res_time[3] += end - start;
        }
        res_time[3] /= REPEATS;
        printf("(Длина %d) %ld тиков.\n", len, res_time[3]);
        s1[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
        s2[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
    }

    printf("\nДамерау-Левенштейн(Кэширование):\n");
    for (int len = 5; len <= 160; len *= 2)
    {
        s1[len + 1] = '\0';
        s2[len + 1] = '\0';
        res_time[4] = 0;
        for (int times = 0; times < REPEATS; times++)
        {
            start = tick();
            dam_lev_rec_matrix(s1, s2);
            end = tick();
            res_time[4] += end - start;
        }
        res_time[4] /= REPEATS;
        printf("(Длина %d) %ld тиков.\n", len, res_time[4]);
        s1[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
        s2[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
    }

    printf("\nДамерау-Левенштейн(Матричный):\n");
    for (int len = 5; len <= 160; len *= 2)
    {
        s1[len + 1] = '\0';
        s2[len + 1] = '\0';
        res_time[5] = 0;
        for (int times = 0; times < REPEATS; times++)
        {
            start = tick();
            dam_lev_iter(s1, s2);
            end = tick();
            res_time[5] += end - start;
        }
        res_time[5] /= REPEATS;
        printf("(Длина %d) %ld тиков.\n", len, res_time[5]);
        s1[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
        s2[len + 1] = 49 + (74-49) * (double)rand() / (double)RAND_MAX + '0';;
    }

    printf("\n\n");
}