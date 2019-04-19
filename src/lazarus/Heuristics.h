#pragma once

#include <cmath>

#include <lazarus/SquareGridMap.h>

namespace lz
{
/**
 * Compute the Manhattan distance between two 2D positions.
 */
double manhattanDistance(const Position2D&, const Position2D&);

/**
 * Compute the Euclidean distance between two 2D positions.
 */
double euclideanDistance(const Position2D&, const Position2D&);

/**
 * Compute the Chebyshev distance between two 2D positions.
 */
double chebyshevDistance(const Position2D&, const Position2D&);

/**
 * Compute the Chebyshev distance between two 2D positions.
 */
double octileDistance(const Position2D&, const Position2D&);
}  // namespace lz