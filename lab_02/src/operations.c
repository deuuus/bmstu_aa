#include "operations.h"

matrix_t *math_mult(const matrix_t *A, const matrix_t *B)
{
    if (A->dim2 != B->dim1)
        return NULL;

    int **ptr = allocate_matrix(A->dim1, B->dim2);
    if (!ptr)
        return NULL;

    matrix_t *C = malloc(sizeof(matrix_t));
    if (!C)
        return NULL;
    fill(C, ptr, A->dim1, B->dim2);

    int i, j, k, M = A->dim1, N = A->dim2, K = B->dim2;
    for (i = 0; i < M; i++)
        for (j = 0; j < K; j++)
            (C->ptr)[i][j] = 0;

    for (i = 0; i < M; i++)
        for (j = 0; j < K; j++)
            for (k = 0; k < N; k++)
                (C->ptr)[i][j] = (C->ptr)[i][j] + (A->ptr)[i][k] * (B->ptr)[k][j];
    return C;
}

matrix_t *coppersmith_winograd(const matrix_t *A, const matrix_t *B)
{
    if (A->dim2 != B->dim1)
        return NULL;
    
    int **ptr = allocate_matrix(A->dim1, B->dim2);
    if (!ptr)
        return NULL;

    matrix_t *C = malloc(sizeof(matrix_t));
    if (!C)
    {
        free_matrix(ptr, A->dim1);
        return NULL;
    }
    fill(C, ptr, A->dim1, B->dim2);

    int i, j, k, M = A->dim1, N = A->dim2, K = B->dim2;

    vector_t *mh = malloc(sizeof(vector_t));
    if (!mh)
    {
        free(C);
        return NULL;
    }

    vector_t *mv = malloc(sizeof(vector_t));
    if (!mv)
    {
        free(mh);
        free(C);
        return NULL;
    }

    mh = make_mh(A);
    if (!mh)
    {
        free(mv);
        free(C);
        return NULL;
    }
    mv = make_mv(B);
    if (!mv)
    {
        free(mh);
        free(C);
        return NULL;
    }

    for (i = 0; i < M; i++)
        for (j = 0; j < K; j++)
            (C->ptr)[i][j] = 0;
    
    for (i = 0; i < M; i++)
        for (j = 0; j < K; j++)
        {
            for (k = 0; k < N / 2; k++)
            {
                (C->ptr)[i][j] = (C->ptr)[i][j] + ((A->ptr)[i][k * 2] + (B->ptr)[k * 2 + 1][j]) *
                                  ((A->ptr)[i][k * 2 + 1] + (B->ptr)[k * 2][j]);
            }
            (C->ptr)[i][j] = (C->ptr)[i][j] - ((mh->ptr)[i] + (mv->ptr)[j]);
        }

    if (N % 2 == 1)
    {
        for (i = 0; i < M; i++)
            for (j = 0; j < K; j++)
                (C->ptr)[i][j] = (C->ptr)[i][j] + (A->ptr)[i][N - 1] * (B->ptr)[N - 1][j];
    }

    free(mh);
    free(mv);
    
    return C;
}

vector_t *make_mh(const matrix_t *A)
{
    int M = A->dim1, N = A->dim2;
    int *ptr = malloc(sizeof(int) * M);
    if (!ptr)
        return NULL;

    vector_t *mh = malloc(sizeof(vector_t));
    if (!mh)
        return NULL;
    mh->ptr = ptr;
    mh->size = M;

    for (int i = 0; i < M; i++)
        (mh->ptr)[i] = 0;
 
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N / 2; j++)
            (mh->ptr)[i] = (mh->ptr)[i] + (A->ptr)[i][2 * j] * (A->ptr)[i][2 * j + 1];
    
    return mh;
}

vector_t *make_mv(const matrix_t *B)
{
    int N = B->dim1, K = B->dim2;
    int *ptr = malloc(sizeof(int) * K);
    if (!ptr)
        return NULL;

    vector_t *mv = malloc(sizeof(vector_t));
    if (!mv)
        return NULL;
    mv->ptr = ptr;
    mv->size = K;

    for (int i = 0; i < K; i++)
        (mv->ptr)[i] = 0;
 
    for (int i = 0; i < N / 2; i++)
        for (int j = 0; j < K; j++)
            (mv->ptr)[j] = (mv->ptr)[j] + (B->ptr)[2 * i][j] * (B->ptr)[2 * i + 1][j]; // 15
    
    return mv;
}

