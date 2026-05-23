#pragma once
#include <string>
#include <set>
#include <functional>

#include "Engine/ECS/EntityID.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"
#include "Engine/ECS/ECSHandle.hpp"
#include "Engine/Math/TransformMode.hpp"
#include "Engine/Math/TransformOperation.hpp"
#include "Editor/Utility/TransformEntityData.hpp"
#include "Editor/Utility/ComponentBufferData.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"

namespace CLX
{

	class Camera;
	class ECS;
	class InputState;
	class EditorCommandTracker;
	class AssetManager;
    class EntityCompositionInstantiationManager;

	using EditorAction = std::function<void(EditorCommandTracker&)>;

	std::size_t GetEntityIndexInParent(const ECS& ecs, const EntityID entityID, const std::vector<EntityID>& rootEntities);

	EntityID CreateEntity(ECS& ecs, std::vector<EntityID>& rootEntities, const EntityID parentID, EditorCommandTracker& commandTracker);
	void DestroyEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);
    void DestroyEntities(ECS& ecs, const std::set<EntityID>& entityIDs, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);
	void DestroyEntityHierarchy(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);
    void DestroyEntityHierarchies(ECS& ecs, const std::set<EntityID>& entityIDs, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);

	EntityID PasteEntityHierarchy(ECS& ecs, const ECS& sourceECS, const EntityID entityID, const EntityID targetParentID, std::vector<EntityID>& rootEntities, IndexVariant childIndex, const class DataTypeRegistry& dataTypeRegistry, EditorCommandTracker& commandTracker);
    EntityID DuplicateEntityHierarchy(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, IndexVariant childIndex, const class DataTypeRegistry& dataTypeRegistry, EditorCommandTracker& commandTracker);

	void SetEntitySelection(const std::set<EntityID>& entityID, std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker);
	void SetEntitySelection(EntityID entityID, std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker);
    void ClearEntitySelection(std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker);

	void ReorderEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, const std::size_t newIndex, EditorCommandTracker& commandTracker);
	void SetParentEntity(ECS& ecs, const EntityID parentID, const EntityID childID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, IndexVariant childIndex);

	void UpdateRootEntities(const ECS& ecs, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);

	void ShowEntityAddButtons(ECS& ecs, EntityID& selectedEntityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, const std::string& imGuiTag, EntityID defaultParentID);

	void ShowEntityHierarchy(ECS& ecs, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, const std::string& imGuiTag,
		EntityID& selectedEntityID, const std::set<EntityID>& uneditableEntities, std::string& entitySearchBuffer);

	void ShowEntityHierarchyWithAddButtons(ECSHandle ecsHandle, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker,
		const std::string& imGuiTag, std::set<EntityID>& selectedEntityID, EntityID defaultParentID, const std::set<EntityID>& uneditableEntities, 
		std::string& entitySearchBuffer, EntityCompositionAssetHandle compositionHandle, EntityCompositionInstantiationManager& compositionInstantiations, const class DataTypeRegistry& dataTypeRegistry, AssetManager& assetManager);
	
	[[nodiscard]] std::optional<EditorAction> ShowEntityName(ECS& ecs, const EntityID selectedEntityID, const InputState& input, bool canChangeName);

	struct ShowEntityComponentsData final
	{
		ECS& ecs;
		EntityID entityID;
		ComponentBufferData& componentBufferData;
		EntityID& copyEntityID;
		std::any& copiedComponent;
		EntityCompositionAssetHandle entityCompositionAsset;
		EntityCompositionInstantiationManager& entityInstantiations;
        const Blackboard& blackboard;
	};

	[[nodiscard]] std::vector<EditorAction> ShowEntityComponents(const ShowEntityComponentsData& data);
	[[nodiscard]] std::optional<EditorAction> ShowEntityAddComponentButtons(ECS& ecs, EntityID entityID, uint32_t& selectedIndex, std::string& searchBuffer, 
		const class DataTypeRegistry& dataTypeRegistry);


	struct ShowEntityInspectorData final
	{
		ECS& ecs;
		EntityID entityID;
		ComponentBufferData& componentBufferData;
		EntityID& copyEntityID;
        uint32_t& selectedComponentPopupIndex;
        std::string& componentSearchString;
        std::any& copiedComponent;
        EntityCompositionAssetHandle entityCompositionAsset;
        EntityCompositionInstantiationManager& entityInstantiations;
        const Blackboard& blackboard;
	};

	[[nodiscard]] std::vector<EditorAction> ShowEntityInspector(const ShowEntityInspectorData& data);
	// Returns root entity of instantiated entity composition
	EntityID InstantiateEntityComposition(ECSHandle targetECSHandle, EntityCompositionAssetHandle compositionAsset, EntityCompositionAssetHandle parentComposition, 
		EntityID parentID, EntityCompositionInstantiationManager& entityInstantiations, const DataTypeRegistry& dataTypeRegistry,
		std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);

	void InstantiateEntityCompositionAndSelectRoot(EntityCompositionAssetHandle target, EntityCompositionAssetHandle toInstantiate, EntityID parentID,
		EntityCompositionInstantiationManager& entityInstantiations, std::vector<EntityID>& rootEntities,
		std::set<EntityID>& selectedEntityIDs, const DataTypeRegistry& dataTypeRegistry, EditorCommandTracker& commandTracker);


	void InstantiateEntityCompositionAndSelectRoot(ECSHandle ecsHandle, EntityCompositionAssetHandle toInstantiate, EntityID parentID,
		EntityCompositionInstantiationManager& entityInstantiations, std::vector<EntityID>& rootEntities,
		std::set<EntityID>& selectedEntityIDs, const DataTypeRegistry& dataTypeRegistry, EditorCommandTracker& commandTracker);

	void ShowEntityImGuizmo(ECS& ecs, EntityID selectedEntityID, eTransformMode transformMode, eTransformOperation transformOperation,
		const Camera& camera, AABB2i renderRect, bool useSnap, float snapValue, int guizmoID, bool applyTransformation,
		TransformEntityData& transformEntityData, EditorCommandTracker& commandTracker);

	void TeleportCameraToEntity(const ECS& ecs, EntityID entityID, Camera& camera, bool changeRotation = true, const float offsetDistance = 5.f);
}