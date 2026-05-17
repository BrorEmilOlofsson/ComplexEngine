#pragma once
#include "Editor/Utility/TransformEntityData.hpp"
#include "Editor/PopUps/Editor/SceneHierarchyPopUp.hpp"
#include "Editor/PopUps/Editor/SceneInspectorPopUp.hpp"
#include "Engine/Utility/Camera.hpp"

namespace CLX
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

		TransformEntityData mTransformEntityData;
		SceneHierarchyPopUp mHierarchyPopUp;
		SceneInspectorPopUp mInspectorPopUp;
		Camera mCamera;
		// TODO: Refactor
		int mGuizmoID = -1;

		static constexpr const char* WindowName = "Scene";
	};
}