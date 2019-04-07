#include "catch/catch.hpp"

#include <lazarus/ECS/EntityHolder.h>
#include <lazarus/common.h>

using namespace lz;

TEST_CASE("add entities to EntityHolder")
{
    EntityHolder entities;
    SECTION("add new entity")
    {
        Entity* entity = entities.addEntity();
        REQUIRE(entity != nullptr);
        Identifier id = entity->getId();
        Entity *other = entities.addEntity();
        REQUIRE(other->getId() == id + 1);
    }
    SECTION("add existing entity")
    {
        Entity entity;
        Identifier id = entity.getId();
        REQUIRE_NOTHROW(entities.addEntity(entity));
        // Add new empty entity
        Entity* other = entities.addEntity();
        REQUIRE(other->getId() == id + 1);
    }
}

TEST_CASE("get entity from identifier")
{
    EntityHolder entities;
    Entity entity;
    Identifier id = entity.getId();
    entities.addEntity(entity);
    SECTION("get existing entity")
    {
        Entity* entPtr = entities.getEntity(id);
        REQUIRE(entPtr != nullptr);
        REQUIRE(entPtr->getId() == id);
    }
    SECTION("get non-existing entity")
    {
        Entity* entPtr = entities.getEntity(1512);
        REQUIRE(entPtr == nullptr);
    }
}