#include "redblack.h"
#include <stdio.h>
#include <stdlib.h>

#include "randomWH.h"
#define SIM_DURATION 40000000L

/* Example of struct initialisation
 * A struct called congruent is pre-initialised for you
 */

Congruence congruent2 = {
    .x = 0,
    .y = 0,
    .z = 0,
    .t = 0,
    .initialised = isfalse,
    .init = Congruence_init,
    .randomWH = randomWH_null,
    .read = Congruence_read,
    .bernoulli = randomWH_bernoulli,
    .geometric = randomWH_geometric,
    .binomial = randomWH_binomial,
    .fairdie = randomWH_fairdie,
};

#define N 100

main()
{
    long int count;
    double mean = 0, moment2 = 0, unif, variance, prob;
    Congruence *sequence = &congruent2;
    /* Generate a sequence and calculate its first and second moment */
    congruent.init(&congruent, 1L, 1L, 1541L, 402L);
    RedBlackTree T;
    Position P;
    int i;
    int j = 0;

    T = Initialize();
    T = MakeEmpty(T);

    for (i = 0; i < N; i++, j = (j + congruent.fairdie(&congruent, 0L, 99L)))
    {
        T = Insert(j, T);
        printf("%d\n", j);
        j = 0;
    }
    printf("Inserts are complete\n");

    for (i = 0; i < N; i++)
        if ((P = Find(i, T)) == NULL || Retrieve(P) != i)
            printf("Error at %d\n", i);

    printf("Min is %d, Max is %d\n", Retrieve(FindMin(T)),
           Retrieve(FindMax(T)));

    return 0;
}