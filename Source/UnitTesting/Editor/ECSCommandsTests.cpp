#include <External/Catch2/catch_amalgamated.hpp>

#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECS/ECSRegistry.hpp"
#include "Engine/ECSEngine/Components/AllEngineComponents.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include "Editor/EntityCompositionInstantiationManager.hpp"

using namespace CLX;

ECSRegistry CreateECSRegistry()
{
    ECSRegistry registry;
    registry.RegisterComponentType<TransformComponent>(eECSComponentTrait::Default);
    registry.RegisterComponentType<NameComponent>(eECSComponentTrait::Default);
    registry.RegisterComponentType<TransformHierarchyComponent>(eECSComponentTrait::Default);
    registry.RegisterComponentType<EntityCompositionInstantiationComponent>(eECSComponentTrait::Default);
    return registry;
}

DataTypeRegistry CreateDataTypeRegistry()
{
    DataTypeRegistry registry;
    registry.RegisterType<TransformComponent>(true);
    registry.RegisterType<NameComponent>(true);
    registry.RegisterType<TransformHierarchyComponent>(true);
    registry.RegisterType<EntityCompositionInstantiationComponent>(true);
    registry.RegisterType<EntityID>(false);
    registry.RegisterMemberVariable(&TransformHierarchyComponent::parent, "Parent", MemberMetaData{});
    registry.RegisterMemberVariable(&TransformHierarchyComponent::children, "Children", MemberMetaData{});
    return registry;
}