matrix_t *upd_coppersmith_winograd(const matrix_t *A, const matrix_t *B)
{
    if (A->dim2 != B->dim1)
        return NULL;
    
    int **ptr = allocate_matrix(A->dim1, B->dim2);
    if (!ptr)
        return NULL;

    matrix_t *C = malloc(sizeof(matrix_t));
    if (!C)
    {
        free_matrix(ptr, A->dim1);
        return NULL;
    }
    fill(C, ptr, A->dim1, B->dim2);

    int i, j, k, M = A->dim1, N = A->dim2, K = B->dim2;

    vector_t *mh = malloc(sizeof(vector_t));
    if (!mh)
    {
        free(C);
        return NULL;
    }

    vector_t *mv = malloc(sizeof(vector_t));
    if (!mv)
    {
        free(mh);
        free(C);
        return NULL;
    }

    mh = make_mh(A);
    if (!mh)
    {
        free(mv);
        free(C);
        return NULL;
    }
    mv = make_mv(B);
    if (!mv)
    {
        free(mh);
        free(C);
        return NULL;
    }

    for (i = 0; i < M; i++)
        for (j = 0; j < K; j++)
            (C->ptr)[i][j] = 0;
    
    for (i = 0; i < M; i++)
        for (j = 0; j < K; j++)
        {
            for (k = 0; k < N - 1; k+=2)
            {
                (C->ptr)[i][j] += ((A->ptr)[i][k] + (B->ptr)[k + 1][j]) *
                                  ((A->ptr)[i][k + 1] + (B->ptr)[k][j]); //17
            }
            (C->ptr)[i][j] -= (mh->ptr)[i] + (mv->ptr)[j];
        }

    if (N % 2 == 1)
    {
        for (i = 0; i < M; i++)
            for (j = 0; j < K; j++)
                (C->ptr)[i][j] += (A->ptr)[i][N - 1] * (B->ptr)[N - 1][j];
    }

    free(mh);
    free(mv);
    
    return C;
}

vector_t *upd_make_mh(const matrix_t *A)
{
    int M = A->dim1, N = A->dim2;
    int *ptr = malloc(sizeof(int) * M);
    if (!ptr)
        return NULL;

    vector_t *mh = malloc(sizeof(vector_t));
    if (!mh)
        return NULL;
    mh->ptr = ptr;
    mh->size = M;

    for (int i = 0; i < M; i++)
        (mh->ptr)[i] = 0;
 
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j+=2)
            (mh->ptr)[i] += (A->ptr)[i][j] * (A->ptr)[i][j + 1];
    
    return mh;
}

vector_t *upd_make_mv(const matrix_t *B)
{
    int N = B->dim1, K = B->dim2;
    int *ptr = malloc(sizeof(int) * K);
    if (!ptr)
        return NULL;

    vector_t *mv = malloc(sizeof(vector_t));
    if (!mv)
        return NULL;
    mv->ptr = ptr;
    mv->size = K;

    for (int i = 0; i < K; i++)
        (mv->ptr)[i] = 0;
 
    for (int i = 0; i < N; i+=2)
        for (int j = 0; j < K; j++)
            (mv->ptr)[j] += (B->ptr)[i][j] * (B->ptr)[i + 1][j];
    
    return mv;
}

void _analysis(void)
{
    FILE *f;
    f = fopen("result.csv", "w");
    for (int i = 100; i <= 1000; i += 100)
        _calc(i, f);
    for (int i = 101; i <= 1001; i += 100)
        _calc(i, f);
    fclose(f);
}

void _calc(int size, FILE *f)
{

    int **ptr_A = allocate_matrix(NMAX, NMAX);
    int **ptr_B = allocate_matrix(NMAX, NMAX);

    matrix_t *A = malloc(sizeof(matrix_t));
    matrix_t *B = malloc(sizeof(matrix_t));

    generate_matrix(ptr_A, size, size);
    generate_matrix(ptr_B, size, size);

    fill(A, ptr_A, size, size);
    fill(B, ptr_B, size, size);

    int **ptr_C = allocate_matrix(size, size);
    matrix_t *C = malloc(sizeof(matrix_t));
    fill(C, ptr_C, size + 1, size + 1);

    uint64_t time[3] = {0}, start, end;

    for (int i = 0; i < REPEATS; i++)
    {
        start = tick();
        C = math_mult(A, B);
        end = tick();
        free(C);
        time[0] += end - start;

        start = tick();
        C = coppersmith_winograd(A, B);
        end = tick();
        free(C);
        time[1] += end - start;

        start = tick();
        C = upd_coppersmith_winograd(A, B);
        end = tick();
        free(C);
        time[2] += end - start;
    }

    for (int i = 0; i < 3; i++)
        time[i] /= REPEATS;
    
    printf("\nСтандартный алгоритм умножения матриц, M = N = K = %d,            %ld тиков.", size, time[0]);
    printf("\nАлгоритм Копперсмита-Винограда, M = N = K = %d,                   %ld тиков.", size, time[1]);
    printf("\nАлгоритм Копперсмита-Винограда(оптимизированный), M = N = K = %d, %ld тиков.\n", size, time[2]);

    fprintf(f, "%ld,%ld,%ld\n", time[0], time[1], time[2]);

    free(A);
    free(B);
}