#include <lazarus/ECS/EntityHolder.h>

using namespace lz;

Entity* EntityHolder::addEntity()
{
    Entity entity;
    std::shared_ptr<Entity> entPtr = std::make_shared<Entity>(entity);
    entities[entity.getId()] = entPtr;
    return entPtr.get();
}

void EntityHolder::addEntity(Entity& entity)
{
    entities[entity.getId()] = std::make_shared<Entity>(entity);
}

Entity* EntityHolder::getEntity(Identifier entityId)
{
    auto found = entities.find(entityId);
    if (found == entities.end())
        return nullptr;
    return found->second.get();
}