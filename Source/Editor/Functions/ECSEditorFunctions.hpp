#pragma once
#include "Engine/ECS/EntityID.hpp"
#include "Utility/Blackboard.hpp"
#include <string>
#include <set>

namespace Simple
{
	class Camera;
	class ECS;

	class EditorCommandTracker;

	EntityID CreateEntity(ECS& ecs, std::vector<EntityID>& rootEntities, const EntityID parentID, EditorCommandTracker& commandTracker);
	void DestroyEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);
	void SelectEntity(EntityID entityID, EntityID& selectedEntityID, EditorCommandTracker& commandTracker);

	void SetParentEntity(ECS& ecs, const EntityID parentID, const EntityID childID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);

	[[nodiscard]] std::vector<EntityID> GetRootEntities(const ECS& ecs);

	void ShowEntityAddButtons(ECS& ecs, EntityID& selectedEntityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, const std::string& aImGuiTag);
	void ShowEntityHierarchy(ECS& ecs, ECS& ecsBuffer, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, const std::string& imGuiTag,
		EntityID& selectedEntityID);
	void ShowEntityHierarchyWithAddButtons(ECS& ecs, ECS& ecsBuffer, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker,
		const std::string& imGuiTag, EntityID& selectedEntityID);
	void ShowEntityComponents(ECS& ecs, EntityID entityID, bool& anyItemActiveLastFrame,
		ECS& ecsBuffer, EntityID& copyEntityID, const Blackboard& blackboard, EditorCommandTracker& commandTracker);
	void ShowEntityAddComponentButtons(ECS& ecs, EntityID entityID, const class DataTypeRegistry& dataTypeRegistry, EditorCommandTracker& commandTracker);
	void ShowEntityInspector(ECS& ecs, EntityID selectedEntityID, bool& anyItemActiveLastFrame,
		ECS& ecsBuffer, EntityID& copyEntityID, const Blackboard& blackboard, EditorCommandTracker& commandTracker);


	void TeleportCameraToEntity(const ECS& ecs, EntityID entityID, Camera& camera, bool changeRotation = true, const float offsetDistance = 5.f);
}