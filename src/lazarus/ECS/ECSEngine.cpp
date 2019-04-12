#include <lazarus/ECS/ECSEngine.h>

using namespace lz;

Entity* ECSEngine::addEntity()
{
    Entity entity;
    std::shared_ptr<Entity> entPtr = std::make_shared<Entity>(entity);
    entities[entity.getId()] = entPtr;
    return entPtr.get();
}

void ECSEngine::addEntity(Entity& entity)
{
    entities[entity.getId()] = std::make_shared<Entity>(entity);
}

Entity* ECSEngine::getEntity(Identifier entityId)
{
    auto found = entities.find(entityId);
    if (found == entities.end())
        return nullptr;
    return found->second.get();
}

void ECSEngine::addUpdateable(Updateable* updateable)
{
    updateables.emplace_back(updateable);
}

void ECSEngine::update()
{
    for (auto updateable : updateables)
    {
        updateable.get()->update(*this);
    }
}