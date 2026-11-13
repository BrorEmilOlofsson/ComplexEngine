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

[[nodiscard]] constexpr bool operator==(const TestComponent& lhs, const TestComponent& rhs)
{
    return lhs.name == rhs.name && lhs.value == rhs.value;
}

ECSRegistryNew CreateECSRegistry()
{
    ECSRegistryNew ecsRegistry;
    ecsRegistry.RegisterComponentType<TestComponent>();
    ecsRegistry.RegisterComponentType<CounterStruct>();
    ecsRegistry.RegisterComponentType<std::string>();
    ecsRegistry.RegisterComponentType<int>();
    REQUIRE(ECSRegisterComponentHelperNew::ComponentPoolID<TestComponent> == 0);
    REQUIRE(ECSRegisterComponentHelperNew::ComponentPoolID<CounterStruct> == 1);
    REQUIRE(ECSRegisterComponentHelperNew::ComponentPoolID<std::string> == 2);
    REQUIRE(ECSRegisterComponentHelperNew::ComponentPoolID<int> == 3);
    return ecsRegistry;
}

TEST_CASE("ComponentPoolNew::AddComponent () allocation")
{
    ComponentPoolNew pool = ComponentPoolNew::Create<CounterStruct>();

    const EntityID entityID{ 0, 0 };
    const auto& addedComponent = pool.AddComponent<CounterStruct>(entityID);

    REQUIRE(addedComponent == CounterStruct{});
}

TEST_CASE("ComponentPoolNew::AddComponent (const T&) allocation")
{
    ComponentPoolNew pool = ComponentPoolNew::Create<CounterStruct>();

    auto counterData = CounterStruct::CreateCounter();

    const CounterStruct defaultComponent{ std::string("Hello"), counterData };
    const EntityID entityID{ 0, 0 };
    const auto& addedComponent = pool.AddComponent(entityID, defaultComponent);

    // Check that the component was copied
    REQUIRE(defaultComponent.GetString() == "Hello");
    REQUIRE(addedComponent.GetString() == "Hello");

    REQUIRE(counterData->constructorCount == 1);
    REQUIRE(counterData->destructorCount == 0);
    REQUIRE(counterData->copyConstructorCount == 1);
    REQUIRE(counterData->copyAssignmentCount == 0);
    REQUIRE(counterData->moveConstructorCount == 0);
    REQUIRE(counterData->moveAssignmentCount == 0);
}

TEST_CASE("ComponentPoolNew::AddComponent (T&&) allocation")
{
    ComponentPoolNew pool(std::type_identity<CounterStruct>{});

    auto counterData = CounterStruct::CreateCounter();

    const EntityID entityID{ 0, 0 };
    const auto& addedComponent = pool.AddComponent(entityID, CounterStruct("Hello", counterData));

    REQUIRE(addedComponent.GetString() == "Hello");

    REQUIRE(counterData->constructorCount == 1);
    REQUIRE(counterData->destructorCount == 1);
    REQUIRE(counterData->copyConstructorCount == 0);
    REQUIRE(counterData->copyAssignmentCount == 0);
    REQUIRE(counterData->moveConstructorCount == 1);
    REQUIRE(counterData->moveAssignmentCount == 0);
}

TEST_CASE("ComponentPoolNew::AddComponent (Args&&...) allocation")
{
    ComponentPoolNew pool(std::type_identity<CounterStruct>{});

    auto counterData = CounterStruct::CreateCounter();

    const EntityID entityID{ 0, 0 };
    const auto& addedComponent = pool.AddComponent<CounterStruct>(entityID, counterData);

    REQUIRE(addedComponent.GetString() == CounterStruct::DefaultString);

    REQUIRE(counterData->constructorCount == 1);
    REQUIRE(counterData->destructorCount == 1);
    REQUIRE(counterData->copyConstructorCount == 0);
    REQUIRE(counterData->copyAssignmentCount == 0);
    REQUIRE(counterData->moveConstructorCount == 1);
    REQUIRE(counterData->moveAssignmentCount == 0);
}


