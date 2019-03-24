#include "catch/catch.hpp"

#include <vector>

#include <lazarus/Random.h>

#define TEST_SEED 12345

using namespace lz;

TEST_CASE("Int range sequence", "[random]")
{
    Random::seed(TEST_SEED);
    // The first 10 numbers generated with range(0, 3)
    // with the given seed should coincide with these
    std::vector<int> sequence{3, 3, 1, 0, 0, 0, 0, 3, 2, 2};
    for (int i = 0; i < sequence.size(); ++i)
        REQUIRE(Random::range(0, 3) == sequence[i]);
}

TEST_CASE("oneIn sequence", "[random]")
{
    Random::seed(TEST_SEED);
    // With this seed, only the 6th roll of oneIn(10) returns true
    for (int i = 0; i < 10; ++i)
    {
        if (i == 5)
            REQUIRE(Random::oneIn(10));
        else
            REQUIRE(!Random::oneIn(10));
    }
}

TEST_CASE("roll sequence", "[random]")
{
    Random::seed(TEST_SEED);
    REQUIRE(Random::roll(6, 2) == 12);
    REQUIRE(Random::roll(2, 5) == 5);
    REQUIRE(Random::roll(20, 1) == 17);
    REQUIRE(Random::roll(0, 0) == 0);
    REQUIRE(Random::roll(1, 0) == 0);
    REQUIRE(Random::roll(0, 5) == 0);

}