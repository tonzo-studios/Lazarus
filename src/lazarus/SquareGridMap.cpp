#include <lazarus/SquareGridMap.h>

#include <array>

using namespace lz;

Position2D::Position2D(int x, int y)
    : x(x)
    , y(y)
{
}

bool Position2D::operator<(const Position2D& other) const
{
    if (y == other.y)
        return x < other.x;
    return y < other.y;
}

bool Position2D::operator==(const Position2D& other) const
{
    return x == other.x && y == other.y;
}

bool Position2D::operator!=(const Position2D& other) const
{
    return !(*this == other);
}

SquareGridMap::SquareGridMap(bool diagonals)
    : diagonals(diagonals)
{
}

std::vector<Position2D> SquareGridMap::neighbours(const Position2D& position) const
{
    std::vector<Position2D> result;
    int x, y;
    x = position.x;
    y = position.y;
    std::array<Position2D, 4> neighbourPositions{
        Position2D(x - 1, y),
        Position2D(x + 1, y),
        Position2D(x, y - 1),
        Position2D(x, y + 1)
    };

    for (Position2D neighbour : neighbourPositions)
    {
        if (isWalkable(neighbour))
            result.push_back(neighbour);
    }

    if (diagonals)
    {
        std::array<Position2D, 4> diagonalPositions{
            Position2D(x - 1, y - 1),
            Position2D(x + 1, y + 1),
            Position2D(x + 1, y - 1),
            Position2D(x - 1, y + 1)};

        for (Position2D neighbour : diagonalPositions)
        {
            if (isWalkable(neighbour))
                result.push_back(neighbour);
        }
    }
    
    return result;
}

void SquareGridMap::carveRoom(const Position2D& topLeft,
                              const Position2D& bottomRight,
                              float cost)
{
    for (int x = topLeft.x; x < bottomRight.x; ++x)
    {
        for (int y = topLeft.y; y < bottomRight.y; ++y)
        {
            setAt(Position2D(x, y), cost, true);
        }
    }
}