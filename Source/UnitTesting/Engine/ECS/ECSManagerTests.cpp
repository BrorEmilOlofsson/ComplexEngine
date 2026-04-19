#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/ECS/ECSManager.hpp"

using namespace CLX;

TEST_CASE("ECSHandle destroys ecs in destructor", "[ECSManager]")
{
    CLX::ECSRegistry ecsRegistry;
    CLX::ECSManager ecsManager;

    {
        ECSOwningHandle ecs1 = CreateECS(ecsManager, ecsRegistry);
        ECSOwningHandle ecs2 = CreateECS(ecsManager, ecsRegistry);
        REQUIRE(&ecs1 != &ecs2);
        REQUIRE(ecsManager.GetECSCount() == 2);
    }

    REQUIRE(ecsManager.GetECSCount() == 0);

}

TEST_CASE("ECSHandle copy constructor creates a new ECS with the same data", "[ECSManager]")
{
    CLX::ECSRegistry ecsRegistry;
    ecsRegistry.RegisterComponentType<int>();
    CLX::ECSManager ecsManager;
    ECSOwningHandle ecs1 = CreateECS(ecsManager, ecsRegistry);
    const CLX::EntityID entityID = ecs1.Get().CreateEntity();
    ecs1.Get().AddComponent<int>(entityID, 42);
    ECSOwningHandle ecs2 = ecs1;
    REQUIRE(ecsManager.GetECSCount() == 2);
    REQUIRE(ecs1.Get().GetComponent<int>(entityID) != nullptr);
    REQUIRE(*ecs2.Get().GetComponent<int>(entityID) == 42);

}

TEST_CASE("ECSHandle copy assignment operator creates a new ECS with the same data", "[ECSManager]")
{
    CLX::ECSRegistry ecsRegistry;
    ecsRegistry.RegisterComponentType<int>();
    CLX::ECSManager ecsManager;
    ECSOwningHandle ecs1 = CreateECS(ecsManager, ecsRegistry);
    const CLX::EntityID entityID = ecs1.Get().CreateEntity();
    ecs1.Get().AddComponent<int>(entityID, 42);
    ECSOwningHandle ecs2 = CreateECS(ecsManager, ecsRegistry);
    REQUIRE(ecsManager.GetECSCount() == 2);
    ecs2 = ecs1;
    REQUIRE(ecsManager.GetECSCount() == 3);
    REQUIRE(ecs1.Get().GetComponent<int>(entityID) != nullptr);
    REQUIRE(*ecs2.Get().GetComponent<int>(entityID) == 42);
}

TEST_CASE("ECSHandle move constructor transfers ownership", "[ECSManager]")
{
    CLX::ECSRegistry ecsRegistry;
    ecsRegistry.RegisterComponentType<int>();
    CLX::ECSManager ecsManager;
    ECSOwningHandle ecs1 = CreateECS(ecsManager, ecsRegistry);
    const CLX::EntityID entityID = ecs1.Get().CreateEntity();
    int* componentPtr = &ecs1.Get().AddComponent<int>(entityID, 42);
    ECSOwningHandle ecs2 = std::move(ecs1);
    REQUIRE(ecsManager.GetECSCount() == 1);
    REQUIRE(ecs2.Get().GetComponent<int>(entityID) != nullptr);
    REQUIRE(*ecs2.Get().GetComponent<int>(entityID) == 42);
    REQUIRE(ecs2.Get().GetComponent<int>(entityID) == componentPtr);
}


TEST_CASE("ECSHandle move assignment operator transfers ownership", "[ECSManager]")
{
    CLX::ECSRegistry ecsRegistry;
    ecsRegistry.RegisterComponentType<int>();
    CLX::ECSManager ecsManager;
    ECSOwningHandle ecs1 = CreateECS(ecsManager, ecsRegistry);
    const CLX::EntityID entityID = ecs1.Get().CreateEntity();
    int* componentPtr = &ecs1.Get().AddComponent<int>(entityID, 42);
    ECSOwningHandle ecs2 = CreateECS(ecsManager, ecsRegistry);
    REQUIRE(ecsManager.GetECSCount() == 2);
    ecs2 = std::move(ecs1);
    REQUIRE(ecsManager.GetECSCount() == 1);
    REQUIRE(ecs2.Get().GetComponent<int>(entityID) != nullptr);
    REQUIRE(*ecs2.Get().GetComponent<int>(entityID) == 42);
    REQUIRE(ecs2.Get().GetComponent<int>(entityID) == componentPtr);
}