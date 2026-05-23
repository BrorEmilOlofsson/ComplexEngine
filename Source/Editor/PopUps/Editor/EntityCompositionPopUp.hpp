#pragma once
#include <set>
#include <vector>
#include <string>

#include "Engine/Utility/Blackboard.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"
#include "Editor/Utility/ComponentBufferData.hpp"

#include "Editor/Utility/TransformEntityData.hpp"

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
		ComponentBufferData mComponentBufferData;
		EntityID mCopyEntityID;
		std::vector<EntityID> mRootEntities;
		Camera mCamera;
		RenderState mRenderState;
		TransformEntityData mTransformEntityData;
		bool mIsOpen = false;
        std::string mEntitySearchBuffer;
        uint32_t mSelectedComponentPopupIndex = 0;
		std::string mComponentSearchBuffer;
        int mGuizmoID = 0;
		bool mIsWindowFocused = false;
		// Refactor
		inline static int sGuizmoIDCounter = 0;
	};
}