TEST_CASE("ComponentPoolNew::AddComponentMoved (void*) allocation")
{
    ComponentPoolNew pool(std::type_identity<CounterStruct>{});

    auto counterData = CounterStruct::CreateCounter();

    const EntityID entityID{ 0, 0 };
    CounterStruct defaultComponent{ std::string("Hello"), counterData };
    const void* addedComponentPtr = pool.AddComponentMoved(entityID, &defaultComponent);
    const CounterStruct& addedComponent = *reinterpret_cast<const CounterStruct*>(addedComponentPtr);

    REQUIRE(defaultComponent.HasMoved());
    REQUIRE(addedComponent.GetString() == "Hello");

    REQUIRE(counterData->constructorCount == 1);
    REQUIRE(counterData->destructorCount == 0);
    REQUIRE(counterData->copyConstructorCount == 0);
    REQUIRE(counterData->copyAssignmentCount == 0);
    REQUIRE(counterData->moveConstructorCount == 1);
    REQUIRE(counterData->moveAssignmentCount == 0);
}

TEST_CASE("ComponentPoolNew::AddComponent (const T&)")
{
    // Add two components
    ComponentPoolNew pool(std::type_identity<TestComponent>{});
    const ComponentPoolNew& constPool = pool;

    const EntityID entityID1{ 0, 0 };
    {
        const TestComponent defaultComponent{ .name = "Hello", .value = 42 };
        const auto& addedComponent = pool.AddComponent(entityID1, defaultComponent);

        auto& components = constPool.GetComponents<TestComponent>();
        REQUIRE(components.size() == 1);
        REQUIRE(&components[0] == &addedComponent);

        auto entityIDs = pool.GetComponentIndexToEntityIDList();
        const std::vector<EntityID> expectedEntityIDs = { entityID1 };
        const std::vector<EntityID> e = entityIDs | std::ranges::to<std::vector>();
        REQUIRE(e == expectedEntityIDs);

        auto componentIndices = constPool.GetEntityIDsToComponentIndexList();
        auto expectedComponentIndices = std::vector<uint32_t>{ 0 };
        const std::vector<uint32_t> ci = componentIndices | std::ranges::to<std::vector>();
        REQUIRE(ci == expectedComponentIndices);
    }
    {
        const EntityID entityID2{ 1, 0 };
        const TestComponent defaultComponent2{ .name = "World", .value = 24 };
        const auto& addedComponent = pool.AddComponent(entityID2, defaultComponent2);

        auto& components = constPool.GetComponents<TestComponent>();
        REQUIRE(components.size() == 2);
        REQUIRE(&components[1] == &addedComponent);

        const auto entityIDs = pool.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
        const std::vector<EntityID> expectedEntityIDs = { entityID1, entityID2 };
        const std::vector<EntityID> e = entityIDs ;
        REQUIRE(e == expectedEntityIDs);

        auto componentIndices = constPool.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
        auto expectedComponentIndices = std::vector<uint32_t>{ 0, 1 };
        REQUIRE(componentIndices == expectedComponentIndices);
    }
}

TEST_CASE("ComponentPoolNew::AddComponent (const T&) with high Entity ID")
{
    ComponentPoolNew pool(std::type_identity<TestComponent>{});
    const ComponentPoolNew& constPool = pool;
    const EntityID entityID1{ 100, 40 };
    const TestComponent defaultComponent1{ .name = "Entity 1", .value = 1 };
    auto& addedComponent = pool.AddComponent(entityID1, defaultComponent1);

    auto& components = constPool.GetComponents<TestComponent>();
    REQUIRE(components.size() == 1);
    REQUIRE(&components.at(0) == &addedComponent);

    const auto  componentIndexToEntityIDList = pool.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
    const auto expectedComponentToEntityIDList = std::vector<EntityID>{ entityID1 };
    REQUIRE(componentIndexToEntityIDList == expectedComponentToEntityIDList);

    const auto entityIDsToComponentIndexList = pool.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
    auto expectedEntityIDsToComponentIndexList = std::vector<uint32_t>(static_cast<size_t>(entityID1.id) + 1, ComponentPoolNew::InvalidComponentIndex);
    expectedEntityIDsToComponentIndexList[entityID1.id] = 0;
    REQUIRE(entityIDsToComponentIndexList == expectedEntityIDsToComponentIndexList);
}

TEST_CASE("ComponentPoolNew::AddComponent (const T&) with same index")
{
    ComponentPoolNew pool = ComponentPoolNew::Create<TestComponent>();
    const ComponentPoolNew& constPool = pool;
    const EntityID entityID1{ 1, 4 };
    const TestComponent defaultComponent1{ .name = "Entity 1", .value = 1 };
    const TestComponent defaultComponent2{ .name = "Entity 2", .value = 2 };
    pool.AddComponent(entityID1, defaultComponent1);
    TestComponent& addedComponent = pool.AddComponent(entityID1, defaultComponent2); // Add component with same entityID, should replace the previous component

    auto& components = constPool.GetComponents<TestComponent>();
    REQUIRE(components.size() == 1);
    REQUIRE(&addedComponent == &components.at(0));
}

