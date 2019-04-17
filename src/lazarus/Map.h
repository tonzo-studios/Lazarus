#pragma once

#include <limits>
#include <map>
#include <vector>

#include <lazarus/common.h>

namespace lz
{
template <typename Cell, typename CostType = float>
class Map
{
public:
    virtual bool isWalkable(const Cell& cell) const
    {
        return costMap.find(cell) != costMap.end();
    }

    virtual bool isTransparent(const Cell& cell) const
    {
        return transparencyMap.find(cell) != transparencyMap.end();
    }

    virtual CostType getCost(const Cell& cell) const
    {
        auto found = costMap.find(cell);
        if (found == costMap.end())
            throw __lz::LazarusException("Cell is not walkable.");
        return found->second;
    }

    virtual std::vector<Cell> neighbours(const Cell& cell) const = 0;

    virtual void setAt(const Cell& cell, CostType cost, bool transparent)
    {
        costMap[cell] = cost;
        transparencyMap[cell] = transparent;
    }

protected:
    std::map<Cell, CostType> costMap;
    std::map<Cell, bool> transparencyMap;
};
}  // namespace lz
