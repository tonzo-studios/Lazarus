#pragma once

#include <lazarus/ECS/EntityHolder.h>

namespace lz
{
class BaseSystem;

class ECSEngine
{
public:
    Entity* addEntity();

    void addEntity(Entity& entity);

    template <typename EventType>
    void subscribe(BaseSystem* system);

    template <typename EventType>
    void unsubscribe(BaseSystem* system);

    template <typename EventType>
    std::vector<BaseSystem*>& getSubscribed();

private:
    EntityHolder entities;
    // Maps system id -> system
    std::unordered_map<Identifier, std::shared_ptr<BaseSystem>> systems;
    // Maps event type index -> list of systems subscribed to that event type
    std::unordered_map<std::type_index, std::vector<BaseSystem*>> subscribers;
};

template <typename EventType>
void ECSEngine::subscribe(BaseSystem* system)
{
    std::type_index typeId = __lz::getTypeIndex<EventType>();
    auto found = subscribers.find(typeId);
    if (found == subscribers.end())
    {
        // No subscribers to this type of event yet, create vector
        std::vector<BaseSystem*> vec;
        vec.push_back(system);
        subscribers[typeId] = vec;
    }
    else
    {
        // There already exists a list of subscribers to this event type
        found->second.push_back(system);
    }
}

template <typename EventType>
void ECSEngine::unsubscribe(BaseSystem* system)
{
    std::vector<BaseSystem*>& eventSubscribers = getSubscribed<EventType>();
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
    throw __lz::LazarusException("System was not subscribed to the given event");
}


template <typename EventType>
std::vector<BaseSystem*>& ECSEngine::getSubscribed()
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
