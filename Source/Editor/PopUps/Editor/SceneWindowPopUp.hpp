#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Editor/Tools/TransformEntityTool.hpp"
#include "Editor/PopUps/Editor/SceneHierarchyPopUp.hpp"
#include "Editor/PopUps/Editor/SceneInspectorPopUp.hpp"
#include "Utility/Camera.hpp"

namespace Simple
{
	class SceneWindowPopUp final
	{
	public:

		SceneWindowPopUp();

		void UpdateInternal(const Blackboard& blackboard);
		void Render(const Blackboard& blackboard);
		void OnSceneLoaded(Scene& scene);
		void OnSceneBeginPlay(Scene& scene);
		void OnSceneEndPlay(Scene& scene);

	private:

		TransformEntityTool mTransformEntityTool;
		SceneHierarchyPopUp mHierarchyPopUp;
		SceneInspectorPopUp mInspectorPopUp;
		Camera mCamera;

		static constexpr const char* WindowName = "Scene";
	};
}