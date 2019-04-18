#pragma once

#include <vector>

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
    SquareGridMap(unsigned width, unsigned height, bool diagonals=false);

    unsigned getWidth() const;
    
    unsigned getHeight() const;

    bool isWalkable(const Position2D& pos) const;

    bool isTransparent(const Position2D& pos) const;

    bool isOutOfBounds(const Position2D& pos) const;

    float getCost(const Position2D& pos) const;

    void setCost(const Position2D& pos, float cost);

    void setUnwalkable(const Position2D& pos);

    void setTransparency(const Position2D& pos, bool transparent);

    virtual std::vector<Position2D> neighbours(const Position2D& position) const;

    void carveRoom(const Position2D& topLeft,
                   const Position2D& bottomRight,
                   float cost=1);

private:
    bool diagonals = false;
    unsigned width, height;
    std::vector<std::vector<float>> costs;
    std::vector<std::vector<bool>> transparencies;
};
}  // namespace lz
