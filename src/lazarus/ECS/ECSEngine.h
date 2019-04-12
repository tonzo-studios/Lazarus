#pragma once

#include <functional>

#include <lazarus/ECS/Entity.h>
#include <lazarus/ECS/EventListener.h>

namespace lz
{
class BaseSystem;

/**
 * Main driver to work with entities, components and systems.
 * 
 * The ECSEngine class manages and puts together all the pieces of the ECS model.
 * It holds the entities and systems, and acts as a controller for the creation,
 * deletion and modification of these objects.
 * 
 * It also acts as an observer for the classes that implement the EventListener
 * interface, and is able to emit events of any type to these listeners, allowing
 * communication between systems.
 * 
 * @see Entity
 * @see BaseSystem
 * @see EventListener
 */
class ECSEngine
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
    Entity* getEntity(Identifier entityId);

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

    template <typename EventType>
    void subscribe(EventListener<EventType>* eventListener);

    template <typename EventType>
    void unsubscribe(EventListener<EventType>* eventListener);

private:
    template <typename EventType>
    std::vector<EventListener<EventType>*>& getSubscribed();

private:
    std::unordered_map<Identifier, std::shared_ptr<Entity>> entities;
    // Maps system id -> system
    std::unordered_map<Identifier, std::shared_ptr<BaseSystem>> systems;
    // Maps event type index -> list of event listeners for that event type
    std::unordered_map<std::type_index, std::vector<__lz::BaseEventListener*>> subscribers;
};

template <typename... Types>
std::vector<Entity*> ECSEngine::entitiesWithComponents(bool includeDeleted)
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
void ECSEngine::applyToEach(
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

template <typename EventType>
void ECSEngine::subscribe(EventListener<EventType>* eventListener)
{
    std::type_index typeId = __lz::getTypeIndex<EventType>();
    auto found = subscribers.find(typeId);
    if (found == subscribers.end())
    {
        // No subscribers to this type of event yet, create vector
        std::vector<EventListener<EventType>*> vec;
        vec.push_back(eventListener);
        subscribers[typeId] = vec;
    }
    else
    {
        // There already exists a list of subscribers to this event type
        found->second.push_back(eventListener);
    }
}

template <typename EventType>
void ECSEngine::unsubscribe(EventListener<EventType>* eventListener)
{
    std::vector<EventListener<EventType>*>& eventSubscribers = getSubscribed<EventType>();
    for (auto it = eventSubscribers.begin(); it != eventSubscribers.end(); ++it)
    {
        if (*it == system)
        {
            // System found, remove it from the subscriber list
            eventSubscribers.erase(it);
            return;
        }
    }
    // System was not found
    throw __lz::LazarusException("ECS engine was not subscribed to the given event");
}


template <typename EventType>
std::vector<EventListener<EventType>*>& ECSEngine::getSubscribed()
{
    auto found = subscribers.find(__lz::getTypeIndex<EventType>());
    if (found == subscribers.end())
    {
        // No subscribers to this type of event yet, create vector
        std::vector<BaseSystem*> vec;
        subscribers[__lz::getTypeIndex<EventType>()] = vec;
        return vec;
    }
    return found->second;
}

}  // namespace lz
