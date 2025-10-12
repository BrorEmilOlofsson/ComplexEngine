#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Editor/Tools/TransformEntityTool.hpp"
#include "Editor/PopUps/Editor/SceneHierarchyPopUp.hpp"
#include "Editor/PopUps/Editor/SceneInspectorPopUp.hpp"
#include "Utility/Camera.hpp"
#include "Engine/Camera/CameraMovement.hpp"

namespace Simple
{
	class SceneWindowPopUp final : public PopUp
	{
	public:

		SceneWindowPopUp(const std::string& name);

		void UpdateInternal(const Blackboard& blackboard) override;
		void Render(const Blackboard& blackboard) override;
		void OnSceneLoaded(Scene& scene) override;
		void OnSceneBeginPlay(Scene& scene) override;
		void OnSceneEndPlay(Scene& scene) override;

	private:

		TransformEntityTool mTransformEntityTool;
		SceneHierarchyPopUp mHierarchyPopUp;
		SceneInspectorPopUp mInspectorPopUp;
		Camera mCamera;
	};
}