#include "catch/catch.hpp"

#include <lazarus/ECS/ECSEngine.h>
#include <lazarus/common.h>

using namespace lz;

TEST_CASE("add entities to the engine")
{
    ECSEngine engine;
    SECTION("add new entity")
    {
        Entity* entity = engine.addEntity();
        REQUIRE(entity != nullptr);
        Identifier id = entity->getId();
        Entity *other = engine.addEntity();
        REQUIRE(other->getId() == id + 1);
    }
    SECTION("add existing entity")
    {
        Entity entity;
        Identifier id = entity.getId();
        REQUIRE_NOTHROW(engine.addEntity(entity));
        // Add new empty entity
        Entity* other = engine.addEntity();
        REQUIRE(other->getId() == id + 1);
    }
}

TEST_CASE("get entity from identifier")
{
    ECSEngine engine;
    Entity entity;
    Identifier id = entity.getId();
    engine.addEntity(entity);
    SECTION("get existing entity")
    {
        Entity* entPtr = engine.getEntity(id);
        REQUIRE(entPtr != nullptr);
        REQUIRE(entPtr->getId() == id);
    }
    SECTION("get non-existing entity")
    {
        Entity* entPtr = engine.getEntity(1512);
        REQUIRE(entPtr == nullptr);
    }
} 