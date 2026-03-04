#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Editor/PopUps/Editor/SceneHierarchyPopUp.hpp"
#include "Editor/PopUps/Editor/SceneInspectorPopUp.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Editor/Tools/TransformEntityTool.hpp"
#include "Engine/Asset/EntityCompositionAsset.hpp"

namespace Simple
{
	class EntityCompositionPopUp final
	{
	public:

		explicit EntityCompositionPopUp(RenderContext&& renderContext);

		void Update(const Blackboard& blackboard);
		void Render(const Blackboard& blackboard);

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
		bool mIsOpen = false;
		static constexpr const char* PopUpName = "Entity Composition Viewer";
	};
}