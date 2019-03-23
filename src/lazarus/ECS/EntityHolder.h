#pragma once

#include <functional>
#include <type_traits>

#include <lazarus/ECS/Entity.h>

namespace lz
{
class EntityHolder
{
public:
    /**
     * Adds a new entity to the collection and returns a pointer to it.
     */
    Entity* addEntity();

    /**
     * Adds an existing entity to the collection.
     * 
     * If the entity already exists in the collection, that is, the ID of
     * the entity is found in the collection, it does nothing.
     */
    void addEntity(Entity& entity);

    /**
     * Gets a pointer to the entity from the collection with the given
     * ID, or a nullptr if an entity with such ID does not exist in the
     * collection.
     */
    Entity* getEntity(size_t entityId);

    /**
     * Returns a vector with the entities that have the specified
     * components.
     */
    template <typename... Types>
    std::vector<Entity*> entitiesWithComponents(bool includeDeleted=false);

    /**
     * Applies a function to each of the entities from the collection that have the
     * specified component types.
     * 
     * The function passed can be a reference to an existing function, a lambda,
     * or an std::function.
     */
    template <typename... Types>
    void applyToEach(
        typename std::common_type<std::function<void(Entity*, Types*...)>>::type&& func,
        bool includeDeleted=false);

private:
    std::unordered_map<size_t, std::shared_ptr<Entity>> entities;
};

template <typename... Types>
std::vector<Entity*> EntityHolder::entitiesWithComponents(bool includeDeleted)
{
    std::vector<Entity*> result;
    applyToEach<Types...>([&](Entity* ent, Types*... comp)
    {
        result.push_back(ent);
    },
    includeDeleted);
    return result;
}

template <typename... Types>
void EntityHolder::applyToEach(
    typename std::common_type<std::function<void(Entity*, Types*...)>>::type&& func,
    bool includeDeleted)
{
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        Entity* entity = it->second.get();

        if (includeDeleted && entity->isDeleted())
            continue;

        if (entity->has<Types...>())
            func(entity, entity->get<Types>()...);
    }
}
}  // namespace lz
