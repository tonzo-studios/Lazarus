#pragma once

#include <memory>
#include <typeindex>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <lazarus/common.h>

namespace __lz  // Meant for internal use only
{
template <typename T>
std::type_index getTypeIndex()
{
    return std::type_index(typeid(T));
}

class BaseComponentHandle
{
public:
    virtual ~BaseComponentHandle() = default;
};

template <typename Component>
class ComponentHandle : public BaseComponentHandle
{
public:
    ComponentHandle(std::shared_ptr<Component> comp)
        : component(std::move(comp))
    {
    }

    Component *get() { return component.get(); }

private:
    std::shared_ptr<Component> component;
};
}

namespace lz
{
using Identifier = size_t;

/**
 * An Entity is a collection of components with a unique ID.
 * 
 * An Entity can only have one component of each type at the same time.
 */
class Entity
{
public:
    /**
     * Default constructor.
     * 
     * Sets the ID of the entity to the next available ID.
     */
    Entity();

    /**
     * Returns the ID of the entity.
     */
    Identifier getId() const { return entityId; }

    /**
     * Returns whether the entity has a component of type T.
     */
    template <typename Component>
    bool has() const;

    /**
     * Returns whether the entity has all the components of the given types.
     */
    template <typename T, typename V, typename... Types>
    bool has() const;

    /**
     * Attaches a component to the entity.
     * 
     * The component will be constructed with the arguments passed and attached to
     * the entity's pool of components. Therefore, the component must have a
     * constructor that matches the arguments passed.
     * 
     * If the entity already has a component of the specified type, an exception
     * will be thrown.
     */
    template <typename Component, typename... Args>
    void addComponent(Args&&... args);

    /**
     * Removes the component of type T from the entity.
     * 
     * If the entity does not have a component of the specified type, an exception
     * is thrown.
     */
    template <typename Component>
    void removeComponent();

    /**
     * Returns a pointer to the entity's component of the specified type.
     * 
     * If the entity does not hold a component of that type, a nullptr will be returned.
     */
    template <typename Component>
    Component* get();

    /**
     * Returns whether this entity is marked for deletion upon the next pass of
     * the garbage collector.
     */
    bool isDeleted() const { return deleted; }

    /**
     * Marks the entity for deletion.
     * 
     * An entity marked for deletion will be cleared from memory on the next pass of the
     * ECS engine garbage collector.
     */
    void markForDeletion() { deleted = true; }

    /**
     * Returns true if the IDs of the entities are the same.
     */
    bool operator==(const Entity& other);

    /**
     * Returns true if the IDs of the entities are different.
     */
    bool operator!=(const Entity& other);
    
    /**
     * Returns true if the ID of this entity is smaller than the other.
     */
    bool operator<(const Entity& other);

private:
    const Identifier entityId;
    static Identifier entityCount;  // Keep track of the number of entities to assign new IDs
    std::unordered_map<std::type_index, std::shared_ptr<__lz::BaseComponentHandle>> components;
    bool deleted = false;
};

template <typename Component>
bool Entity::has() const
{
    return components.find(__lz::getTypeIndex<Component>()) != components.end();
}

template <typename T, typename V, typename... Types>
bool Entity::has() const
{
    return has<T>() && has<V, Types...>();
}

template <typename Component, typename... Args>
void Entity::addComponent(Args&&... args)
{
    // Check if the entity already holds a component T
    if (has<Component>())
    {
        std::stringstream msg;
        msg << "Entity " << getId() << " already holds a component of type "
            << typeid(Component).name();
        throw __lz::LazarusException(msg.str());
    }

    // Construct component and add it to the map
    std::shared_ptr<__lz::BaseComponentHandle> handle(
        new __lz::ComponentHandle<Component>(std::make_shared<Component>(args...))
    );
    components[__lz::getTypeIndex<Component>()] = std::move(handle);
}

template <typename Component>
void Entity::removeComponent()
{
    if (!has<Component>())
    {
        std::stringstream msg;
        msg << "Entity " << getId() << " does not have a component of type "
            << typeid(Component).name();
        throw __lz::LazarusException(msg.str());
    }

    components.erase(__lz::getTypeIndex<Component>());
}

template <typename Component>
Component* Entity::get()
{
    auto found = components.find(__lz::getTypeIndex<Component>());
    if (found == components.end())
        return nullptr;  // TODO: Log this case
    auto compHandle = dynamic_cast<__lz::ComponentHandle<Component>*>(found->second.get());
    return compHandle->get();
}
}  // namespace lz
