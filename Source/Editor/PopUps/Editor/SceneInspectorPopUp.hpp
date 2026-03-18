#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Engine/ECS/EntityID.hpp"

namespace CLX
{
	class SceneHierarchyPopUp;

	class SceneInspectorPopUp final : public PopUp
	{
	public:

		SceneInspectorPopUp(const std::string& name, SceneHierarchyPopUp* sceneHierarchyPopUp, class Camera* sceneCamera);

		void Render(const Blackboard& blackboard) override;

	private:

		SceneHierarchyPopUp* mHierarchyPopUp = nullptr;

		bool mAnyItemActiveLastFrame = false;
		EntityID mCopyEntityID;
		class Camera* mSceneCamera = nullptr;

		std::function<void(EntityID)> mEntitySelectedCallback;
	};
}