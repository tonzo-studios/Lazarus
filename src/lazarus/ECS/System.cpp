#include <lazarus/ECS/System.h>

using namespace lz;

Identifier BaseSystem::systemCount = 0;

BaseSystem::BaseSystem(ECSEngine& engine)
    : id(++systemCount)
    , engine(engine)
{
}

bool BaseSystem::operator==(const BaseSystem& other)
{
    return getId() == other.getId();
}

bool BaseSystem::operator!=(const BaseSystem& other)
{
    return !(*this == other);
}

bool BaseSystem::operator<(const BaseSystem& other)
{
    return getId() < other.getId();
}
