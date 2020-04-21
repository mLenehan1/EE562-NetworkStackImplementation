/*!
 * @file randomWH_usage.h
 * @author Martin Collier
 * @date 11 February 2014
 * @brief A test program to evaluate the pseudorandom number generator
 *
 * @see randomWH.h
 *
 * @version 0.2 Alpha
 *
 *
 * <B> @em Usage </B><P>
 * Assuming that all your source files are in the one directory, and you are using the Gnu compiler, type
 * (in a Linux terminal) "gcc -o test randomWH_usage.c randomWH.c -lm", followed by "./test". In a Windows OS,
 * it may be simpler to let your IDE figure out the command line.
 *
 * The latest version can also be built using the command " g++ -std=c++11 -o ccc.exe randomWH_usage.c randomWH.c".
 * This means that you can use C++ syntax in randomWH_usage.c or replace it in the command line with (say) assignment.cpp
 *
 *
 * @copyright
 *   &copy; 2014 Martin Collier.
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0.
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */
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
    long int count;
    double mean = 0, moment2 = 0, unif, variance, prob;
    Congruence *sequence = &congruent2;
    /* Generate a sequence and calculate its first and second moment */
    congruent.init(&congruent, 1L, 2L, 3L, 4L);
    for (count = 0; count < SIM_DURATION; count++)
    {
        unif = congruent.randomWH(&congruent);
        mean += unif;
        moment2 += (unif * unif);
    }
    mean /= SIM_DURATION;
    moment2 /= (SIM_DURATION - 1.0);                                          /* using Bessel's correction */
    variance = moment2 - mean * mean * (SIM_DURATION) / (SIM_DURATION - 1.0); /* Bessel's correction continued */
    printf("Mean after %ld iterations is %lf; sample variance is %lf\n", SIM_DURATION, mean, variance);
    printf("Seeds to use for next run:\n");
    Congruence_dump(&congruent);

    printf("\n");

    /* Generate the same sequence, using a second instance of the generator */
    mean = moment2 = 0;
    sequence->init(sequence, 1L, 2L, 3L, 4L);
    for (count = 0; count < SIM_DURATION; count++)
    {
        unif = sequence->randomWH(sequence);
        mean += unif;
        moment2 += (unif * unif);
    }
    mean /= SIM_DURATION;
    moment2 /= (SIM_DURATION - 1.0);                                          /* using Bessel's correction */
    variance = moment2 - mean * mean * (SIM_DURATION) / (SIM_DURATION - 1.0); /* Bessel's correction continued */
    printf("Mean after %ld iterations is %lf; sample variance is %lf\n", SIM_DURATION, mean, variance);
    printf("Seeds to use for next run:\n");
    Congruence_dump(sequence);

    /* Estimate means of some distributions */
    prob = 0.1;
    mean = 0.0;
    for (count = 1; count <= 50000; count++)
        mean += congruent.geometric(&congruent, prob);
    printf("\nMean number of tries until first success, each trial with probability %lf: %lf\n", prob, mean / (count - 1));
    prob = 0.5;
    mean = 0.0;
    for (count = 1; count <= 50000; count++)
        mean += congruent.bernoulli(&congruent, prob) == istrue ? 1 : -1;
    printf("\n%ld fair coin tosses: Mean: %lf (should approach zero)\n", count - 1, mean / (count - 1));
    prob = 0.666666666666666666;
    mean = 0.0;
    for (count = 1; count <= 50000; count++)
        mean += congruent.binomial(&congruent, 30L, prob);
    printf("\nMean number of successes in 30 trials with prob. of success %lf is %lf", prob, mean / (count - 1));
    printf("\n");

    mean = 0.0;
    for (count = 1; count <= 50000; count++)
        mean += congruent.fairdie(&congruent, 1L, 6L);
    printf("\nMean result of %ld dice throws: %lf", count - 1, mean / (count - 1));
    printf("\n");

    return EXIT_SUCCESS;
}