TEST_CASE("Create Entities")
{
    EntitySerializationIDGenerator idGenerator;
    ECS ecs(CreateECSRegistry(), idGenerator);
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
    EntitySerializationIDGenerator idGenerator;
    ECS ecs(CreateECSRegistry(), idGenerator);
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
    EntitySerializationIDGenerator idGenerator;
    ECS ecs(CreateECSRegistry(), idGenerator);
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

        EntitySerializationIDGenerator entityIDGenerator;
        EntityCompositionAsset compositionAsset(EntityComposition(ecsManager, ecsManager.CreateECS(CreateECSRegistry(), entityIDGenerator)), std::filesystem::path("path/to/asset"));

        ECSOwningHandle targetECS(ecsManager, ecsManager.CreateECS(CreateECSRegistry()));

        EditorCommandTracker commandTracker;
        const DataTypeRegistry dataTypeRegistry = CreateDataTypeRegistry();
        std::vector<EntityID> rootEntities;
        EntityCompositionInstantiationManager instantiationManager;

        const EntityID instantiatedRootEntityID = InstantiateEntityComposition(
            ToHandle(targetECS),
            EntityCompositionAssetHandle(compositionAsset),
            EntityCompositionAssetHandle::Empty(),
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

        EntitySerializationIDGenerator entityIDGenerator;
        EntityCompositionAsset compositionAsset(EntityComposition(ecsManager, ecsManager.CreateECS(CreateECSRegistry(), entityIDGenerator)), std::filesystem::path("path/to/asset"));

        ECSOwningHandle targetECS(ecsManager, ecsManager.CreateECS(CreateECSRegistry(), entityIDGenerator));

        const EntityID parentID = targetECS.Get().CreateEntity();
        const EntityID child1 = compositionAsset->GetECS().CreateEntity();
        const EntityID child2 = compositionAsset->GetECS().CreateEntity();
        const EntityID child3 = compositionAsset->GetECS().CreateEntity();
        const EntityID child4 = compositionAsset->GetECS().CreateEntity();
        SetParentEntity(compositionAsset->GetECS(), child1, compositionAsset->GetRootEntity(), LastIndex{});
        SetParentEntity(compositionAsset->GetECS(), child2, compositionAsset->GetRootEntity(), LastIndex{});
        SetParentEntity(compositionAsset->GetECS(), child3, compositionAsset->GetRootEntity(), LastIndex{});
        SetParentEntity(compositionAsset->GetECS(), child4, compositionAsset->GetRootEntity(), LastIndex{});

        EditorCommandTracker commandTracker;
        const DataTypeRegistry dataTypeRegistry = CreateDataTypeRegistry();
        std::vector<EntityID> rootEntities;
        EntityCompositionInstantiationManager instantiationManager;

        const EntityID instantiatedRootEntityID = InstantiateEntityComposition(
            ToHandle(targetECS),
            EntityCompositionAssetHandle(compositionAsset),
            EntityCompositionAssetHandle::Empty(),
            parentID,
            instantiationManager,
            dataTypeRegistry,
            rootEntities,
            commandTracker
        );

        TransformHierarchyComponent* transformHierarchy = targetECS.Get().GetComponent<TransformHierarchyComponent>(instantiatedRootEntityID);
        TransformHierarchyComponent* transformHierarchy2 = targetECS.Get().GetComponent<TransformHierarchyComponent>(parentID);
        REQUIRE(targetECS.Get().GetEntityCount() == 6);
        REQUIRE(transformHierarchy->parent == parentID);
        REQUIRE(transformHierarchy->children.size() == 4);
        REQUIRE(transformHierarchy2->children.size() == 1);
        REQUIRE(transformHierarchy2->children[0] == instantiatedRootEntityID);

        for (EntityID childID : transformHierarchy->children)
        {
            TransformHierarchyComponent* childTransformHierarchy = targetECS.Get().GetComponent<TransformHierarchyComponent>(childID);
            REQUIRE(childTransformHierarchy->parent == instantiatedRootEntityID);
            REQUIRE(childTransformHierarchy->children.empty());
        }
    }


    {

        ECSManager ecsManager;
        EntitySerializationIDGenerator entityIDGenerator;

        EntityCompositionAsset compositionAsset1(EntityComposition(ecsManager, ecsManager.CreateECS(CreateECSRegistry(), entityIDGenerator)), std::filesystem::path("path/to/asset"));
        EntityCompositionAsset compositionAsset2(EntityComposition(ecsManager, ecsManager.CreateECS(CreateECSRegistry(), entityIDGenerator)), std::filesystem::path("path/to/asset"));

        const EntityID child1 = compositionAsset1->GetECS().CreateEntity();
        const EntityID child2 = compositionAsset1->GetECS().CreateEntity();
        const EntityID child3 = compositionAsset1->GetECS().CreateEntity();
        const EntityID child4 = compositionAsset1->GetECS().CreateEntity();
        SetParentEntity(compositionAsset1->GetECS(), child1, compositionAsset1->GetRootEntity(), LastIndex{});
        SetParentEntity(compositionAsset1->GetECS(), child2, compositionAsset1->GetRootEntity(), LastIndex{});
        SetParentEntity(compositionAsset1->GetECS(), child3, compositionAsset1->GetRootEntity(), LastIndex{});
        SetParentEntity(compositionAsset1->GetECS(), child4, compositionAsset1->GetRootEntity(), LastIndex{});

        EditorCommandTracker commandTracker;
        const DataTypeRegistry dataTypeRegistry = CreateDataTypeRegistry();
        std::vector<EntityID> rootEntities;
        EntityCompositionInstantiationManager instantiationManager;

        const EntityID instantiatedRootEntityID = InstantiateEntityComposition(
            compositionAsset2->GetECSHandle(),
            EntityCompositionAssetHandle(compositionAsset1),
            EntityCompositionAssetHandle(compositionAsset2),
            compositionAsset2->GetRootEntity(),
            instantiationManager,
            dataTypeRegistry,
            rootEntities,
            commandTracker
        );

        ECS& targetECS = compositionAsset2->GetECS();

        TransformHierarchyComponent* instantiatedRootEntityTransformHierarchy = targetECS.GetComponent<TransformHierarchyComponent>(instantiatedRootEntityID);
        TransformHierarchyComponent* targetRootEntityTransformHierarchy = targetECS.GetComponent<TransformHierarchyComponent>(compositionAsset2->GetRootEntity());
        REQUIRE(targetECS.GetEntityCount() == 6);
        REQUIRE(instantiatedRootEntityTransformHierarchy->parent == compositionAsset2->GetRootEntity());
        REQUIRE(instantiatedRootEntityTransformHierarchy->children.size() == 4);
        REQUIRE(targetRootEntityTransformHierarchy->children.size() == 1);
        REQUIRE(targetRootEntityTransformHierarchy->children[0] == instantiatedRootEntityID);

        for (EntityID childID : instantiatedRootEntityTransformHierarchy->children)
        {
            TransformHierarchyComponent* childTransformHierarchy = targetECS.GetComponent<TransformHierarchyComponent>(childID);
            REQUIRE(childTransformHierarchy->parent == instantiatedRootEntityID);
            REQUIRE(childTransformHierarchy->children.empty());
        }
    }
}