#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

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
struct BaseComponent
{
    // Add virtual destructor to make class polymorphic
    virtual ~BaseComponent() = default;

    size_t entityId;
};

class Entity
{
public:
    Entity();

    size_t getId() const { return entityId; }

    template <typename T>
    bool has() const;

    template <typename T, typename V, typename... Types>
    bool has() const;

    template <typename T, typename... Args>
    void addComponent(Args&&... args);

    template <typename T>
    T* get();

private:
    const size_t entityId;
    static size_t entityCount;
    std::unordered_map<std::type_index, std::shared_ptr<BaseComponent>> components;
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
    static_assert(std::is_base_of<BaseComponent, T>::value);

    // Check if the entity already holds a component T
    if (has<T>())
    {
        // TODO: Make own exception class
        throw new std::runtime_error("The entity already holds a component of the same type");
    }

    // Construct component and add it to the map
    components[__lz::getTypeIndex<T>()] = std::make_shared<T>(args...);
}

template <typename T>
T* Entity::get()
{
    auto found = components.find(__lz::getTypeIndex<T>());
    if (found == components.end())
        return nullptr;
    return dynamic_cast<T*>(found->second.get());
}
}