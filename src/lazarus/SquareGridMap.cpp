#include <lazarus/SquareGridMap.h>

#include <array>

using namespace lz;

Position::Position(int x, int y)
    : x(x)
    , y(y)
{
}

bool Position::operator<(const Position& other) const
{
    if (y == other.y)
        return x < other.x;
    return y < other.y;
}

bool Position::operator==(const Position& other) const
{
    return x == other.x && y == other.y;
}

bool Position::operator!=(const Position& other) const
{
    return !(*this == other);
}

SquareGridMap::SquareGridMap(bool diagonals)
    : diagonals(diagonals)
{
}

std::vector<Position> SquareGridMap::neighbours(const Position& position) const
{
    std::vector<Position> result;
    int x, y;
    x = position.x;
    y = position.y;
    std::array<Position, 4> neighbourPositions{
        Position(x - 1, y),
        Position(x + 1, y),
        Position(x, y - 1),
        Position(x, y + 1)
    };

    for (Position neighbour : neighbourPositions)
    {
        if (isWalkable(neighbour))
            result.push_back(neighbour);
    }

    if (diagonals)
    {
        std::array<Position, 4> diagonalPositions{
            Position(x - 1, y - 1),
            Position(x + 1, y + 1),
            Position(x + 1, y - 1),
            Position(x - 1, y + 1)};

        for (Position neighbour : diagonalPositions)
        {
            if (isWalkable(neighbour))
                result.push_back(neighbour);
        }
    }
    
    return result;
}

void SquareGridMap::carveRoom(const Position& topLeft,
                              const Position& bottomRight,
                              float cost)
{
    for (int x = topLeft.x; x < bottomRight.x; ++x)
    {
        for (int y = topLeft.y; y < bottomRight.y; ++y)
        {
            setAt(Position(x, y), cost, true);
        }
    }
}