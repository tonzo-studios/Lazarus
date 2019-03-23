#pragma once

#include <random>

#include <lazarus/common.h>

namespace lz
{
class Random
{
public:
    /**
     * Sets a new seed for the random generator using a hardware random device if
     * available, or a seed using the current time.
     */
    static void seed();

    /**
    * Return a random integer between the two given numbers with equal probability.
    * 
    * If a < b, then it will return an integer in [a, b].
    * Otherwise, it will be in [b, a].
    */
    static long range(long a, long b);

    /**
     * Return a random floating point between the two given numbers with equal probability.
     * 
     * If a < b, then it will return a double in [a, b).
     * Otherwise, it will be in [b, a).
     */
    static double rangeReal(double a, double b);

    /**
     * Rolls a dice with the specified number of sides a certain number of times
     * and returns the total result.
     */
    static ulong roll(unsigned sides=6, unsigned times=1);

    /**
     * Return true with a 1 in n probability.
     */
    static bool oneIn(unsigned n);

    /**
     * Return a random number generated from a normal distribution.
     * 
     * The normal (or Gaussian) distribution will use the given mean and
     * standard deviation.
     * 
     * @param mean The mean of the distribution.
     * @param stdev The standard deviation of the distribution.
     */
    static double normal(double mean, double stdev);

private:
    static std::mt19937 generator;
};
}