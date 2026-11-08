#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/ECS/ECSNew.hpp"
#include "Engine/Utility/PrintStruct.hpp"

using namespace CLX;

TEST_CASE("ECSNew Constructor")
{
    ECSRegistryNew ecsRegistry;
    EntitySerializationIDGenerator entityIDGenerator;
    REQUIRE_NOTHROW(ECSNew(ecsRegistry, entityIDGenerator));
}

struct TestComponent
{
    std::string name;
    uint32_t value = 0;
};


TEST_CASE("ComponentPoolSBO::AddComponent (const T&)")
{
    ComponentPoolSBO pool(std::type_identity<CounterStruct>{});
    pool.ResizeComponentIndices(10);

    std::shared_ptr<CounterStructCounter> counter = std::make_shared<CounterStructCounter>();

    const CounterStruct defaultComponent{ std::string("Hello"), counter };
    auto& addedComponent = pool.AddComponent<CounterStruct>({ 0, 0 }, defaultComponent);

    // Check that the component was copied
    REQUIRE(defaultComponent.GetString() == "Hello");
    REQUIRE(addedComponent.GetString() == "Hello");

    REQUIRE(counter->constructorCount == 1);
    REQUIRE(counter->destructorCount == 0);
    REQUIRE(counter->copyConstructorCount == 1);
    REQUIRE(counter->copyAssignmentCount == 0);
    REQUIRE(counter->moveConstructorCount == 0);
    REQUIRE(counter->moveAssignmentCount == 0);
}

TEST_CASE("ComponentPoolSBO::Iterate")
{
    ComponentPoolSBO pool(std::type_identity<CounterStruct>{});
    pool.ResizeComponentIndices(10);

    std::shared_ptr<CounterStructCounter> counter = std::make_shared<CounterStructCounter>();

    CounterStruct defaultComponent{ std::string("Hello"), counter };
    CounterStruct& addedComponent = pool.AddComponent<CounterStruct>({ 0, 0 }, std::move(defaultComponent));

    REQUIRE(addedComponent.GetString() == "Hello");

    // Check that the component was moved
    REQUIRE(defaultComponent.GetString().empty());

    REQUIRE(counter->constructorCount == 1);
    REQUIRE(counter->destructorCount == 0);
    REQUIRE(counter->copyConstructorCount == 0);
    REQUIRE(counter->copyAssignmentCount == 0);
    REQUIRE(counter->moveConstructorCount == 1);
    REQUIRE(counter->moveAssignmentCount == 0);

    int count = 0;
    for (auto& component : pool.Iterate<CounterStruct>())
    {
        count++;
        REQUIRE(component.GetString() == "Hello");
        REQUIRE(&addedComponent == &component);
    }
    REQUIRE(count == 1);

    // Make sure no additional copies or moves were made during iteration
    REQUIRE(counter->constructorCount == 1);
    REQUIRE(counter->destructorCount == 0);
    REQUIRE(counter->copyConstructorCount == 0);
    REQUIRE(counter->copyAssignmentCount == 0);
    REQUIRE(counter->moveConstructorCount == 1);
    REQUIRE(counter->moveAssignmentCount == 0);
}

