#pragma once

#include <lazarus/Map.h>

namespace lz
{
struct Position2D
{
    Position2D(int x, int y);

    bool operator<(const Position2D& other) const;

    bool operator==(const Position2D& other) const;

    bool operator!=(const Position2D& other) const;

    int x, y;
};

class SquareGridMap
{
public:
    SquareGridMap() = default;

    SquareGridMap(bool diagonals);

    bool isWalkable(const Position2D& pos);

    bool isTransparent(const Position2D& pos);

    float getCost(const Position2D& pos);

    void setAt(const Position2D& pos, bool walkable, bool transparent=false);

    void setAt(const Position2D& pos, float cost, bool transparent=false);

    virtual std::vector<Position2D> neighbours(const Position2D& position) const;

    void carveRoom(const Position2D& topLeft,
                   const Position2D& bottomRight,
                   float cost=1);

private:
    bool diagonals = false;
};

}  // namespace lz