TEST_CASE("ComponentPoolNew::ComponentPoolNew (const &)")
{
    auto counterData = CounterStruct::CreateCounter();

    ComponentPoolNew pool1 = ComponentPoolNew::Create<CounterStruct>();

    pool1.ReserveComponents(10);
    pool1.AddComponent({ 0, 0 }, CounterStruct("Test", counterData));
    pool1.AddComponent({ 1, 0 }, CounterStruct("Test2", counterData));

    const ComponentPoolNew pool2 = pool1;

    REQUIRE(pool1 == pool2);

    std::span<const CounterStruct> components1 = pool1.GetComponents<CounterStruct>();
    std::span<const CounterStruct> components2 = pool2.GetComponents<CounterStruct>();

    REQUIRE(components1.data() != components2.data());
    REQUIRE(std::ranges::equal(components1, components2));

    auto entityIDs1 = pool1.GetComponentIndexToEntityIDList();
    auto entityIDs2 = pool2.GetComponentIndexToEntityIDList();
    REQUIRE(entityIDs1.data() != entityIDs2.data());
    REQUIRE(std::ranges::equal(entityIDs1, entityIDs2));

    auto componentIndices1 = pool1.GetEntityIDsToComponentIndexList();
    auto componentIndices2 = pool2.GetEntityIDsToComponentIndexList();
    REQUIRE(componentIndices1.data() != componentIndices2.data());
    REQUIRE(std::ranges::equal(componentIndices1, componentIndices2));

    REQUIRE(counterData->constructorCount == 2);
    REQUIRE(counterData->destructorCount == 2);
    REQUIRE(counterData->copyConstructorCount == 2);
    REQUIRE(counterData->copyAssignmentCount == 0);
    REQUIRE(counterData->moveConstructorCount == 2);
    REQUIRE(counterData->moveAssignmentCount == 0);
}

TEST_CASE("ComponentPoolNew::ComponentPoolNew (&&)")
{
    auto counterData = CounterStruct::CreateCounter();

    ComponentPoolNew pool1 = ComponentPoolNew::Create<CounterStruct>();

    pool1.ReserveComponents(10);
    pool1.AddComponent({ 0, 0 }, CounterStruct("Hello", counterData));
    pool1.AddComponent({ 1, 0 }, CounterStruct("Hello1", counterData));

    const std::vector<CounterStruct> components1 = pool1.GetComponents<CounterStruct>() | std::ranges::to<std::vector>();
    const std::vector<EntityID> entityIDs1 = pool1.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
    const std::vector<uint32_t> componentIndices1 = pool1.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();

    const ComponentPoolNew pool2 = std::move(pool1);

    REQUIRE(pool1.IsMovedFrom());
    REQUIRE(pool2.IsMovedFrom() == false);

    std::span<const CounterStruct> components2 = pool2.GetComponents<CounterStruct>();

    REQUIRE(std::ranges::equal(components1, components2));

    const std::vector<EntityID> entityIDs2 = pool2.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
    REQUIRE(std::ranges::equal(entityIDs1, entityIDs2));

    const std::vector<uint32_t> componentIndices2 = pool2.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
    REQUIRE(std::ranges::equal(componentIndices1, componentIndices2));

    REQUIRE(counterData->constructorCount == 2);
    REQUIRE(counterData->destructorCount == 2);
    REQUIRE(counterData->copyConstructorCount == 2);
    REQUIRE(counterData->copyAssignmentCount == 0);
    REQUIRE(counterData->moveConstructorCount == 2);
    REQUIRE(counterData->moveAssignmentCount == 0);
}

