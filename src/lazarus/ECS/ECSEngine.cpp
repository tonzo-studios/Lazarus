#include <lazarus/ECS/ECSEngine.h>

using namespace lz;

Entity* ECSEngine::addEntity()
{
    return entities.addEntity();
}

void ECSEngine::addEntity(Entity& entity)
{
    entities.addEntity(entity);
}