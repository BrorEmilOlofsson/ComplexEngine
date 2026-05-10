#pragma once
#include <set>
#include <vector>
#include <string>

#include "Engine/Utility/Blackboard.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"
#include "Engine/Reflection/JsonAny.hpp"

#include "Editor/Tools/TransformEntityTool.hpp"

namespace CLX
{
	class EntityCompositionPopUp final
	{
	public:

		explicit EntityCompositionPopUp(EntityCompositionAssetHandle assetHandle, RenderContext&& renderContext);
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
		std::set<EntityID> mSelectedEntityIDs;
		bool mAnyItemActiveLastFrame = false;
		EntityID mCopyEntityID;
		std::vector<EntityID> mRootEntities;
		Camera mCamera;
		RenderState mRenderState;
		TransformEntityTool mTransformEntityTool;
		bool mIsOpen = false;
        std::string mEntitySearchBuffer;
        uint32_t mSelectedComponentPopupIndex = 0;
		std::string mComponentSearchBuffer;
        JsonAny mCopiedComponent;
	};
}