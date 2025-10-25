#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Editor/PopUps/Editor/SceneHierarchyPopUp.hpp"
#include "Editor/PopUps/Editor/SceneInspectorPopUp.hpp"
#include "Utility/Camera.hpp"
#include "Graphics/RenderState.hpp"
#include "Editor/Tools/TransformEntityTool.hpp"
#include "Utility/Asset/EntityCompositionAsset.hpp"

namespace Simple
{
	class EntityCompositionPopUp final : public PopUp
	{
	public:

		explicit EntityCompositionPopUp(const std::string& name, RenderContext&& renderContext);

		void UpdateInternal(const Blackboard& blackboard) override;
		void Render(const Blackboard& blackboard) override;

		EntityCompositionAssetHandle GetCompositionAsset()
		{
			return mEntityCompositionAsset;
		}

		void SetCompositionAsset(EntityCompositionAssetHandle asset);

	private:

		EntityCompositionAssetHandle mEntityCompositionAsset;
		EntityID mSelectedEntityID;
		bool mAnyItemActiveLastFrame = false;
		EntityID mCopyEntityID;
		std::vector<EntityID> mRootEntities;
		Camera mCamera;
		RenderState mRenderState;
		TransformEntityTool mTransformEntityTool;
	};
}