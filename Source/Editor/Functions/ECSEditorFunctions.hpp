#pragma once
#include <string>
#include <set>
#include <functional>

#include "Engine/ECS/EntityID.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"
#include "Engine/ECS/ECSHandle.hpp"
#include "Engine/Reflection/JsonAny.hpp"

namespace CLX
{
	class Camera;
	class ECS;
	class InputState;
	class EditorCommandTracker;

	using EditorAction = std::function<void(EditorCommandTracker&)>;

	std::size_t GetEntityIndexInParent(const ECS& ecs, const EntityID entityID, const std::vector<EntityID>& rootEntities);

	EntityID CreateEntity(ECS& ecs, std::vector<EntityID>& rootEntities, const EntityID parentID, EditorCommandTracker& commandTracker);
	void DestroyEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);
    void DestroyEntities(ECS& ecs, const std::set<EntityID>& entityIDs, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);
	void DestroyEntityHierarchy(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);
    void DestroyEntityHierarchies(ECS& ecs, const std::set<EntityID>& entityIDs, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);

    EntityID DuplicateEntityHierarchy(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, ChildIndexSetting indexSetting, const class DataTypeRegistry& dataTypeRegistry, EditorCommandTracker& commandTracker);

	void SetEntitySelection(const std::set<EntityID>& entityID, std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker);
	void SetEntitySelection(EntityID entityID, std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker);
    void ClearEntitySelection(std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker);

	void ReorderEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, const std::size_t newIndex, EditorCommandTracker& commandTracker);
	void SetParentEntity(ECS& ecs, const EntityID parentID, const EntityID childID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, ChildIndexSetting indexSetting);

	void ShowEntityAddButtons(ECS& ecs, EntityID& selectedEntityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, const std::string& imGuiTag, EntityID defaultParentID);

	void ShowEntityHierarchy(ECS& ecs, ECS& ecsBuffer, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, const std::string& imGuiTag,
		EntityID& selectedEntityID, const std::set<EntityID>& uneditableEntities, std::string& entitySearchBuffer);

	void ShowEntityHierarchyWithAddButtons(ECS& ecs, ECS& ecsBuffer, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker,
		const std::string& imGuiTag, std::set<EntityID>& selectedEntityID, EntityID defaultParentID, const std::set<EntityID>& uneditableEntities, 
		std::string& entitySearchBuffer, const class DataTypeRegistry& dataTypeRegistry);
	
	[[nodiscard]] std::optional<EditorAction> ShowEntityName(ECS& ecs, const EntityID selectedEntityID, const InputState& input);
	[[nodiscard]] std::vector<EditorAction> ShowEntityComponents(ECS& ecs, EntityID entityID, bool& anyItemActiveLastFrame,
		ECS& ecsBuffer, EntityID& copyEntityID, JsonAny& copiedComponent, EntityCompositionAssetHandle entityCompositionAsset, const Blackboard& blackboard);
	[[nodiscard]] std::optional<EditorAction> ShowEntityAddComponentButtons(ECS& ecs, EntityID entityID, uint32_t& selectedIndex, std::string& searchBuffer, const class DataTypeRegistry& dataTypeRegistry);
	[[nodiscard]] std::vector<EditorAction> ShowEntityInspector(ECS& ecs, EntityID selectedEntityID, bool& anyItemActiveLastFrame,
		ECS& ecsBuffer, EntityID& copyEntityID, uint32_t& selectedComponentPopupIndex, std::string& componentSearchString, JsonAny& copiedComponent,
		EntityCompositionAssetHandle entityCompositionAsset, const Blackboard& blackboard);

	// Returns root entity of instantiated entity composition
	EntityID InstantiateEntityComposition(ECSHandle targetECSHandle, const EntityCompositionAssetHandle& compositionAsset, EntityID parentID, const DataTypeRegistry& dataTypeRegistry,
		std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);

	void TeleportCameraToEntity(const ECS& ecs, EntityID entityID, Camera& camera, bool changeRotation = true, const float offsetDistance = 5.f);
}