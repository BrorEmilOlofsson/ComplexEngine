#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Engine/ECS/EntityID.hpp"
#include <vector>

namespace CLX
{

	class SceneHierarchyPopUp final : public PopUp
	{
	public:

		explicit SceneHierarchyPopUp(const std::string& name);

		void UpdateInternal(const Blackboard& blackboard) override;
		void Render(const Blackboard& blackboard) override;

		void OnSceneLoaded(Scene& scene) override;
		void OnSceneBeginPlay(Scene& scene) override;
		void OnSceneEndPlay(Scene& scene) override;

		EntityID& GetSelectedEntityID();
		EntityID GetSelectedEntityID() const;

		std::vector<EntityID>& GetRootEntities();
		const std::vector<EntityID>& GetRootEntities() const;

	private:

		EntityID mSelectedEntityID;
		EntityID mCopiedEntityID;
		std::vector<EntityID> mRootEntities;
        std::string mEntitySearchBuffer;

		EntityID mStoredSelectedEntityID;
	};

}