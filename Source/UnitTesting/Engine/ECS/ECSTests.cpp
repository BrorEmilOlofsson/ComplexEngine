#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/ECS/ECS.hpp"
#include <thread>

using namespace CLX;


struct DummyComponent
{
    int value = 0;
};

static ECSRegistry CreateRegistry()
{
    ECSRegistry registry;
    registry.RegisterComponentType<DummyComponent>();
    return registry;
}


TEST_CASE("ECS - Create Entity", "[ECS]")
{
    {
        ECS ecs(CreateRegistry());
        const EntityID entityID = ecs.CreateEntity();
        REQUIRE(entityID.id == 0);
    }

    {
        ECS ecs(CreateRegistry());
        const EntityID entityID1 = ecs.CreateEntity();
        const EntityID entityID2 = ecs.CreateEntity();
        REQUIRE(entityID1.id == 0);
        REQUIRE(entityID2.id == 1);
    }
}

TEST_CASE("ECS - Destroy Entity", "[ECS]")
{
    {
        ECS ecs(CreateRegistry());
        const EntityID entityID = ecs.CreateEntity();
        ecs.DestroyEntity(entityID);
        REQUIRE(entityID.id == 0);
    }

    {
        ECS ecs(CreateRegistry());
        const EntityID entityID1 = ecs.CreateEntity();
        const EntityID entityID2 = ecs.CreateEntity();

        ecs.DestroyEntity(entityID1);
        const EntityID entityID3 = ecs.CreateEntity();

        REQUIRE(entityID1.id == 0);
        REQUIRE(entityID2.id == 1);
        REQUIRE(entityID3.id == 0);
    }

    {
        ECS ecs(CreateRegistry());
        const EntityID entityID1 = ecs.CreateEntity();
        const EntityID entityID2 = ecs.CreateEntity();

        ecs.DestroyEntity(entityID1);
        const EntityID entityID3 = ecs.CreateEntity();

        ecs.DestroyEntity(entityID3);

        REQUIRE(ecs.GetEntityCount() == 1);
    }
}


TEST_CASE("ECS - Create Entity Generation ID", "[ECS]")
{
    {
        ECS ecs(CreateRegistry());
        const EntityID entityID = ecs.CreateEntity();
        REQUIRE(entityID.generation == 0);
    }
    {
        ECS ecs(CreateRegistry());
        const EntityID entityID1 = ecs.CreateEntity();
        const EntityID entityID2 = ecs.CreateEntity();
        ecs.DestroyEntity(entityID1);

        const EntityID entityID3 = ecs.CreateEntity();

        REQUIRE(entityID1.id == 0);
        REQUIRE(entityID3.generation == 1);

        ecs.DestroyEntity(entityID2);

        const EntityID entityID4 = ecs.CreateEntity();

        REQUIRE(entityID2.id == 1);
        REQUIRE(entityID4.generation == 1);

        ecs.DestroyEntity(entityID3);
        ecs.DestroyEntity(entityID4);

        const EntityID entityID5 = ecs.CreateEntity();

        REQUIRE(entityID5.id == 0);
        REQUIRE(entityID5.generation == 2);

        const EntityID entityID6 = ecs.CreateEntity();

        REQUIRE(entityID6.id == 1);
        REQUIRE(entityID6.generation == 2);

        const EntityID entityID7 = ecs.CreateEntity();

        REQUIRE(entityID7.id == 2);
        REQUIRE(entityID7.generation == 0);

    }
}

TEST_CASE("ECS - GetComponent with wrong generation ID", "[ECS]")
{
    {
        ECS ecs(CreateRegistry());
        const EntityID entityID = ecs.CreateEntity();
        ecs.AddComponent<DummyComponent>(entityID, DummyComponent{});

        ecs.DestroyEntity(entityID);

        {
            // Should be nullptr after destroying the entity
            auto* component = ecs.GetComponent<DummyComponent>(entityID);
            REQUIRE(component == nullptr);
        }



        const EntityID newEntityID = ecs.CreateEntity();

        auto* component = ecs.GetComponent<DummyComponent>(newEntityID);
        REQUIRE(component == nullptr);

        REQUIRE(entityID.id == newEntityID.id);
    }
}

TEST_CASE("ECS - Entity Iteration After Destroying Entities", "[ECS]")
{
    {
        ECS ecs(CreateRegistry());

        std::size_t entityCount = 5;
        for (std::size_t i = 0; i < entityCount; i++)
        {
            ecs.CreateEntity();
        }

        std::size_t counter = 0;
        std::vector<EntityID> entityIDs;
        for (const auto& entityView : ecs.ViewEntities())
        {
            counter++;
            entityIDs.push_back(entityView.GetEntityID());
        }

        REQUIRE(counter == entityCount);
        REQUIRE(entityIDs[0].id == 0);
        REQUIRE(entityIDs[1].id == 1);
        REQUIRE(entityIDs[2].id == 2);
        REQUIRE(entityIDs[3].id == 3);
        REQUIRE(entityIDs[4].id == 4);
    }

    {
        ECS ecs(CreateRegistry());

        std::size_t entityCount = 5;
        for (std::size_t i = 0; i < entityCount; i++)
        {
            ecs.CreateEntity();
        }

        ecs.DestroyEntity(EntityID{ 1 });

        std::size_t counter = 0;
        std::vector<EntityID> entityIDs;
        for (const auto& entityView : ecs.ViewEntities())
        {
            counter++;
            entityIDs.push_back(entityView.GetEntityID());
        }

        REQUIRE(counter == 4);
        REQUIRE(entityIDs[0].id == 0);
        REQUIRE(entityIDs[1].id == 2);
        REQUIRE(entityIDs[2].id == 3);
        REQUIRE(entityIDs[3].id == 4);
    }
}

TEST_CASE("ECS - Component Pool Copy", "[ECS]")
{
    ComponentPoolSBO c(std::type_identity<unsigned int>{});

    c.ResizeComponentIndices(10);

    for (unsigned int i = 0; i < 10; i++)
    {
        c.AddComponent<unsigned int>(EntityID{ i }, &i);
    }


    ComponentPoolSBO c1 = c;

    for (unsigned int i = 0; i < 10; i++)
    {
        unsigned int* a = c1.GetComponent<unsigned int>(EntityID{ i });
        REQUIRE(*a == i);
    }

}