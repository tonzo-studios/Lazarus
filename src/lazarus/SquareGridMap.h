#pragma once

#include <lazarus/Map.h>

namespace lz
{
struct Position
{
    Position(int x, int y);

    bool operator<(const Position& other) const;

    bool operator==(const Position& other) const;

    bool operator!=(const Position& other) const;

    int x, y;
};

class SquareGridMap
{
public:
    SquareGridMap() = default;

    SquareGridMap(bool diagonals);

    bool isWalkable(const Position& pos);

    bool isTransparent(const Position& pos);

    float getCost(const Position& pos);

    void setAt(const Position& pos, bool walkable, bool transparent=false);

    void setAt(const Position& pos, float cost, bool transparent=false);

    virtual std::vector<Position> neighbours(const Position& position) const;

    void carveRoom(const Position& topLeft,
                   const Position& bottomRight,
                   float cost=1);

private:
    bool diagonals = false;
};

}  // namespace lz
