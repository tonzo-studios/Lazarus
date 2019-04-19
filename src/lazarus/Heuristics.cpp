#include <lazarus/Heuristics.h>

using namespace lz;

float manhattanDistance(const Position2D& a, const Position2D& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

float euclideanDistance(const Position2D& a, const Position2D& b)
{
    int dx = a.x - b.x,
        dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

float chebyshevDistance(const Position2D& a, const Position2D& b)
{
    return std::max(std::abs(a.x - b.x), std::abs(a.y - b.y));
}

float octileDistance(const Position2D& a, const Position2D& b)
{
    float dx = std::abs(a.x - b.x);
    float dy = std::abs(a.y - b.y);
    return std::sqrt(2) * std::min(dx, dy) + std::abs(dx - dy);
}
