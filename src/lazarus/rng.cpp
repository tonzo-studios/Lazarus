#include <lazarus/rng.h>

#include <algorithm>
#include <chrono>
#include <random>

std::mt19937 Random::m_generator;  // Initialize with default seed (5489u)

void Random::Seed()
{
    try
    {
        // Use a random device if available
        std::random_device randomDevice;
        m_generator.seed(randomDevice());
    }
    catch(const std::exception& e)
    {
        // Random device not available
        // Use a time seed
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        m_generator.seed(seed);
    }
}

long Random::rng(long a, long b)
{
    if (a > b)
        std::swap(a, b);

    std::uniform_int_distribution<long> dist(a, b);
    return dist(m_generator);
}
