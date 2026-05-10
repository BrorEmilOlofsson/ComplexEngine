#pragma once
#include <vector>
#include <memory>

#include "Editor/Core/PopUp.hpp"
#include "Editor/Core/Base/MenuItemTabBase.hpp"
#include "Editor/Command/Core/EditorCommandTracker.hpp"
#include "Engine/Camera/CameraMovement.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Editor/ImGui/ImGuiStyleManager.hpp"
#include "Editor/EditorSceneSettings.hpp"
#include "Editor/EditorWindows/GraphicsSettingsWindow.hpp"
#include "Editor/FlyScript/NodeScriptingWindow.hpp"
#include "Editor/PopUps/Editor/SceneWindowPopUp.hpp"
#include "Editor/PopUps/Editor/AssetBrowserPopUp.hpp"
#include "Editor/EntityCompositionInstantiationManager.hpp"
#include "Editor/EditorWindows/EditorWindowManager.hpp"

namespace CLX
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
        bool mIsCameraSettingsPopUpActive = false;
        bool mIsGraphicsSettingsPopUpActive = false;
        GraphicsSettingsData mGraphicsSettingsData = { };
        EditorWindowManager mWindowManager;
		NodeScriptingWindow mNodeScriptingWindow;
		SceneWindowPopUp mSceneWindow;
		AssetBrowserPopUp mAssetBrowserWindow;
		EntityCompositionInstantiationManager mEntityCompositionInstantiationManager;
	};
}