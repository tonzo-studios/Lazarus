#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <lazarus/common.h>

namespace __lz  // Only meant for internal use
{
template <typename T>
std::type_index getTypeIndex()
{
    return std::type_index(typeid(T));
}
}

namespace lz
{
/**
 * The class all components must derive from.
 * 
 * A component is just a collection of data, i.e., it has no methods.
 * In the ECS pattern, the logic that uses this data is in the Systems, which act on
 * the entities holding these components.
 */
struct BaseComponent
{
    // Add virtual destructor to make class polymorphic
    virtual ~BaseComponent() = default;

    /**
     * ID of the entity that holds this component.
     * 
     * It is set upon calling Entity::addComponent.
     */
    size_t entityId;
};

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
    size_t getId() const { return entityId; }

    /**
     * Returns whether the entity has a component of type T.
     */
    template <typename T>
    bool has() const;

    /**
     * Returns whether the entity has all the components of the given types.
     */
    template <typename T, typename V, typename... Types>
    bool has() const;

    /**
     * Attaches a component to the entity.
     * 
     * The type specified must be a type derived from BaseComponent.
     * The component will be constructed with the passed arguments and attached to
     * the entity's pool of components.
     * 
     * If the entity already has a component of the specified type, an exception
     * will be thrown.
     */
    template <typename T, typename... Args>
    void addComponent(Args&&... args);

    /**
     * Removes the component of type T from the entity.
     * 
     * If the entity does not have a component of the specified type, an exception
     * is thrown.
     */
    template <typename T>
    void removeComponent();

    /**
     * Returns a pointer to the entity's component of the specified type.
     * 
     * If the entity does not hold a component of that type, a nullptr will be returned.
     */
    template <typename T>
    T* get();

    /**
     * Returns whether this entity is marked for deletion upon the next pass of
     * the garbage collector.
     */
    bool isDeleted() const { return deleted; }

private:
    const size_t entityId;
    static size_t entityCount;  // Keep track of the number of entities to assign new IDs
    std::unordered_map<std::type_index, std::shared_ptr<BaseComponent>> components;
    bool deleted = false;
};

template <typename T>
bool Entity::has() const
{
    return components.find(__lz::getTypeIndex<T>()) != components.end();
}

template <typename T, typename V, typename... Types>
bool Entity::has() const
{
    return has<T>() && has<V, Types...>();
}

template <typename T, typename... Args>
void Entity::addComponent(Args&&... args)
{
    // Make sure it is derived from BaseComponent
    static_assert(std::is_base_of<BaseComponent, T>::value, "T must be derived of BaseComponent");

    // Check if the entity already holds a component T
    if (has<T>())
        throw new __lz::LazarusException("The entity already holds a component of the same type");

    // Construct component and add it to the map
    auto component = std::make_shared<T>(args...);
    component.get()->entityId = getId();
    components[__lz::getTypeIndex<T>()] = component;
}

template <typename T>
void Entity::removeComponent()
{
    if (!has<T>())
        throw new __lz::LazarusException("The entity does not have a component of the specified type");

    components.erase(__lz::getTypeIndex<T>());
}

template <typename T>
T* Entity::get()
{
    auto found = components.find(__lz::getTypeIndex<T>());
    if (found == components.end())
        return nullptr;
    return dynamic_cast<T*>(found->second.get());
}
}  // namespace lz