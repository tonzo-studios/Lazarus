#include <lazarus/ECS/Entity.h>

using namespace lz;

size_t Entity::entityCount = 0;

Entity::Entity()
    : entityId(++entityCount)
{
}