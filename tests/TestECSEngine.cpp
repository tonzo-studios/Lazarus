#include "catch/catch.hpp"

#include <lazarus/ECS/ECSEngine.h>
#include <lazarus/common.h>

using namespace lz;

struct TestEvent
{
    int num;
};

struct TestComponent
{
    TestComponent(int num)
        : num(num)
    {
    }

    int num;
};

struct TestComponent2
{
    TestComponent2(int num)
    : num(num)
    {
    }

    int num;
};

class TestSystem : public Updateable, public EventListener<TestEvent>
{
public:
    virtual void update(ECSEngine& engine)
    {
        ++x;
    }

    virtual void receive(ECSEngine& engine, const TestEvent& event)
    {
        x += event.num;
    }

    int x = 0;
};

void addNumBy10(Entity* ent, TestComponent* comp)
{
    comp->num += 10;
}

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

TEST_CASE("entity management")
{
    ECSEngine engine;
    // Add one entity with TestComponent and TestComponent2
    Entity *entity = engine.addEntity();
    Identifier id1 = entity->getId();
    entity->addComponent<TestComponent>(0);
    entity->addComponent<TestComponent2>(0);
    // Add another entity, only with TestComponent
    entity = engine.addEntity();
    Identifier id2 = entity->getId();
    entity->addComponent<TestComponent>(0);

    SECTION("entitiesWithComponents")
    {
        // Both entities have TestComponent
        auto entities = engine.entitiesWithComponents<TestComponent>();
        REQUIRE(entities.size() == 2);

        // Only entity 1 has TestComponent2
        entities = engine.entitiesWithComponents<TestComponent2>();
        REQUIRE(entities.size() == 1);
        REQUIRE(entities[0]->getId() == id1);

        // Only entity 1 has both components
        entities = engine.entitiesWithComponents<TestComponent,
                                                 TestComponent2>();
        REQUIRE(entities.size() == 1);
        REQUIRE(entities[0]->getId() == id1);
    }
    SECTION("applyToEach with lambda")
    {
        // Check that components have constructed values
        TestComponent* comp = engine.getEntity(id1)->get<TestComponent>();
        REQUIRE(comp->num == 0);
        comp = engine.getEntity(id2)->get<TestComponent>();
        REQUIRE(comp->num == 0);

        // Update TestComponent on all entities that have it
        engine.applyToEach<TestComponent>([](Entity* ent, TestComponent* comp)
        {
            comp->num++;
        });

        // Check that it was modified for both entities
        comp = engine.getEntity(id1)->get<TestComponent>();
        REQUIRE(comp->num == 1);
        comp = engine.getEntity(id2)->get<TestComponent>();
        REQUIRE(comp->num == 1);
    }
    SECTION("applyToEach with std::function")
    {
        // Check that components have constructed values
        TestComponent* comp = engine.getEntity(id1)->get<TestComponent>();
        REQUIRE(comp->num == 0);
        comp = engine.getEntity(id2)->get<TestComponent>();
        REQUIRE(comp->num == 0);

        // Update TestComponent on all entities that have it
        engine.applyToEach<TestComponent>(addNumBy10);

        // Check that it was modified for both entities
        comp = engine.getEntity(id1)->get<TestComponent>();
        REQUIRE(comp->num == 10);
        comp = engine.getEntity(id2)->get<TestComponent>();
        REQUIRE(comp->num == 10);
    }
}

TEST_CASE("event management")
{
    ECSEngine engine;
    TestSystem system;
    TestEvent event{10};
    SECTION("event listener subscription/unsubscription")
    {
        // System is not subscribed yet
        REQUIRE_THROWS_AS(engine.unsubscribe<TestEvent>(&system),
                          __lz::LazarusException);
        // Subscribe system
        REQUIRE_NOTHROW(engine.subscribe<TestEvent>(&system));
        // Try to unsubscribe another similar system
        TestSystem other;
        REQUIRE_THROWS_AS(engine.unsubscribe<TestEvent>(&other),
                          __lz::LazarusException);
        // Unsubscribe original system
        REQUIRE_NOTHROW(engine.unsubscribe<TestEvent>(&system));
    }
    SECTION("emitting and receiving events from the engine without subscribers")
    {
        REQUIRE(system.x == 0);
        // Emit without any subscribers
        engine.emit(event);
        REQUIRE(system.x == 0);
    }
    SECTION("emitting and receiving events from the engine with subscribers")
    {
        // Emit with subscribers
        REQUIRE_NOTHROW(engine.subscribe<TestEvent>(&system));
        engine.emit(event);
        REQUIRE(system.x == 10);
    }
}

TEST_CASE("updateable management")
{
    ECSEngine engine;
    TestSystem system;
    SECTION("update without subscribers")
    {
        REQUIRE(system.x == 0);
        engine.update();
        REQUIRE(system.x == 0);
    }
    SECTION("update with subscribers")
    {
        REQUIRE(system.x == 0);
        engine.registerUpdateable(&system);
        engine.update();
        REQUIRE(system.x == 1);
    }
}
