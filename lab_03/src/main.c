#include "sort.h"
#include "utils.h"

int main()
{
    int action = 0, rc;
    int a[NMAX], copy[NMAX], n = NMAX;

    while (action != EXIT)
    {
        if ((rc = choose_action(&action)))
            return rc;
        if (action == SORT)
        {
            if ((rc = read_dimension(&n)))
                return rc;
            if ((rc = read_array(a, n)))
                return rc;

            copy_array(a, copy, n);
            bubble_sort(copy, n);
            printf("\nРезультат сортировки пузырьком:\n");
            print_array(copy, n);

            copy_array(a, copy, n);
            select_sort(copy, n);
            printf("\nРезультат сортировки выбором:\n");
            print_array(copy, n);

            copy_array(a, copy, n);
            gnome_sort(copy, n);
            printf("\nРезультат гномьей сортировки:\n");
            print_array(copy, n);
        }
        if (action == ANALYSIS)
        {
            random_analysis();
            sorted_analysis();
            antisorted_analysis();
        }
    }

    return EXIT_SUCCESS;
}