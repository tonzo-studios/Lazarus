#pragma once

#include <lazarus/ECS/ECSEngine.h>

namespace lz
{
/**
 * Base system class that user-defined systems must derive from.
 * 
 * Each system will hold a reference to the engine it lives in.
 * Systems have a unique ID which is set on creation.
 * 
 * @see ECSEngine
 */
class BaseSystem
{
public:
    /**
     * Construct a system that will live in the given engine.
     */
    BaseSystem(ECSEngine& engine);

    /**
     * Returns the ID of the system.
     */
    Identifier getId() const { return id; }

    /**
     * Virtual update method.
     * 
     * Systems deriving from this class can define this method to do
     * something on the entities passed as argument.
     * The ECSEngine will call the update method on all systems it has when
     * the update method from the ECSEngine is called.
     */
    virtual void update(ECSEngine& engine)
    {
    }

    /**
     * Subscribes this system to the events of type EventType.
     */
    template <typename EventType>
    void subscribe();

    /**
     * Unsubscribes this system to the events of type EventType.
     */
    template <typename EventType>
    void unsubscribe();

    /**
     * Returns true if the IDs of the systems are the same.
     */
    bool operator==(const BaseSystem& other);
    
    /**
     * Returns true if the IDs of the systems are different.
     */
    bool operator!=(const BaseSystem& other);
    
    /**
     * Returns true if the ID of this system is smaller than the other.
     */
    bool operator<(const BaseSystem& other);

private:
    Identifier id;
    static Identifier systemCount;
    ECSEngine& engine;
};

template <typename EventType>
void BaseSystem::subscribe()
{
    engine.subscribe<EventType>(this);
}
}  // namespace lz
