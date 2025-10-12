#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Editor/PopUps/Editor/SceneHierarchyPopUp.hpp"
#include "Editor/PopUps/Editor/SceneInspectorPopUp.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Utility/Camera.hpp"
#include "Graphics/RenderState.hpp"
#include "Editor/Tools/TransformEntityTool.hpp"

namespace Simple
{
	class EntityCompositionPopUp final : public PopUp
	{
	public:

		explicit EntityCompositionPopUp(const std::string& windowName, const RenderTargetView& renderTargetView, const DepthStencilViewHandle dsvHandle);

		void UpdateInternal(const Blackboard& blackboard) override;
		void Render(const Blackboard& blackboard) override;

	private:

		EntityComposition mEntityComposition;
		EntityID mSelectedEntityID;
		bool mAnyItemActiveLastFrame = false;
		EntityID mCopyEntityID;
		std::vector<EntityID> mRootEntities;
		Camera mCamera;
		RenderState mRenderState;
		TransformEntityTool mTransformEntityTool;
	};
}