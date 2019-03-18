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
    * Return a random integer between the two given numbers, both ends included.
    * 
    * If a < b, then it will return an integer in [a, b].
    * Otherwise, it will be between [b, a].
    */
    static long range(long a, long b);

    /**
     * Rolls a dice with the specified number of sides a certain number of times
     * and returns the total result.
     */
    static ulong roll(unsigned sides=6, unsigned times=1);

    /**
     * Return true with a 1 in n probability.
     */
    static bool oneIn(unsigned n);

private:
    static std::mt19937 generator;
};
}