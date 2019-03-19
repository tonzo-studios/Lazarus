#include <lazarus/ECS/Entity.h>

size_t lz::Entity::entityCount = 0;

lz::Entity::Entity()
    : entityId(++entityCount)
{
}