#include <stdio.h>
#include <stdlib.h>

#include "randomWH.h"
#include "./rbTree/rbtree.h"
#include "./rbTree/fatal.h"
#include "./rbTree/nodeEntry.h"

#define SIM_DURATION 40000000L
#define N 100

Congruence congruent2 = {
    .x = 0,
    .y = 0,
    .z = 1541,
    .t = 0402,
    .initialised = isfalse,
    .init = Congruence_init,
    .randomWH = randomWH_null,
    .read = Congruence_read,
    .bernoulli = randomWH_bernoulli,
    .geometric = randomWH_geometric,
    .binomial = randomWH_binomial,
    .fairdie = randomWH_fairdie,
};

void Congruence_dump(Congruence *generatorZ);

void Congruence_dump(Congruence *generatorZ)
{
    long int *read_con;
    int count;
    char seednames[] = {'x', 'y', 'z', 't'};
    read_con = generatorZ->read(generatorZ);
    for (count = 0; count < 4; count++)
        printf("seed %c = %ldL\n", seednames[count], read_con[count]);
}

int main()
{

    RedBlackTree T;
    Position P;
    int i;
    int j = 0;
    long int count;

    T = Initialize();
    T = MakeEmpty(T);
    Congruence *sequence = &congruent2;
    struct node_entry test[100];
    /* Generate a sequence and calculate its first and second moment */
    congruent.init(&congruent, 1L, 2L, 3L, 4L);
    for (int i = 0; i < N; i++)
    {
        test[i].finish_key = congruent.fairdie(&congruent, 0L, 999L);
        test[i].vpn = i;
        PrintValuePair(test[i]);
        T = Insert(test[i], T);
    }

    printf("Inserts are complete\n");

    printf("Start\n");
    PrintTree(T);
    printf("End\n");
    
    printf("Min is %ld, Max is %ld\n", Retrieve(FindMin(T)),
           Retrieve(FindMax(T)));

    return EXIT_SUCCESS;
}
