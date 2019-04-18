#include <lazarus/SquareGridMap.h>

#include <array>
#include <sstream>

#include <lazarus/common.h>

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

SquareGridMap::SquareGridMap(unsigned width, unsigned height, bool diagonals)
    : diagonals(diagonals)
    , width(width)
    , height(height)
    , costs(height, std::vector<float>(width, -1.))
    , transparencies(height, std::vector<bool>(width, false))
{
}

unsigned SquareGridMap::getWidth() const
{
    return width;
}

unsigned SquareGridMap::getHeight() const
{
    return height;
}

bool SquareGridMap::isOutOfBounds(const Position2D& pos) const
{
    return pos.x < 0 || pos.y < 0 || pos.x >= width || pos.y >= height;
}

bool SquareGridMap::isWalkable(const Position2D& pos) const
{
    if (isOutOfBounds(pos))
        return false;
    return costs[pos.y][pos.x] < 0;
}

bool SquareGridMap::isTransparent(const Position2D& pos) const
{
    if (isOutOfBounds(pos))
        return false;
    return transparencies[pos.y][pos.x];
}

std::vector<Position2D> SquareGridMap::neighbours(const Position2D& pos) const
{
    std::vector<Position2D> result;
    int x = pos.x, y = pos.y;

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
            Position2D(x - 1, y + 1)
        };

        for (Position2D neighbour : diagonalPositions)
        {
            if (isWalkable(neighbour))
                result.push_back(neighbour);
        }
    }
    
    return result;
}

void SquareGridMap::setCost(const Position2D& pos, float cost)
{
    if (isOutOfBounds(pos))
    {
        std::stringstream msg;
        msg << "Position (" << pos.x << ", " << pos.y << ") is out of bounds.";
        throw __lz::LazarusException(msg.str());
    }

    costs[pos.y][pos.x] = cost;
}

void SquareGridMap::setUnwalkable(const Position2D& pos)
{
    setCost(pos, -1.);
}

void SquareGridMap::setTransparency(const Position2D& pos, bool transparent)
{
    if (isOutOfBounds(pos))
    {
        std::stringstream msg;
        msg << "Position (" << pos.x << ", " << pos.y << ") is out of bounds.";
        throw __lz::LazarusException(msg.str());
    }

    transparencies[pos.y][pos.x] = transparent;
}

void SquareGridMap::carveRoom(const Position2D& topLeft,
                              const Position2D& bottomRight,
                              float cost)
{
    for (int x = topLeft.x; x < bottomRight.x; ++x)
    {
        for (int y = topLeft.y; y < bottomRight.y; ++y)
        {
            Position2D pos(x, y);
            setCost(pos, cost);
            setTransparency(pos, true);
        }
    }
}