/*!
 * @file randomWH.h
 * @author Martin Collier
 * @date 11 February 2014
 * @brief A pseudorandom number generator with good statistical properties.
 *
 * @see http://www.sciencedirect.com/science/article/pii/S0167947306001836?np=y
 * @see http://www.viva64.com/en/a/0004/
 *
 * @version 0.3 Alpha
 *
 * @details
 *
 * <B> Details </B><P>
 * This implements the algorithm described by Wichmann and Hill in
 *  -    B.A. Wichmann and I.D. Hill, "Generating good pseudo-random numbers", <em> Computational Statistics & Data Analysis</em>, vol. 51, no. 3., pp. 1614-1622, Dec. 2006.
 * <p>
 * Two implementations are provided. One is for compilers that use 32 bits to store variables of the <c>long int</c> type. A faster algorithm is also provided
 * for use with compilers that feature a 64-bit <c>long int</c> type. The algorithm to be used is determined automatically at run time.
 *
 * @mainpage A library for generating uniformly distributed random variates
 * This implements the algorithm described by Wichmann and Hill in
 *  -    B.A. Wichmann and I.D. Hill, "Generating good pseudo-random numbers", <em> Computational Statistics & Data Analysis</em>, vol. 51, no. 3., pp. 1614-1622, Dec. 2006.
 * <p>
 *
 * Two implementations are provided. One is for compilers that use 32 bits to store variables of the <c>long int</c> type. A faster algorithm is also provided
 * for use with compilers that feature a 64-bit <c>long int</c> type. The algorithm to be used is determined automatically at run time.
 *
 * @remarks
 *
 * - v 0.2 alpha - first public release
 * - Changes for v 0.3 alpha
 *     -  renamed @c congruence  to Congruence
 *     - Changed @c Congruence struct initialisation to avoid
 *      "sorry, unimplemented: non-trivial designated initializers not supported"
 *      error in g++ (C++, unlike C, does not support designated initializers - see http://placidrage.bitbucket.org/0-computer/0-development/0-languages/0-c/0-designated-initializers/index.html).
 *     - Changed randomWH_error() @c message argument from @c char*  to @c const @c char*
 *
 *    Code will now compile without error or warnings in g++ as well as gcc
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

#ifndef _RANDOMWH_H
#define _RANDOMWH_H /*!< The usual do-not-load-me-twice macro */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/**
 * A boolean type
 */

enum BOOLEAN /*!< Declares an enumeration data type called BOOLEAN */
{
    isfalse, /*!< false = 0, true = 1 */
    istrue
};

/*!
 * @brief @c Typedef of @c struct to store and process the four generator seeds.
 *
 * In ordinary usage, you will not use this typedef directly. Instead, you will use the
 * instance of it called @c Congruence which is pre-initialised in randomWH.c. An exception would be if
 * your code requires multiple instances of the random number generator. A second instance
 * can be generated using the following code (failure to initialise this correctly will
 * almost always cause fatal errors at run time).
 * @code
 * #include "randomWH.h"
 *
 *
 * Congruence generator2 = {
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
 * @endcode
 * This generator can be accessed using @c generator2.init(), @c generator2.read(), etc. <p>
 * If multiple generators are instantiated, the number sequences generated may overlap; this is <em>certain</em> to occur
 * if two or more are seeded with the same values.
 * @see congruent
 */
