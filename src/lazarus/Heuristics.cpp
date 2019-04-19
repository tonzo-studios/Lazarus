#include <lazarus/Heuristics.h>

using namespace lz;

double manhattanDistance(const Position2D& a, const Position2D& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

double euclideanDistance(const Position2D& a, const Position2D& b)
{
    int dx = a.x - b.x,
        dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

double chebyshevDistance(const Position2D& a, const Position2D& b)
{
    return std::max(std::abs(a.x - b.x), std::abs(a.y - b.y));
}

double octileDistance(const Position2D& a, const Position2D& b)
{
    double dx = std::abs(a.x - b.x);
    double dy = std::abs(a.y - b.y);
    return std::sqrt(2) * std::min(dx, dy) + std::abs(dx - dy);
}
