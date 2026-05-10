#include <External/Catch2/catch_amalgamated.hpp>

#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/AllEngineComponents.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include "Editor/EntityCompositionInstantiationManager.hpp"

using namespace CLX;

ECSRegistry CreateECSRegistry()
{
    ECSRegistry registry;
    registry.RegisterComponentType<TransformComponent>(true);
    registry.RegisterComponentType<NameComponent>(true);
    registry.RegisterComponentType<TransformHierarchyComponent>(true);
    registry.RegisterComponentType<EntityCompositionInstantiationComponent>(false);
    return registry;
}

DataTypeRegistry CreateDataTypeRegistry()
{
    DataTypeRegistry registry;
    registry.RegisterType<TransformComponent>(true);
    registry.RegisterType<NameComponent>(true);
    registry.RegisterType<TransformHierarchyComponent>(true);
    registry.RegisterType<EntityCompositionInstantiationComponent>(true);
    return registry;
}

TEST_CASE("Create Entities")
{
    ECS ecs(CreateECSRegistry());
    EditorCommandTracker commandTracker;
    std::vector<EntityID> rootEntities;

    for (size_t i = 0; i < 10; i++)
    {
        EntityID entityID = CreateEntity(ecs, rootEntities, InvalidEntityID, commandTracker);
    }

    REQUIRE(rootEntities.size() == 10);
    commandTracker.UndoCommand();
    REQUIRE(rootEntities.size() == 9);
    commandTracker.UndoCommand();
    REQUIRE(rootEntities.size() == 8);
    commandTracker.RedoCommand();
    REQUIRE(rootEntities.size() == 9);
}

TEST_CASE("Destroy Entity")
{
    ECS ecs(CreateECSRegistry());
    EditorCommandTracker commandTracker;
    std::vector<EntityID> rootEntities;

    for (size_t i = 0; i < 10; i++)
    {
        EntityID entityID = CreateEntity(ecs, rootEntities, InvalidEntityID, commandTracker);
    }

    REQUIRE(ecs.GetEntityCount() == 10);


    DestroyEntity(ecs, EntityID{ 4 }, rootEntities, commandTracker);

    REQUIRE(commandTracker.GetUndoSize() == 11);

    REQUIRE(rootEntities.size() == 9);
    commandTracker.UndoCommand();

    REQUIRE(rootEntities.size() == 10);
    REQUIRE(rootEntities[4] == EntityID{ 4 });
}

TEST_CASE("Add Child Entity")
{
    ECS ecs(CreateECSRegistry());
    EditorCommandTracker commandTracker;
    std::vector<EntityID> rootEntities;
    for (size_t i = 0; i < 10; i++)
    {
        EntityID entityID = ecs.CreateEntity();
        rootEntities.push_back(entityID);
    }

    SetParentEntity(ecs, EntityID{ 0 }, EntityID{ 4 }, rootEntities, commandTracker, LastIndex{});

    REQUIRE(rootEntities.size() == 9);
    REQUIRE(commandTracker.GetUndoSize() == 1);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 4 })->parent == EntityID{ 0 });
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 0 })->children.size() == 1);

    commandTracker.UndoCommand();

    REQUIRE(rootEntities.size() == 10);
    REQUIRE(commandTracker.GetRedoSize() == 1);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 4 })->parent == InvalidEntityID);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 0 })->children.size() == 0);

    commandTracker.RedoCommand();

    REQUIRE(rootEntities.size() == 9);
    REQUIRE(commandTracker.GetUndoSize() == 1);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 4 })->parent == EntityID{ 0 });
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 0 })->children.size() == 1);

    SetParentEntity(ecs, EntityID{ 1 }, EntityID{ 4 }, rootEntities, commandTracker, LastIndex{});

    REQUIRE(rootEntities.size() == 9);
    REQUIRE(commandTracker.GetUndoSize() == 2);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 4 })->parent == EntityID{ 1 });
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 0 })->children.size() == 0);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 1 })->children.size() == 1);

    SetParentEntity(ecs, EntityID{ 1 }, EntityID{ 0 }, rootEntities, commandTracker, LastIndex{});

    REQUIRE(rootEntities.size() == 8);
    REQUIRE(commandTracker.GetUndoSize() == 3);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 0 })->parent == EntityID{ 1 });
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 1 })->children.size() == 2);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 4 })->parent == EntityID{ 1 });
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 1 })->children[0] == EntityID{ 4 });
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 1 })->children[1] == EntityID{ 0 });

    commandTracker.UndoCommand();

    REQUIRE(rootEntities.size() == 9);
    REQUIRE(commandTracker.GetRedoSize() == 1);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 0 })->parent == InvalidEntityID);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 1 })->children.size() == 1);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 4 })->parent == EntityID{ 1 });

    commandTracker.RedoCommand();

    SetParentEntity(ecs, EntityID{ 2 }, EntityID{ 4 }, rootEntities, commandTracker, LastIndex{});

    REQUIRE(rootEntities.size() == 8);
    REQUIRE(commandTracker.GetUndoSize() == 4);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 4 })->parent == EntityID{ 2 });
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 2 })->children.size() == 1);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 1 })->children.size() == 1);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 1 })->children[0] == EntityID{ 0 });

    commandTracker.UndoCommand();

    REQUIRE(rootEntities.size() == 8);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 4 })->parent == EntityID{ 1 });
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 1 })->children.size() == 2);
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 1 })->children[0] == EntityID{ 4 });
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 1 })->children[1] == EntityID{ 0 });
    REQUIRE(ecs.GetComponent<TransformHierarchyComponent>(EntityID{ 2 })->children.size() == 0);
}