typedef struct generator
{
    /*! @name Variables
    */
    /*@{*/
    long int x;               /*!< The seed for the first generator <!-- @ relates congruent --> */
    long int y;               /*!< The seed for the second generator <!-- @ relates congruent --> */
    long int z;               /**< The seed for the third generator <!-- @ relates congruent --> */
    long int t;               /**< The seed for the fourth generator <!-- @ relates congruent --> */
    enum BOOLEAN initialised; /*!< initialised to false; set to true when function pointers updated <!-- @ relates congruent --> */
    /*@}*/
    /*! @name Member Functions
    */
    /*@{*/
    /*!
     * @brief Initialise the generator with four values of type @c long @c int
     *
     * We need to populate the <tt> congruent </tt> structure with seed values before calling @c randomWH().
     * This function isolates the initialisation from the implementation.
     * <p><b>Usage</b><p>
     * @code
     *	#include "randomWH.h"
     *
     * Congruence generatorX={
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
     *      generatorX.init(&generatorX, 1L,2L,3L,4L);
     * @endcode
     * <!-- @ relates congruent -->
     * @param x The seed for the first generator
     * @param y The seed for the second generator
     * @param z The seed for the third generator
     * @param t The seed for the fourth generator
     * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
     * @note Initialising the generator with the same seeds from program run to program run will produce <em> exactly the same sequence </em>.
     * Usually in simulation programs (particularly when testing and debugging them), this is <b>exactly what you want</b>.
     */

    void (*init)(struct generator *self, long int x, long int y, long int z, long int t);
    /*!
     * @brief Generate a (pseudo-)random number drawn from the U(0,1) distribution.
     *
     *
     * <p><b>Usage</b><p>
     * @code
     * #include <stdio.h>
     * #include "randomWH.h"
     * #define SIM_DURATION 40000000L
     * Congruence generatorX={
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
     * 	    long int count;
     * 	    double mean=0, moment2=0, unif, variance;
     * 	    generatorX.init(&generatorX,1L,2L,3L,4L);
     * 	    for(count =0;count<SIM_DURATION;count++){
     * 		    unif = generatorX.randomWH(&generatorX);
     * 		    mean += unif;
     * 		    moment2 += (unif*unif);
     * 	    }
     * 	    mean /= SIM_DURATION;
     * 	    moment2 /= (SIM_DURATION-1.0); // using Bessel's correction
     * 	    variance = moment2-mean*mean*(SIM_DURATION)/(SIM_DURATION-1.0); // Bessel's correction continued
     * 	    printf("Mean after %ld iterations is %lf; sample variance is %lf\n", SIM_DURATION, mean, variance);
     * @endcode
     * @return a @c double value drawn (to a good approximation) from a uniform distribution in the range [0,1]
     * <!-- @ relates congruent -->
     * @see Congruence_init()
     * @see Congruence_read()
     * @see  http://www.sciencedirect.com/science/article/pii/S0167947306001836?np=y
     * @warning Never use a random number generator that you have not tested extensively first. Never use a generator if you do not know and understand the algorithm it implements.
     * @note Initialising the generator with the same seeds from program run to program run will produce <em> exactly the same sequence </em>.
     * Usually in simulation programs (particularly when testing and debugging them), this is <b>exactly what you want</b>.
     */

    double (*randomWH)(struct generator *self);
    /*!
     * @brief Read the current state of the pseudo-random number generator
     *
     * <p><b>Usage</b><p>
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
     * 	    long int* read_con;
     * 	    int count;
     * 	    read_con=generatorX.read(&generatorX);
     * 	    for(count=0;count<4;count++)
     * 		    printf("%ld\n", read_con[count]);
     * @endcode
     * @return A pointer to an array of @b four <tt> long int </tt> values .
     * @see init()
     * <!-- @ relates congruent -->
     * @note This function can be used immediately prior to program shutdown to read the current seed values prior
     * to saving them in persistent storage. Loading these values from persistent storage during program initialisation
     * and using them as the arguments in a call to the relevant .init() will allow execution to resume exactly as if the previous
     * program run had continued.
     * @warning Only @b four values are returned. Do @b not use an index greater than 3 to access the array.
     */

    long int *(*read)(struct generator *self);

    /*!
     * @brief Generate a binomially distributed random variate
     *
     * <p><b>Usage</b><p>
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
     * 	    if (generatorX.bernoulli(&generatorX,0.5)==istrue)
     *          printf("Heads!\n");
     *      else
     *          printf("Tails!\n");
     * @endcode
     * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
     * @param prob The probability of success
     * @return a BOOLEAN variable indicating an outcome of success or failure
     */
    enum BOOLEAN (*bernoulli)(struct generator *self, double prob);

    /*!
 * @brief Generate a geometrically distributed random variate
 *
 * <p><b>Usage</b><p>
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
 * long int patience;
 * .
 * .
 * .
 *
 * 	       patience = generatorX.geometric(&generatorX,1.0/52.0)
 * 	       printf("It took %ld draws before my card appeared\n", patience);
 * @endcode
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @param prob The probability of success in each trial
 */

    long int (*geometric)(struct generator *self, double prob);

    /*!
 * @brief Generate a binomially distributed random variate
 *
 * <p><b>Usage</b><p>
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
 * long int wheelspin;
 * .
 * .
 * .
 * 	    wheelspin = generatorX.binomial(&generatorX,100,1.0/38.0)
 *      printf("My (un)lucky number came up %ld times in 100 spins of a roulette wheel\n", wheelspin);
 * @endcode
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @param trials The number of trials
 * @param prob The probability of success in each trial
 */

    long int (*binomial)(struct generator *self, long int trials, double prob);

    /*!
 * @brief Generate a discrete-valued uniformly distributed random variate drawn from a specified range
 *
 * <p><b>Usage</b><p>
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
 * long int lotto;
 * .
 * .
 * .
 *
 *      lotto = generatorX.fairdie(&generatorX,1L,45L)
 *      printf("The first Lotto number drawn was %ld\n", lotto);
 * @endcode
 * @param self A pointer to the calling @c struct (required since C does not support the @c@b this keyword).
 * @param lower_inclusive the lowest value of variate that can be returned
 * @param upper_inclusive the highest value of variate that can be returned
 */

    long int (*fairdie)(struct generator *self, long int lower_inclusive, long int upper_inclusive);

    /*@}*/

} Congruence;

