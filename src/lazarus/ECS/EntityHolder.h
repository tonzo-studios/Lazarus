#pragma once

#include <lazarus/ECS/Entity.h>

namespace lz
{
class EntityHolder
{
public:
    // TODO: Methods to iterate over entities

private:
    std::vector<std::shared_ptr<Entity>> entityList;
};
}