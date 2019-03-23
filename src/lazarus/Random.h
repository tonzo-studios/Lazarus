#pragma once

#include <random>
#include <type_traits>

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
    * Return a random integral between the two given numbers with equal probability.
    * 
    * If a < b, then it will return an integral in [a, b].
    * Otherwise, it will be in [b, a].
    * 
    * Type is automatically detected, and the return type will be the most
    * appropriate type depending on the argument types.
    * For example, range(int, unsigned) will return an unsigned, and
    * range(short, long) will return a long.
    */
    template <typename T, typename U,
              typename std::enable_if_t<
                (std::is_integral<T>::value || std::is_unsigned<T>::value)
                && (std::is_integral<U>::value || std::is_unsigned<U>::value)
                >* = nullptr>
    static typename std::common_type<T, U>::type range(T a, U b)
    {
        typedef typename std::common_type<T, U>::type common_type;
        auto _a = static_cast<common_type>(a);
        auto _b = static_cast<common_type>(b);
        if (_a > _b)
            std::swap(_a, _b);
        return std::uniform_int_distribution<common_type>(_a, _b)(generator);
    }

    /**
    * Return a random floating point number between the two given numbers
    * with equal probability.
    * 
    * If a < b, then it will return an integral in [a, b).
    * Otherwise, it will be in [b, a).
    * 
    * Type is automatically detected, and the return type will be the most
    * appropriate type depending on the argument types.
    * For example, range(float, double) will return a double, and
    * range(int, float) will return a float.
    */
    template <typename T, typename U,
              typename std::enable_if_t<
                std::is_floating_point<T>::value || std::is_floating_point<U>::value
                >* = nullptr>
    static typename std::common_type<T, U>::type range(T a, U b)
    {
        typedef typename std::common_type<T, U>::type common_type;
        auto _a = static_cast<common_type>(a);
        auto _b = static_cast<common_type>(b);
        if (_a > _b)
            std::swap(_a, _b);
        return std::uniform_real_distribution<common_type>(_a, _b)(generator);
    }

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
}  // namespace lz
