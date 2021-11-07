#include <stdio.h>
#include <stdlib.h>

#define True 1
#define False 0

int main(void)
{
    int n = 3;                             // (1)
    int a[n];                              // (2)
    int swapped = True;                    // (3)

    for (int i = 0; i < n; i++)            // (4)
        a[i] = n * n - 5 * i * i + i;      // (5)

    int j = -1;                            // (6)
    int temp;                              // (7)
    while (swapped)                        // (8)
    {
        swapped = False;                   // (9)
        j += 1;                            // (10)

        for (int i = 1; i < n - j; i++)    // (11)
            if (a[i - 1] > a[i])           // (12)
            {
                temp = a[i - 1];           // (13)
                a[i - 1] = a[i];           // (14)
                a[i] = temp;               // (15)
                swapped = True;            // (16)
            }
    }

    for (int i = 0; i < n; i++)            // (17)
        printf("%d ", a[i]);               // (18)

    return 0;                              // (19)
}