/*! <!--
 * The copyright notice, author information, etc. for this file is in the corresponding header file
 * (named in the @copydoc doxygen command below).
 * -->
 * @file randomWH.c
 * @copydoc randomWH.h
 */

#include "randomWH.h"

Congruence congruent = {
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
}; /*!< The initialised congruent struct - @see congruent*/

/*!
 * @brief Initialise the generator with four values of type @c long @c int
 *
 * We need to populate the <tt> congruent </tt> structure with seed values before calling @c randomWH().
 * This function isolates the initialisation from the implementation.
 * <p><b>Usage</b><p>
 * see Congruence.init()
 * @code
 *	#include "randomWH.h"
 * .
 * .
 * .
 *      Congruence_init(&congruent,1L,2L,3L,4L);
 * @endcode
 * @param x The seed for the first generator
 * @param y The seed for the second generator
 * @param z The seed for the third generator
 * @param t The seed for the fourth generator
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @note Initialising the generator with the same seeds from program run to program run will produce <em> exactly the same sequence </em>.
 * Usually in simulation programs (particularly when testing and debugging them), this is <b>exactly what you want</b>.
 * @warning Private function. Do not call it directly.
 */

void Congruence_init(Congruence *self, long int x, long int y, long int z, long int t)
{
    if (x <= 0 || y <= 0 || z <= 0 || t <= 0)
        randomWH_error("Seeds must be positive!");
    self->x = x;
    self->y = y;
    self->z = z;
    self->t = t;
    if (self->initialised)
        return;
    self->initialised = istrue;
    switch (sizeof(long int) / 8)
    {
    case 1:
        self->randomWH = randomWH_64;
        break;
    case 0:
        self->randomWH = randomWH_32;
        break;
    default:
        randomWH_error("Could not initialise randomWH function pointer");
    }
}

/*!
 * @brief A very basic error handling function
 *
 * This just prints an error message to @c stderr and then exits.
 * <p><b>Usage</b><p>
 * @code
 *	#include "randomWH.h"
 * .
 * .
 * .
 * 		randomWH_error("A pertinent error message");
 * @endcode
 * @param message A byte string containing a message about the error.
 * @return The function never returns. The program is exited with status EXIT_FAILURE.
 */

void randomWH_error(const char *message)
{
    char prefix[] = "Error in randomWH.c: ";
    fprintf(stderr, "%s %s\n", prefix, message);
    exit(EXIT_FAILURE);
}

/*!
 * @brief Read the current state of the pseudo-random number generator
 *
 * <p>@b Usage<p>
 * see Congruence.read()
 * @code
 *	#include <stdio.h>
 *	#include "randomWH.h"
 * .
 * .
 * .
 * 	    long int* read_con;
 * 	    int count;
 * 	    read_con=Congruence_read(&congruent);
 * 	    for(count=0;count<4;count++)
 * 		    printf("%ld\n", read_con[count]);
 * @endcode
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @return A pointer to an array of @b four <tt> long int </tt> values .
 * @see Congruence_init()
 * @note This function can be used immediately prior to program shutdown to read the current seed values prior
 * to saving them in persistent storage. Loading these values from persistent storage during program initialisation
 * and using them as the arguments in a call to Congruence_init() will allow execution to resume exactly as if the previous
 * program run had continued.
 * @warning Only @b four values are returned. Do @b not use an index greater than 3 to access the array.
 * @warning Private function. Do not call it directly.
 */

long int *Congruence_read(Congruence *self)
{
    return (long int *)&self->x;
}

/*!
 * @brief Generate an error message and triggers a program exit if the calling struct has not been initialised properly.
 *
 * This simulates exception handling for the exception "you forgot to read the documentation".
 *
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @return nothing - the program will exit, returning EXIT_FAILURE.
 * @see Congruence_init()
 * @see Congruence_read()
 */

double randomWH_null(Congruence *self)
{
    fprintf(stderr, "Could not initialise randomWH function pointer\n");
    exit(EXIT_FAILURE);
}

/*!
 * @brief Generate a (pseudo-)random number drawn from the U(0,1) distribution using the 32-bit W-H algorithm .
 *
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @return a @c double value drawn (to a good approximation) from a uniform distribution in the range [0,1]
 * @see Congruence_init()
 * @see Congruence_read()
 * @see  http://www.sciencedirect.com/science/article/pii/S0167947306001836?np=y
 * @warning Never call this function directly as it requires struct initialisation before use.
 * No test is made for correct initialisation here in the interest of performance. Bad things will happen
 * if you bypass initialisation.
 */

