#include "catch/catch.hpp"

#include <cmath>
#include <vector>

#include <lazarus/Random.h>

#define TEST_SEED 12345

const double TOLERANCE = 1e-5;

// Checks whether a and b are closer than a certain tolerance
bool equalFloat(double a, double b)
{
    return std::abs(a - b) < TOLERANCE;
}

using namespace lz;

TEST_CASE("int range generation", "[random]")
{
    Random::seed(TEST_SEED);
    SECTION("generation with fixed interval")
    { // The first 10 numbers generated with range(0, 3)
        // with the given seed should coincide with these
        std::vector<int> sequence{3, 3, 1, 0, 0, 0, 0, 3, 2, 2};
        for (int i = 0; i < sequence.size(); ++i)
            REQUIRE(Random::range(0, 3) == sequence[i]);
    }
    SECTION("varying intervals")
    {
        REQUIRE(Random::range(0, 2) == 2);
        REQUIRE(Random::range(5, 10) == 10);
        REQUIRE(Random::range(-1, 1) == -1);
        REQUIRE(Random::range(0, 0) == 0);
        REQUIRE(Random::range(2, -5) == -4);
    }
}

TEST_CASE("oneIn test", "[random]")
{
    Random::seed(TEST_SEED);
    SECTION("sequence with same probability")
    { // With this seed, only the 6th roll of oneIn(10) returns true
        for (int i = 0; i < 10; ++i)
        {
            if (i == 5)
                REQUIRE(Random::oneIn(10));
            else
                REQUIRE(!Random::oneIn(10));
        }
    }
    SECTION("different probabilities")
    {
        REQUIRE(!Random::oneIn(5));
        // -1 gets casted to a huge integer since the parameter is unsigned and should
        // therefore be false almost always
        REQUIRE(!Random::oneIn(-1));
        // oneIn(0) is true by definition
        REQUIRE(Random::oneIn(0));
    }
    SECTION("oneIn(1) is always true")
    {
        // oneIn(1) should be true always
        for (int i = 0; i < 15; ++i)
            REQUIRE(Random::oneIn(1) == 1);
    }
}

TEST_CASE("roll test", "[random]")
{
    Random::seed(TEST_SEED);
    REQUIRE(Random::roll(6, 2) == 12);
    REQUIRE(Random::roll(2, 5) == 5);
    REQUIRE(Random::roll(20, 1) == 17);
    // Test base cases
    REQUIRE(Random::roll(0, 0) == 0);
    REQUIRE(Random::roll(1, 0) == 0);
    REQUIRE(Random::roll(0, 5) == 0);
}

TEST_CASE("normal distribution", "[random]")
{
    Random::seed(TEST_SEED);
    REQUIRE(equalFloat(Random::normal(0, 2), 1.1901823323));
    REQUIRE(equalFloat(Random::normal(5, 10), 13.00446708));
    REQUIRE(equalFloat(Random::normal(-1, 0.5), -1.52153444));
    REQUIRE(equalFloat(Random::normal(0, 0), 0));
    REQUIRE(equalFloat(Random::normal(0, -1), -0.488176471));
}