TEST_CASE("Instantiate Entity Composition")
{
    {

        ECSManager ecsManager;

        EntityCompositionAsset compositionAsset(EntityComposition(ecsManager, ecsManager.CreateECS(CreateECSRegistry())), std::filesystem::path("path/to/asset"));

        ECSOwningHandle targetECS(ecsManager, ecsManager.CreateECS(CreateECSRegistry()));

        EditorCommandTracker commandTracker;
        const DataTypeRegistry dataTypeRegistry = CreateDataTypeRegistry();
        std::vector<EntityID> rootEntities;
        EntityCompositionInstantiationManager instantiationManager;

        const EntityID instantiatedRootEntityID = InstantiateEntityComposition(
            ToHandle(targetECS),
            EntityCompositionAssetHandle(compositionAsset),
            InvalidEntityID,
            instantiationManager,
            dataTypeRegistry,
            rootEntities,
            commandTracker
        );

        TransformHierarchyComponent* transformHierarchy = targetECS.Get().GetComponent<TransformHierarchyComponent>(instantiatedRootEntityID);
        REQUIRE(targetECS.Get().GetEntityCount() == 1);
        REQUIRE(transformHierarchy->parent == InvalidEntityID);
        REQUIRE(transformHierarchy->children.empty());
    }

    {

        ECSManager ecsManager;

        EntityCompositionAsset compositionAsset(EntityComposition(ecsManager, ecsManager.CreateECS(CreateECSRegistry())), std::filesystem::path("path/to/asset"));

        ECSOwningHandle targetECS(ecsManager, ecsManager.CreateECS(CreateECSRegistry()));

        const EntityID parentID = targetECS.Get().CreateEntity();

        EditorCommandTracker commandTracker;
        const DataTypeRegistry dataTypeRegistry = CreateDataTypeRegistry();
        std::vector<EntityID> rootEntities;
        EntityCompositionInstantiationManager instantiationManager;

        const EntityID instantiatedRootEntityID = InstantiateEntityComposition(
            ToHandle(targetECS),
            EntityCompositionAssetHandle(compositionAsset),
            parentID,
            instantiationManager,
            dataTypeRegistry,
            rootEntities,
            commandTracker
        );

        TransformHierarchyComponent* transformHierarchy = targetECS.Get().GetComponent<TransformHierarchyComponent>(instantiatedRootEntityID);
        REQUIRE(targetECS.Get().GetEntityCount() == 2);
        REQUIRE(transformHierarchy->parent == parentID);
        REQUIRE(transformHierarchy->children.empty());
    }
}