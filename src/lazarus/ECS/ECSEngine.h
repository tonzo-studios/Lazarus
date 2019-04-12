#pragma once

#include <lazarus/ECS/EntityHolder.h>
#include <lazarus/ECS/EventListener.h>

namespace lz
{
class BaseSystem;

class ECSEngine
{
public:
    Entity* addEntity();

    void addEntity(Entity& entity);

    template <typename EventType>
    void subscribe(EventListener<EventType>* eventListener);

    template <typename EventType>
    void unsubscribe(EventListener<EventType>* eventListener);

private:
    template <typename EventType>
    std::vector<EventListener<EventType>*>& getSubscribed();

private:
    EntityHolder entities;
    // Maps system id -> system
    std::unordered_map<Identifier, std::shared_ptr<BaseSystem>> systems;
    // Maps event type index -> list of event listeners for that event type
    std::unordered_map<std::type_index, std::vector<__lz::BaseEventListener*>> subscribers;
};

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
