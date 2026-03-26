#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Engine/ECS/EntityID.hpp"
#include <vector>
#include <set>

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

		[[nodiscard]] std::set<EntityID>& GetSelectedEntityIDs();
		[[nodiscard]] const std::set<EntityID>& GetSelectedEntityIDs() const;

		[[nodiscard]] std::vector<EntityID>& GetRootEntities();
		[[nodiscard]] const std::vector<EntityID>& GetRootEntities() const;

	private:

		std::set<EntityID> mSelectedEntityIDs;
		EntityID mCopiedEntityID;
		std::vector<EntityID> mRootEntities;
        std::string mEntitySearchBuffer;

		std::set<EntityID> mStoredSelectedEntityIDs;
	};

}