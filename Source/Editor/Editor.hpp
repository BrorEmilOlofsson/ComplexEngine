#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Editor/Core/Base/MenuItemTabBase.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"
#include "Engine/Camera/CameraMovement.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Editor/ImGui/ImGuiStyleManager.hpp"
#include "Editor/EditorSceneSettings.hpp"
#include "Editor/PopUps/Settings/GraphicsSettingsPopUp.hpp"
#include <vector>
#include <memory>

namespace Simple
{

	class Editor final
	{
	public:

		explicit Editor(class Engine* engine);

		void Init();
		void Update();
		void Render();

		void OnSceneBeginPlay(class Scene& scene);
		void OnSceneEndPlay(class Scene& scene);

		ImGuiStyleManager& GetImGuiStyleManager() { return mImGuiStyleManager; }

	private:

		ECS mECSBuffer;
		std::vector<std::shared_ptr<PopUp>> mPopUpWindows;
		std::vector<std::unique_ptr<MainMenuTabBase>> mMainMenuTabs;
		EditorCommandTracker mCommandTracker;
		class Engine* mEngine = nullptr;
		ImGuiStyleManager mImGuiStyleManager;
		FreeFlyCameraSettings mFreeFlyCameraSettings;
		EditorSceneSettings mEditorSceneSettings;
		EntityCompositionPopUp* mEntityCompositionPopUp = nullptr;
        bool mIsCameraSettingsPopUpActive = false;
        bool mIsGraphicsSettingsPopUpActive = false;
        GraphicsSettingsData mGraphicsSettingsData = { };
	};
}