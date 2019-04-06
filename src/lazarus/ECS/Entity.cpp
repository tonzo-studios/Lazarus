#include <lazarus/ECS/Entity.h>

using namespace lz;

Identifier Entity::entityCount = 0;

Entity::Entity()
    : entityId(++entityCount)
{
}