TEST_CASE("ECSNew::Multi Iterate")
{
    ECSRegistryNew ecsRegistry;
    EntitySerializationIDGenerator entityIDGenerator;

    ECSNew ecs(ecsRegistry, entityIDGenerator);


    std::vector<ComponentPoolSBO> componentPools =
    {
        ComponentPoolSBO(std::type_identity<CounterStruct>{}),
        ComponentPoolSBO(std::type_identity<int>{}),
        ComponentPoolSBO(std::type_identity<TestComponent>{}),
        ComponentPoolSBO(std::type_identity<std::string>{}),
    };
    auto& counterStructPool = componentPools[0];
    auto& intPool = componentPools[1];
    auto& testComponentPool = componentPools[2];
    auto& stringPool = componentPools[3];

    counterStructPool.ResizeComponentIndices(10);
    intPool.ResizeComponentIndices(10);
    testComponentPool.ResizeComponentIndices(10);
    stringPool.ResizeComponentIndices(10);

    EntityID entityID1{ 0, 0 };
    EntityID entityID2{ 1, 0 };
    EntityID entityID3{ 2, 0 };

    counterStructPool.AddComponent<CounterStruct>(entityID1);
    counterStructPool.AddComponent<CounterStruct>(entityID2);
    counterStructPool.AddComponent<CounterStruct>(entityID3);
    intPool.AddComponent<int>(entityID1, 1);
    intPool.AddComponent<int>(entityID3, 3);
    intPool.AddComponent<int>(entityID2, 2);
    testComponentPool.AddComponent<TestComponent>(entityID3);
    testComponentPool.AddComponent<TestComponent>(entityID2);
    testComponentPool.AddComponent<TestComponent>(entityID1);
    stringPool.AddComponent<std::string>(entityID2, "EntityID 2");
    stringPool.AddComponent<std::string>(entityID1, "EntityID 1");
    stringPool.AddComponent<std::string>(entityID3, "EntityID 3");


    MultiComponentView<TestComponent, std::string, CounterStruct> multiIterHelper(&componentPools, { 2, 3, 0 });

    int count = 0;
    for (auto [test, str, counter] : multiIterHelper)
    {
        counter;
        test;
        str;
        if (count == 0)
        {
            REQUIRE(str == "EntityID 3");
        }
        else if (count == 1)
        {
            REQUIRE(str == "EntityID 2");
        }
        else if (count == 2)
        {
            REQUIRE(str == "EntityID 1");
        }
        count++;
    }
    REQUIRE(count == 3);

}

TEST_CASE("ECSNew::Archetype Iterate")
{
    ECSRegistryNew ecsRegistry;
    EntitySerializationIDGenerator entityIDGenerator;

    ECSNew ecs(ecsRegistry, entityIDGenerator);


    std::vector<ComponentPoolSBO> componentPools1 =
    {
        ComponentPoolSBO(std::type_identity<CounterStruct>{}),
        ComponentPoolSBO(std::type_identity<int>{}),
        ComponentPoolSBO(std::type_identity<TestComponent>{}),
        ComponentPoolSBO(std::type_identity<std::string>{}),
    };
    std::vector<ComponentPoolSBO> componentPools2 =
    {
        ComponentPoolSBO(std::type_identity<std::string>{}),
        ComponentPoolSBO(std::type_identity<CounterStruct>{}),
        ComponentPoolSBO(std::type_identity<TestComponent>{}),
    };
    auto& counterStructPool1 = componentPools1[0];
    auto& intPool1 = componentPools1[1];
    auto& testComponentPool1 = componentPools1[2];
    auto& stringPool1 = componentPools1[3];

    auto& counterStructPool2 = componentPools2[1];
    auto& testComponentPool2 = componentPools2[2];
    auto& stringPool2 = componentPools2[0];

    counterStructPool1.ResizeComponentIndices(10);
    intPool1.ResizeComponentIndices(10);
    testComponentPool1.ResizeComponentIndices(10);
    stringPool1.ResizeComponentIndices(10);

    counterStructPool2.ResizeComponentIndices(10);
    testComponentPool2.ResizeComponentIndices(10);
    stringPool2.ResizeComponentIndices(10);

    EntityID entityID1{ 0, 0 };
    EntityID entityID2{ 1, 0 };
    EntityID entityID3{ 2, 0 };
    EntityID entityID4{ 3, 0 };
    EntityID entityID5{ 4, 0 };

    counterStructPool1.AddComponent<CounterStruct>(entityID1);
    counterStructPool1.AddComponent<CounterStruct>(entityID3);
    counterStructPool1.AddComponent<CounterStruct>(entityID4);
    intPool1.AddComponent<int>(entityID1, 1);
    intPool1.AddComponent<int>(entityID3, 3);
    intPool1.AddComponent<int>(entityID4, 2);
    testComponentPool1.AddComponent<TestComponent>(entityID3);
    testComponentPool1.AddComponent<TestComponent>(entityID4);
    testComponentPool1.AddComponent<TestComponent>(entityID3);
    stringPool1.AddComponent<std::string>(entityID1, "EntityID 1");
    stringPool1.AddComponent<std::string>(entityID3, "EntityID 3");
    stringPool1.AddComponent<std::string>(entityID4, "EntityID 4");

    counterStructPool2.AddComponent<CounterStruct>(entityID2);
    counterStructPool2.AddComponent<CounterStruct>(entityID5);
    testComponentPool2.AddComponent<TestComponent>(entityID2);
    testComponentPool2.AddComponent<TestComponent>(entityID5);
    stringPool2.AddComponent<std::string>(entityID5, "EntityID 5");
    stringPool2.AddComponent<std::string>(entityID2, "EntityID 2");


    MultiComponentView<std::string, CounterStruct> cView1(&componentPools1, { 3, 0 });
    MultiComponentView<std::string, CounterStruct> cView2(&componentPools2, { 0, 1 });
    std::vector archetypes = { cView1, cView2 };
    ECSArchetypeView<std::string, CounterStruct> archetypeView(std::move(archetypes));
    int count = 0;
    for (auto [str, counter] : archetypeView)
    {
        if (count == 0)
        {
            REQUIRE(str == "EntityID 1");
        }
        else if (count == 1)
        {
            REQUIRE(str == "EntityID 3");
        }
        else if (count == 2)
        {
            REQUIRE(str == "EntityID 4");
        }
        else if (count == 3)
        {
            REQUIRE(str == "EntityID 5");
        }
        else if (count == 4)
        {
            REQUIRE(str == "EntityID 2");
        }
        counter;
        str;
        count++;
    }
    REQUIRE(count == 5);
}


