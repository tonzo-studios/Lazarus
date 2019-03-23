#include <lazarus/Random.h>

#include <algorithm>
#include <chrono>
#include <random>

std::mt19937 lz::Random::generator;  // Initialize with default seed (5489u)

void lz::Random::seed()
{
    try
    {
        // Use a random device if available
        std::random_device randomDevice;
        generator.seed(randomDevice());
    }
    catch (const std::exception &e)
    {
        // Random device not available, use a time seed
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }
}

long lz::Random::range(long a, long b)
{
    if (a > b)
        std::swap(a, b);

    std::uniform_int_distribution<long> dist(a, b);
    return dist(generator);
}

ulong lz::Random::roll(unsigned sides, unsigned times)
{
    if (sides < 2 || times < 1)
        return times;
    std::uniform_int_distribution<unsigned> dist(1, sides);
    ulong total = 0;
    for (unsigned t = 0; t < times; ++t)
        total += dist(generator);
    return total;
}

bool lz::Random::oneIn(unsigned n)
{
    if (n < 2)
        return true;
    return lz::Random::range(1, n) == 1;
}