TEST_CASE("ComponentPoolNew::operator= (const &)")
{
    auto counterData = CounterStruct::CreateCounter();

    ComponentPoolNew pool1 = ComponentPoolNew::Create<CounterStruct>();

    pool1.ReserveComponents(10);
    pool1.AddComponent({ 0, 0 }, CounterStruct("Test", counterData));
    pool1.AddComponent({ 1, 0 }, CounterStruct("Test2", counterData));

    ComponentPoolNew pool2 = ComponentPoolNew::Create<CounterStruct>();
    pool2.ReserveComponents(10);
    pool2.AddComponent({ 2, 0 }, CounterStruct("Test3", counterData));
    pool2.AddComponent({ 3, 0 }, CounterStruct("Test3", counterData));
    
    pool2 = pool1;

    REQUIRE(pool1 == pool2);

    std::span<const CounterStruct> components1 = pool1.GetComponents<CounterStruct>();
    std::span<const CounterStruct> components2 = pool2.GetComponents<CounterStruct>();

    REQUIRE(components1.data() != components2.data());
    REQUIRE(std::ranges::equal(components1, components2));

    auto entityIDs1 = pool1.GetComponentIndexToEntityIDList();
    auto entityIDs2 = pool2.GetComponentIndexToEntityIDList();
    REQUIRE(entityIDs1.data() != entityIDs2.data());
    REQUIRE(std::ranges::equal(entityIDs1, entityIDs2));

    auto componentIndices1 = pool1.GetEntityIDsToComponentIndexList();
    auto componentIndices2 = pool2.GetEntityIDsToComponentIndexList();
    REQUIRE(componentIndices1.data() != componentIndices2.data());
    REQUIRE(std::ranges::equal(componentIndices1, componentIndices2));

    REQUIRE(counterData->constructorCount == 4);
    REQUIRE(counterData->destructorCount == 6);
    REQUIRE(counterData->copyConstructorCount == 2);
    REQUIRE(counterData->copyAssignmentCount == 0);
    REQUIRE(counterData->moveConstructorCount == 4);
    REQUIRE(counterData->moveAssignmentCount == 0);
}

TEST_CASE("ComponentPoolNew::operator= (&&)")
{
    auto counterData = CounterStruct::CreateCounter();

    ComponentPoolNew pool1 = ComponentPoolNew::Create<CounterStruct>();

    pool1.ReserveComponents(10);
    pool1.AddComponent({ 0, 0 }, CounterStruct("Hello", counterData));
    pool1.AddComponent({ 1, 0 }, CounterStruct("Hello1", counterData));

    const std::vector<CounterStruct> components1 = pool1.GetComponents<CounterStruct>() | std::ranges::to<std::vector>();
    const std::vector<EntityID> entityIDs1 = pool1.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
    const std::vector<uint32_t> componentIndices1 = pool1.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();

    ComponentPoolNew pool2 = ComponentPoolNew::Create<CounterStruct>();
    pool2.ReserveComponents(10);
    pool2.AddComponent({ 2, 0 }, CounterStruct("Hello2", counterData));
    pool2.AddComponent({ 3, 0 }, CounterStruct("Hello3", counterData));
    pool2 = std::move(pool1);

    REQUIRE(pool1.IsMovedFrom());
    REQUIRE(pool2.IsMovedFrom() == false);

    std::span<const CounterStruct> components2 = pool2.GetComponents<CounterStruct>();

    REQUIRE(std::ranges::equal(components1, components2));

    const std::vector<EntityID> entityIDs2 = pool2.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
    REQUIRE(std::ranges::equal(entityIDs1, entityIDs2));

    const std::vector<uint32_t> componentIndices2 = pool2.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
    REQUIRE(std::ranges::equal(componentIndices1, componentIndices2));

    REQUIRE(counterData->constructorCount == 4);
    REQUIRE(counterData->destructorCount == 6);
    REQUIRE(counterData->copyConstructorCount == 2);
    REQUIRE(counterData->copyAssignmentCount == 0);
    REQUIRE(counterData->moveConstructorCount == 4);
    REQUIRE(counterData->moveAssignmentCount == 0);
}

