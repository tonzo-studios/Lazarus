#include <random>

class Random
{
public:
    /**
     * Sets a new seed for the random generator using a hardware random device if
     * available, or a seed using the current time.
     */
    static void Seed();

    /**
    * Return a random integer between the two given numbers, both ends included.
    * 
    * If a < b, then it will return an integer in [a, b].
    * Otherwise, it will be between [b, a].
    */
    static long rng(long a, long b);

private:
    static std::mt19937 m_generator;
};
