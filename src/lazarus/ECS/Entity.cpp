#include <lazarus/ECS/Entity.h>

using namespace lz;

Identifier Entity::entityCount = 0;

Entity::Entity()
    : entityId(++entityCount)
{
}

bool Entity::operator==(const Entity& other)
{
    return getId() == other.getId();
}

bool Entity::operator!=(const Entity& other)
{
    return !(*this == other);
}

bool Entity::operator<(const Entity& other)
{
    return getId() < other.getId();
}