TEST_CASE("ComponentPoolNew::AddComponent + RemoveComponent")
{
    // Add two components + remove one + add another + add another + remove one + add another
    ComponentPoolNew pool = ComponentPoolNew::Create<TestComponent>();
    const ComponentPoolNew& constPool = pool;

    EntityID entityID0{ 0, 0 };
    EntityID entityID1{ 1, 0 };
    EntityID entityID2{ 2, 0 };
    EntityID entityID3{ 3, 0 };
    EntityID entityID4{ 4, 0 };
    EntityID entityID5{ 5, 0 };
    EntityID entityID0New{ 0, 1 };
    {
        const TestComponent defaultComponent0{ .name = "Entity 0", .value = 0 };
        const TestComponent defaultComponent1{ .name = "Entity 1", .value = 1 };
        const TestComponent defaultComponent2{ .name = "Entity 2", .value = 2 };
        const TestComponent defaultComponent3{ .name = "Entity 3", .value = 3 };
        const TestComponent defaultComponent4{ .name = "Entity 4", .value = 4 };
        {
            // Add component 0
            pool.AddComponent(entityID0, defaultComponent0);
            auto& components = constPool.GetComponents<TestComponent>();
            const std::vector<TestComponent> expectedComponents = { defaultComponent0 };
            REQUIRE(components == expectedComponents);

            auto componentIndexToEntityIDList = pool.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
            const std::vector<EntityID> expectedComponentToEntityIDList = { entityID0 };
            REQUIRE(componentIndexToEntityIDList == expectedComponentToEntityIDList);

            auto entityIDsToComponentIndexList = pool.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
            const std::vector<uint32_t> expectedEntityIDsToComponentIndexList = { 0 };
            REQUIRE(entityIDsToComponentIndexList == expectedEntityIDsToComponentIndexList);
        }
        {
            // Add component 1
            pool.AddComponent(entityID1, defaultComponent1);
            auto& components = constPool.GetComponents<TestComponent>();
            const std::vector<TestComponent> expectedComponents = { defaultComponent0, defaultComponent1 };
            REQUIRE(components == expectedComponents);

            auto componentIndexToEntityIDList = pool.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
            const std::vector<EntityID> expectedComponentToEntityIDList = { entityID0, entityID1 };
            REQUIRE(componentIndexToEntityIDList == expectedComponentToEntityIDList);

            auto entityIDsToComponentIndexList = pool.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
            const std::vector<uint32_t> expectedEntityIDsToComponentIndexList = { 0, 1 };
            REQUIRE(entityIDsToComponentIndexList == expectedEntityIDsToComponentIndexList);
        }
        {
            // Remove component 0
            pool.RemoveComponent(entityID0);
            auto& components = constPool.GetComponents<TestComponent>();
            const std::vector<TestComponent> expectedComponents = { defaultComponent1 };
            REQUIRE(components == expectedComponents);

            auto componentIndexToEntityIDList = pool.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
            const std::vector<EntityID> expectedComponentToEntityIDList = { entityID1 };
            REQUIRE(componentIndexToEntityIDList == expectedComponentToEntityIDList);

            auto entityIDsToComponentIndexList = pool.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
            const std::vector<uint32_t> expectedEntityIDsToComponentIndexList = { ComponentPoolNew::InvalidComponentIndex, 0 };
            REQUIRE(entityIDsToComponentIndexList == expectedEntityIDsToComponentIndexList);
        }
        {
            // Add component 2
            pool.AddComponent(entityID2, defaultComponent2);
            auto& components = constPool.GetComponents<TestComponent>();
            const std::vector<TestComponent> expectedComponents = { defaultComponent1, defaultComponent2 };
            REQUIRE(components == expectedComponents);

            auto componentIndexToEntityIDList = pool.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
            const std::vector<EntityID> expectedComponentToEntityIDList = { entityID1, entityID2 };
            REQUIRE(componentIndexToEntityIDList == expectedComponentToEntityIDList);

            auto entityIDsToComponentIndexList = pool.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
            const std::vector<uint32_t> expectedEntityIDsToComponentIndexList = { ComponentPoolNew::InvalidComponentIndex, 0, 1 };
            REQUIRE(entityIDsToComponentIndexList == expectedEntityIDsToComponentIndexList);
        }
        {
            // Add component 3
            pool.AddComponent(entityID3, defaultComponent3);
            auto& components = constPool.GetComponents<TestComponent>();
            const std::vector<TestComponent> expectedComponents = { defaultComponent1, defaultComponent2, defaultComponent3 };
            REQUIRE(components == expectedComponents);

            auto componentIndexToEntityIDList = pool.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
            const std::vector<EntityID> expectedComponentToEntityIDList = { entityID1, entityID2, entityID3 };
            REQUIRE(componentIndexToEntityIDList == expectedComponentToEntityIDList);

            auto entityIDsToComponentIndexList = pool.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
            const std::vector<uint32_t> expectedEntityIDsToComponentIndexList = { ComponentPoolNew::InvalidComponentIndex, 0, 1, 2 };
            REQUIRE(entityIDsToComponentIndexList == expectedEntityIDsToComponentIndexList);
        }
        {
            // Remove component 2
            pool.RemoveComponent(entityID2);
            auto& components = constPool.GetComponents<TestComponent>();
            const std::vector<TestComponent> expectedComponents = { defaultComponent1, defaultComponent3 };
            REQUIRE(components == expectedComponents);

            auto componentIndexToEntityIDList = pool.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
            const std::vector<EntityID> expectedComponentToEntityIDList = { entityID1, entityID3 };
            REQUIRE(componentIndexToEntityIDList == expectedComponentToEntityIDList);

            auto entityIDsToComponentIndexList = pool.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
            const std::vector<uint32_t> expectedEntityIDsToComponentIndexList = { ComponentPoolNew::InvalidComponentIndex, 0, ComponentPoolNew::InvalidComponentIndex, 1 };
            REQUIRE(entityIDsToComponentIndexList == expectedEntityIDsToComponentIndexList);
        }
        {
            // Add component 4
            pool.AddComponent(entityID4, defaultComponent4);
            auto& components = constPool.GetComponents<TestComponent>();
            const std::vector<TestComponent> expectedComponents = { defaultComponent1, defaultComponent3, defaultComponent4 };
            REQUIRE(components == expectedComponents);
            auto componentIndexToEntityIDList = pool.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
            const std::vector<EntityID> expectedComponentToEntityIDList = { entityID1, entityID3, entityID4 };
            REQUIRE(componentIndexToEntityIDList == expectedComponentToEntityIDList);
            auto entityIDsToComponentIndexList = pool.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
            const std::vector<uint32_t> expectedEntityIDsToComponentIndexList = { ComponentPoolNew::InvalidComponentIndex, 0, ComponentPoolNew::InvalidComponentIndex, 1, 2 };
            REQUIRE(entityIDsToComponentIndexList == expectedEntityIDsToComponentIndexList);
        }
        {
            // Add component 0 again with new entityID
            pool.AddComponent(entityID0New, defaultComponent0);
            auto& components = constPool.GetComponents<TestComponent>();
            const std::vector<TestComponent> expectedComponents = { defaultComponent1, defaultComponent3, defaultComponent4, defaultComponent0 };
            REQUIRE(components == expectedComponents);
            auto componentIndexToEntityIDList = pool.GetComponentIndexToEntityIDList() | std::ranges::to<std::vector>();
            const std::vector<EntityID> expectedComponentToEntityIDList = { entityID1, entityID3, entityID4, entityID0New };
            REQUIRE(componentIndexToEntityIDList == expectedComponentToEntityIDList);
            auto entityIDsToComponentIndexList = pool.GetEntityIDsToComponentIndexList() | std::ranges::to<std::vector>();
            const std::vector<uint32_t> expectedEntityIDsToComponentIndexList = { 3, 0, ComponentPoolNew::InvalidComponentIndex, 1, 2 };
            REQUIRE(entityIDsToComponentIndexList == expectedEntityIDsToComponentIndexList);
        }
    }
}