TEST_CASE("ECSNew::ForEach")
{
    ECSRegistryNew ecsRegistry;
    EntitySerializationIDGenerator entityIDGenerator;

    ecsRegistry.RegisterComponentType<TestComponent>();

    ECSNew ecs(ecsRegistry, entityIDGenerator);


    for (auto [testComponent] : ecs.ForEach<TestComponent>())
    {
        testComponent;
    }
}

ECSRegistryNew CreateECSRegistry()
{
    ECSRegistryNew ecsRegistry;
    ecsRegistry.RegisterComponentType<TestComponent>();
    return ecsRegistry;
}

TEST_CASE("ECSNew::CreateEntity")
{
    ECSRegistryNew ecsRegistry = CreateECSRegistry();
    EntitySerializationIDGenerator entityIDGenerator;
    auto testComponentTypeID = ECSRegisterComponentHelperNew::ComponentPoolID<TestComponent>;
    REQUIRE(testComponentTypeID == 0);
    {
        // Add an entity and check that it has the correct ID and generation
        ECSNew ecs(ecsRegistry, entityIDGenerator);
        const EntityID entityID = ecs.CreateEntity();
        REQUIRE(entityID.id == 0);
        REQUIRE(entityID.generation == 0);
        const auto& entityData = ecs.GetEntityData(entityID);
        REQUIRE(entityData.mask.None()); // No components added
        REQUIRE(entityData.generation == 0);
        REQUIRE(entityData.isActive == true);
    }
    {
        // Add 2 entities and check that it has the correct ID and generation
        ECSNew ecs(ecsRegistry, entityIDGenerator);
        const EntityID entityID1 = ecs.CreateEntity();
        const EntityID entityID2 = ecs.CreateEntity();
        REQUIRE(entityID1.id == 0);
        REQUIRE(entityID1.generation == 0);
        const auto& entityData1 = ecs.GetEntityData(entityID1);
        REQUIRE(entityData1.mask.None()); // No components added
        REQUIRE(entityData1.generation == 0);
        REQUIRE(entityData1.isActive == true);
        const auto& entityData2 = ecs.GetEntityData(entityID2);
        REQUIRE(entityData2.mask.None()); // No components added
        REQUIRE(entityData2.generation == 0);
        REQUIRE(entityData2.isActive == true);
    }
}

TEST_CASE("ECSNew::AddComponent (T&&)")
{
    ECSRegistryNew ecsRegistry = CreateECSRegistry();
    EntitySerializationIDGenerator entityIDGenerator;
    ECSNew ecs(ecsRegistry, entityIDGenerator);
    const EntityID entityID = ecs.CreateEntity();
    TestComponent testComponent{ "Test", 42 };
    TestComponent& addedComponent = ecs.AddComponent<TestComponent>(entityID, std::move(testComponent));
    
    REQUIRE(testComponent.name.empty()); // Moved-from state
    REQUIRE(addedComponent.name == "Test");
    REQUIRE(addedComponent.value == 42);
    const auto& entityData = ecs.GetEntityData(entityID);
    REQUIRE(entityData.mask.Any()); // Component added
}