double randomWH_32(Congruence *self)
{
    double result;
    self->x = 11600L * (self->x % 185127L) - 10379L * (self->x / 185127L);
    self->y = 47003L * (self->y % 45688L) - 10479L * (self->y / 45688L);
    self->z = 23000L * (self->z % 93368L) - 19423L * (self->z / 93368L);
    self->t = 33000L * (self->t % 65075L) - 8123L * (self->t / 65075L);
    if (self->x < 0)
        self->x += 2147483579L;
    if (self->y < 0)
        self->y += 2147483543L;
    if (self->z < 0)
        self->z += 2147483423L;
    if (self->t < 0)
        self->t += 2147483123L;
    result = (double)(self->x) * 0.0000000004656613022697297188506231646486 + (double)(self->y) * 0.0000000004656613100759859932486569933169 + (double)(self->z) * 0.0000000004656613360968421314794009471615 + (double)(self->t) * 0.0000000004656614011489951998100056779817;
    while (result > 1.0)
        result -= 1.0;
    return result;
}

/*!
 * @brief Generate a (pseudo-)random number drawn from the U(0,1) distribution using the 64-bit W-H algorithm .
 *
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @return a @c double value drawn (to a good approximation) from a uniform distribution in the range [0,1]
 * @see Congruence_init()
 * @see Congruence_read()
 * @see  http://www.sciencedirect.com/science/article/pii/S0167947306001836?np=y
 * @warning Never call this function directly as it requires struct initialisation before use.
 * No test is made for correct initialisation here in the interest of performance. Bad things will happen
 * if you bypass initialisation.
 */

double randomWH_64(Congruence *self)
{
    double result;
    self->x = (11600L * self->x) % 2147483579L;
    self->y = (47003L * self->y) % 2147483543L;
    self->z = (23000L * self->z) % 2147483423L;
    self->t = (33000L * self->t) % 2147483123L;
    result = (double)(self->x) * 0.0000000004656613022697297188506231646486 + (double)(self->y) * 0.0000000004656613100759859932486569933169 + (double)(self->z) * 0.0000000004656613360968421314794009471615 + (double)(self->t) * 0.0000000004656614011489951998100056779817;
    while (result > 1.0)
        result -= 1.0;
    return result;
}

/*!
 * @brief Generate a geometrically distributed random variate
 *
 * <p><b>Usage</b><p>
 * see Congruence.geometric()
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @param prob The probability of success in each trial
 */

long int randomWH_geometric(Congruence *self, double prob)
{
    double unif = self->randomWH(self);
    double p_bar = 1 - prob;
    long int result = 1L;
    if (p_bar > 0)
    {
        result += log(unif) / log(p_bar);
    }
    return result;
}

/*!
 * @brief Generate a Bernoulli random variate
 *
 * <p><b>Usage</b><p>
 * see Congruence.bernoulli()
 * @code
 *	#include <stdio.h>
 *	#include "randomWH.h"
 *  Congruence generatorX={
 *    .x=0,
 *    .y=0,
 *    .z=0,
 *    .t=0,
 *    .initialised=isfalse,
 *    .init=Congruence_init,
 *    .randomWH=randomWH_null,
 *    .read=Congruence_read,
 *    .bernoulli=randomWH_bernoulli,
 *    .geometric=randomWH_geometric,
 *    .binomial=randomWH_binomial,
 *    .fairdie=randomWH_fairdie,
 * };
 * .
 * .
 * .
 *   	if (generatorX.bernoulli(&generatorX,0.5)
 *          printf("Heads!\n");
 *      else
  *         printf("Tails!\n");
 * @endcode
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @param prob The probability of success
 * @return a BOOLEAN variable indicating an outcome of success or failure
 */

enum BOOLEAN randomWH_bernoulli(Congruence *self, double prob)
{
    double unif = self->randomWH(self);
    enum BOOLEAN result = isfalse;

    if (unif < prob)
    {
        result = istrue;
    }
    return result;
}

/*!
 * @brief Generate a binomially distributed random variate
 *
 * <p><b>Usage</b><p>
 * see Congruence.binomial()
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @param prob The probability of success in each trial
 * @param trials The number of trials
 */

long int randomWH_binomial(Congruence *self, long int trials, double prob)
{
    long int result = 0;
    long int count;

    for (count = 0; count < trials; count++)
        result += self->bernoulli(self, prob);
    return result;
}

/*!
 * @brief Generate a discrete-valued uniformly distributed random variate drawn from a specified range
 *
 * <p><b>Usage</b><p>
 * see Congruence.fairdie()
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @param lower_inclusive the lowest value of variate that can be returned
 * @param upper_inclusive the highest value of variate that can be returned
 */

long int randomWH_fairdie(Congruence *self, long int lower_inclusive, long int upper_inclusive)
{
    long int result;
    double randvar = self->randomWH(self);
    if (randvar == 1.0)
        return upper_inclusive;
    randvar = lower_inclusive + randvar * (upper_inclusive - lower_inclusive + 1);
    result = randvar; /* compiler will assign result <- floor(randvar) */
    return result;
}