TEST_CASE("ComponentPoolNew::Iterate")
{
    ComponentPoolNew pool(std::type_identity<CounterStruct>{});

    auto counterData = CounterStruct::CreateCounter();

    CounterStruct defaultComponent{ std::string("Hello"), counterData };
    CounterStruct& addedComponent = pool.AddComponent<CounterStruct>({ 0, 0 }, std::move(defaultComponent));

    REQUIRE(counterData->constructorCount == 1);
    REQUIRE(counterData->destructorCount == 0);
    REQUIRE(counterData->copyConstructorCount == 0);
    REQUIRE(counterData->copyAssignmentCount == 0);
    REQUIRE(counterData->moveConstructorCount == 1);
    REQUIRE(counterData->moveAssignmentCount == 0);

    int count = 0;
    for (auto& component : pool.Iterate<CounterStruct>())
    {
        count++;
        REQUIRE(component.GetString() == "Hello");
        REQUIRE(&addedComponent == &component);
    }
    REQUIRE(count == 1);

    // Make sure no additional copies or moves were made during iteration
    REQUIRE(counterData->constructorCount == 1);
    REQUIRE(counterData->destructorCount == 0);
    REQUIRE(counterData->copyConstructorCount == 0);
    REQUIRE(counterData->copyAssignmentCount == 0);
    REQUIRE(counterData->moveConstructorCount == 1);
    REQUIRE(counterData->moveAssignmentCount == 0);
}

