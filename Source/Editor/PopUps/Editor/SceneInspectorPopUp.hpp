#pragma once
#include <functional>

#include "Editor/Core/PopUp.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Editor/Utility/ComponentBufferData.hpp"

#include "Engine/Reflection/JsonAny.hpp"

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

		ComponentBufferData mComponentBufferData;
		EntityID mCopyEntityID;
		class Camera* mSceneCamera = nullptr;

		std::function<void(EntityID)> mEntitySelectedCallback;

        uint32_t mSelectedComponentPopupIndex = 0;
        std::string mComponentSearchBuffer;
	};
}