void Congruence_init(Congruence *self, long int x, long int y, long int z, long int t);
long int *Congruence_read(Congruence *self);
double randomWH_32(Congruence *self);
double randomWH_64(Congruence *self);
double randomWH_null(Congruence *self);
void randomWH_error(const char *message);
long int randomWH_geometric(Congruence *self, double prob);
enum BOOLEAN randomWH_bernoulli(Congruence *self, double prob);
long int randomWH_binomial(Congruence *self, long int trials, double prob);
long int randomWH_fairdie(Congruence *self, long int lower_inclusive, long int upper_inclusive);

/*! @struct congruent
 * @brief An instance of the @c Congruence @c struct.
 *
 * The four seeds used by the generator are stored in this @c struct.
 * Because they must have global scope, they are contained within a structure
 * to minimise the chances of a name conflict with other variables of global scope.
 * The functions to process these values are accessed via function pointers contained
 * within the struct. This simulates the member function concept of C++ using
 * only C code. Most applications will require only a single generator instance - this one
 * has safe values assigned to the member function pointers.
 * <p><b>Usage</b><p>
 * @code
 * #include <stdio.h>
 * #include "randomWH.h"
 * #define SIM_DURATION 40000000L
 * .
 * .
 * .
 *
 *  double mean=0, moment2=0, unif, variance;
 * 	long int count;
 *  congruent.init(&congruent,1L,2L,3L,4L);
 * 	for(count =0;count<SIM_DURATION;count++){
 * 		unif = congruent.randomWH(&congruent);
 * 		mean += unif;
 * 		moment2 += (unif*unif);
 * 	}
 * 	mean /= SIM_DURATION;
 * 	moment2 /= (SIM_DURATION-1.0); // using Bessel's correction
 * 	variance = moment2-mean*mean*(SIM_DURATION)/(SIM_DURATION-1.0); // Bessel's correction continued
 * 	printf("Mean after %ld iterations is %lf; sample variance is %lf\n", SIM_DURATION, mean, variance);
 * @endcode
 * @see Congruence
 * @note You may create other generators by declaring other structs of type
 * @c Congruence but you <b>must</b> initialise them correctly.
 * <!-- Due to a deficiency in doxygen
 * (or in the code maintainer's understanding thereof), the documentation advises you, regarding the "Related Functions"
 * to "(Note that these are not member functions.)".  Ignore this; these <em><b>are</b></em> members of @c congruent. You can
 * access, for example the @c init() member as @c congruent.init(). -->
 */
extern Congruence congruent;

#endif /*_RANDOMWH_H */