TEST_CASE("ECSNew::Multi Iterate")
{
    ECSRegistryNew ecsRegistry;
    EntitySerializationIDGenerator entityIDGenerator;

    ECSNew ecs(ecsRegistry, entityIDGenerator);


    std::vector<ComponentPoolNew> componentPools =
    {
        ComponentPoolNew(std::type_identity<CounterStruct>{}),
        ComponentPoolNew(std::type_identity<int>{}),
        ComponentPoolNew(std::type_identity<TestComponent>{}),
        ComponentPoolNew(std::type_identity<std::string>{}),
    };
    auto& counterStructPool = componentPools[0];
    auto& intPool = componentPools[1];
    auto& testComponentPool = componentPools[2];
    auto& stringPool = componentPools[3];

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

    ECSArchetype archetype
    {
        .componentTypeIndices = { 2, 3, 0 },
        .pools = componentPools
    };

    MultiComponentView<TestComponent, std::string, CounterStruct> multiIterHelper(&archetype, { 2, 3, 0 });

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


    std::vector<ComponentPoolNew> componentPools1 =
    {
        ComponentPoolNew(std::type_identity<CounterStruct>{}),
        ComponentPoolNew(std::type_identity<int>{}),
        ComponentPoolNew(std::type_identity<TestComponent>{}),
        ComponentPoolNew(std::type_identity<std::string>{}),
    };
    std::vector<ComponentPoolNew> componentPools2 =
    {
        ComponentPoolNew(std::type_identity<std::string>{}),
        ComponentPoolNew(std::type_identity<CounterStruct>{}),
        ComponentPoolNew(std::type_identity<TestComponent>{}),
    };
    auto& counterStructPool1 = componentPools1[0];
    auto& intPool1 = componentPools1[1];
    auto& testComponentPool1 = componentPools1[2];
    auto& stringPool1 = componentPools1[3];

    auto& counterStructPool2 = componentPools2[1];
    auto& testComponentPool2 = componentPools2[2];
    auto& stringPool2 = componentPools2[0];

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

    ECSArchetype archetype1
    {
        .componentTypeIndices = { 2, 3, 0, 1 },
        .pools = componentPools1
    };

    ECSArchetype archetype2
    {
        .componentTypeIndices = { 0, 1, 2 },
        .pools = componentPools2
    };

    MultiComponentView<std::string, CounterStruct> cView1(&archetype1, { 3, 0 });
    MultiComponentView<std::string, CounterStruct> cView2(&archetype2, { 0, 1 });
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


TEST_CASE("ECSNew::CreateEntity")
{
    ECSRegistryNew ecsRegistry = CreateECSRegistry();
    EntitySerializationIDGenerator entityIDGenerator;
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
    TestComponent& addedComponent = ecs.AddComponent(entityID, std::move(testComponent));

    REQUIRE(testComponent.name.empty()); // Moved-from state
    REQUIRE(addedComponent.name == "Test");
    REQUIRE(addedComponent.value == 42);
    const auto& entityData = ecs.GetEntityData(entityID);
    const DynamicBitset expectedMask = ecsRegistry.CreateComponentMaskFromTypeList<TestComponent>();
    REQUIRE(entityData.mask == expectedMask); // Component added

    const auto& archetypes = ecs.GetArchetypes();

    REQUIRE(archetypes.size() == 1);
    REQUIRE(archetypes.find(entityData.mask) != archetypes.end());
}

TEST_CASE("ECSNew::AddComponent (T&&) twice")
{
    ECSRegistryNew ecsRegistry = CreateECSRegistry();
    EntitySerializationIDGenerator entityIDGenerator;

    ECSNew ecs(ecsRegistry, entityIDGenerator);
    const EntityID entityID = ecs.CreateEntity();
    TestComponent testComponent{ "Test", 42 };
    TestComponent testComponent2{ "Test 2", 35 };
    TestComponent& addedComponent = ecs.AddComponent(entityID, std::move(testComponent));
    TestComponent& addedComponent2 = ecs.AddComponent(entityID, std::move(testComponent2));

    REQUIRE(&addedComponent == &addedComponent2);

    REQUIRE(testComponent.name.empty()); // Moved-from state
    REQUIRE(addedComponent.name == "Test 2");
    REQUIRE(addedComponent.value == 35);
    const auto& entityData = ecs.GetEntityData(entityID);
    const DynamicBitset expectedMask = ecsRegistry.CreateComponentMaskFromTypeList<TestComponent>();
    REQUIRE(entityData.mask == expectedMask); // Component added

    const auto& archetypes = ecs.GetArchetypes();

    REQUIRE(archetypes.size() == 1);
    REQUIRE(archetypes.find(entityData.mask) != archetypes.end());
}

TEST_CASE("ECSNew::AddComponent (T&&) Multiple components")
{
    ECSRegistryNew ecsRegistry = CreateECSRegistry();
    EntitySerializationIDGenerator entityIDGenerator;

    ECSNew ecs(ecsRegistry, entityIDGenerator);
    const EntityID entityID = ecs.CreateEntity();
    TestComponent testComponent{ "Test", 42 };
    int i{ 100 };
    ecs.AddComponent(entityID, std::move(testComponent));
    ecs.AddComponent(entityID, std::move(i));

    TestComponent* addedComponent = ecs.GetComponent<TestComponent>(entityID);
    int* addedInt = ecs.GetComponent<int>(entityID);

    REQUIRE(testComponent.name.empty()); // Moved-from state
    REQUIRE(addedComponent != nullptr);
    REQUIRE(addedComponent->name == "Test");
    REQUIRE(addedComponent->value == 42);
    REQUIRE(addedInt != nullptr);
    REQUIRE(*addedInt == 100);
    const auto& entityData = ecs.GetEntityData(entityID);
    const DynamicBitset expectedMask = ecsRegistry.CreateComponentMaskFromTypeList<TestComponent, int>();
    REQUIRE(entityData.mask == expectedMask); // Component added

    const auto& archetypes = ecs.GetArchetypes();

    REQUIRE(archetypes.size() == 2);
    REQUIRE(archetypes.contains(expectedMask));

    const ECSArchetype* archetype = ecs.GetArchetype(expectedMask);
    REQUIRE(archetype != nullptr);
    REQUIRE(archetype->componentTypeIndices == std::vector<size_t>{ 0, 3 });
    REQUIRE(archetype->pools.size() == 2);

    const DynamicBitset oldMask = ecsRegistry.CreateComponentMaskFromTypeList<TestComponent>();
    REQUIRE(archetypes.contains(oldMask));

    const ECSArchetype* oldArchetype = ecs.GetArchetype(oldMask);
    REQUIRE(oldArchetype != nullptr);
    REQUIRE(oldArchetype->componentTypeIndices == std::vector<size_t>{ 0 });
    REQUIRE(oldArchetype->pools.size() == 1);

    {
        auto& p = oldArchetype->pools[0];
        REQUIRE(p.GetType() == std::type_index(typeid(TestComponent)));
        auto entityIDs = p.GetComponentIndexToEntityIDList();
        REQUIRE(entityIDs.size() == 0);
    }

    {
        const EntityID entityID2 = ecs.CreateEntity();
        REQUIRE(entityID2 == EntityID{ .id = 1, .generation = 0 });
        TestComponent& addedComponent2 = ecs.AddComponent(entityID2, TestComponent{ "Test 43", 350 });
        REQUIRE(addedComponent2.name == "Test 43");
        REQUIRE(addedComponent2.value == 350);

        REQUIRE(oldArchetype->pools.size() == 1);
        auto& p = oldArchetype->pools[0];
        auto& components = p.GetComponents<TestComponent>();
        REQUIRE(components.size() == 1);
        REQUIRE(&components[0] == &addedComponent2);
    }
}

template<typename... Types>
[[nodiscard]] uint32_t CountIterations(ECSNew& ecs)
{
    uint32_t count = 0;
    for (const std::tuple<Types...> _ : ecs.ForEach<Types...>())
    {
        count++;
    }
    return count;
}

TEST_CASE("ECSNew::ForEach Empty ECS")
{
    ECSRegistryNew ecsRegistry = CreateECSRegistry();
    EntitySerializationIDGenerator entityIDGenerator;

    ECSNew ecs(ecsRegistry, entityIDGenerator);

    REQUIRE(CountIterations<TestComponent>(ecs) == 0);
    REQUIRE(CountIterations<std::string>(ecs) == 0);
    REQUIRE(CountIterations<CounterStruct>(ecs) == 0);
    REQUIRE(CountIterations<int>(ecs) == 0);
    REQUIRE(CountIterations<std::string, int, CounterStruct, TestComponent>(ecs) == 0);
    REQUIRE(CountIterations<int, CounterStruct>(ecs) == 0);
    REQUIRE(CountIterations<std::string, CounterStruct>(ecs) == 0);
    REQUIRE(CountIterations<CounterStruct, std::string, TestComponent>(ecs) == 0);
}

