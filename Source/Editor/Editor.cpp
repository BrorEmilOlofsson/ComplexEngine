#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor.hpp"

#include "Editor/Core/Tabs/MenuTabWindow.hpp"
#include "Editor/Core/Tabs/MenuTabDefault.hpp"

#include "Editor/EditorWindows/CameraSettingsWindow.hpp"
#include "Editor/EditorWindows/GraphicsSettingsWindow.hpp"
#include "Editor/EditorWindows/CurveEditorWindow.hpp"

#include "Editor/PopUps/Editor/AssetBrowserPopUp.hpp"
#include "Editor/PopUps/Editor/SceneWindowPopUp.hpp"
#include "Editor/PopUps/Editor/EntityCompositionPopUp.hpp"
#include "Editor/FlyScript/NodeScriptingWindow.hpp"

#include "Editor/Functions/SceneFileFunctions.hpp"
#include "Editor/Functions/EditorCallbackFunctions.hpp"

#include "Engine/Utility/File/FileUtility.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"

namespace CLX
{

    /*template<DerivedFromPopUpWindow T, typename ... Args>
    inline std::shared_ptr<T> AddPopUpWindow(auto& popUpWindows, const std::string& name, Args&& ... args)
    {
        const std::string tag = std::string("##") + ConvertTypeIndexNameToPrettyName(typeid(T).name());

        std::shared_ptr<T> window = std::make_shared<T>(name, std::forward<Args>(args)...);
        window->SetImGuiTag(tag.c_str());

        popUpWindows.push_back(window);
        return window;
    }*/

    template<DerivedFromMainMenuTabBase T>
    inline T& AddMenuTab(auto& mainMenuTabs, const std::string& name)
    {
        const std::string tag = ConvertTypeIndexNameToPrettyName(typeid(T).name());

        std::unique_ptr<T> tab = std::make_unique<T>(name, tag.c_str());
        T& ref = *tab.get();

        mainMenuTabs.push_back(std::move(tab));

        return ref;
    }

    static void SetUpDefaultLayout(
        std::vector<std::unique_ptr<MainMenuTabBase>>& mainMenuTabs,
        Engine& engine,
        bool& isCameraSettingsPopUpActive,
        bool& isGraphicsSettingsPopUpActive)
    {
        MenuTabDefault& sceneTab = AddMenuTab<MenuTabDefault>(mainMenuTabs, "Scene");
        MenuTabWindow& windowsTab = AddMenuTab<MenuTabWindow>(mainMenuTabs, "Windows");
        MenuTabDefault& settingsTab = AddMenuTab<MenuTabDefault>(mainMenuTabs, "Settings");

        MenuItemButton* sceneSaveButton = sceneTab.AddButton("Save");
        MenuItemSelectable* sceneLoadSelectable = sceneTab.AddSelectable("Load");
        MenuItemMenu* sceneCreateMenu = sceneTab.AddMenu("Create");
        MenuItemButton* sceneReloadButton = sceneTab.AddButton("Reload");
        MenuItemButton* sceneSetAsActiveButton = sceneTab.AddButton("Set As Active");

        MenuItemButton* sceneCreateNewButton = sceneCreateMenu->AddChild<MenuItemButton>("New");
        MenuItemButton* sceneCreateCopyButton = sceneCreateMenu->AddChild<MenuItemButton>("Copy");

        MenuItemPopUp* settingsCameraButton = settingsTab.AddPopUp("Camera");
        MenuItemPopUp* settingsGraphicsButton = settingsTab.AddPopUp("Graphics");
        settingsTab.AddButton("Save layout")->SetCallback([]()
            {
                std::filesystem::path path = SIMPLE_DIR_DEPENDENCIES_SETTINGS;
                path /= "imgui.ini";
                std::string strPath = std::filesystem::absolute(path).string();
                ImGui::SaveIniSettingsToDisk(strPath.c_str());
            });

        MenuItemPopUp* editorPopUpButton = windowsTab.AddPopUp("Editor");

        {
            const std::vector<std::filesystem::path> scenePaths = FileUtility::GetPathsFromDirectory(std::filesystem::absolute(SIMPLE_DIR_SCENES));
            std::vector<std::string> sceneNames;
            std::ranges::transform(scenePaths, std::back_inserter(sceneNames), [](const auto& path) { return path.string(); });
            sceneLoadSelectable->SetStrings(sceneNames);
        }

        sceneSaveButton->SetCallback(SceneFileFunctions::Save(&engine.GetSceneManager(), &engine.GetDataTypeRegistry()));
        sceneLoadSelectable->SetCallback(SceneFileFunctions::Load(&engine.GetSceneManager(), engine.GetAssetManager()));
        sceneCreateNewButton->SetCallback(SceneFileFunctions::CreateNew(&engine.GetSceneManager(), engine.GetAssetManager()));
        sceneCreateCopyButton->SetCallback(SceneFileFunctions::CreateCopy(&engine.GetSceneManager(), engine.GetAssetManager()));
        sceneReloadButton->SetCallback(SceneFileFunctions::Reload(&engine.GetSceneManager(), engine.GetAssetManager()));
        sceneSetAsActiveButton->SetCallback(SceneFileFunctions::SetAsActive(&engine.GetSceneManager()));

        settingsCameraButton->AddCallback([&isCameraSettingsPopUpActive]()
            {
                isCameraSettingsPopUpActive = !isCameraSettingsPopUpActive;
            });
        settingsGraphicsButton->AddCallback([&isGraphicsSettingsPopUpActive]()
            {
                isGraphicsSettingsPopUpActive = !isGraphicsSettingsPopUpActive;
            });

        {
            editorPopUpButton->SetIsActive(true);
            editorPopUpButton->Invoke();
        }
    }

    Editor::Editor(Engine* engine)
        : mECSBuffer(engine->GetECSRegistry())
        , mEngine(engine)
    {
    }

    void Editor::Init()
    {
        SetUpDefaultLayout(
            mMainMenuTabs, *
            mEngine,
            mIsCameraSettingsPopUpActive, 
            mIsGraphicsSettingsPopUpActive
            //mSceneWindow
        );

        mEntityCompositionWindow = std::make_unique<EntityCompositionPopUp>(
            mEngine->GetOperatingSystem().GetGraphicsFoundation().CreateRenderContext(mEngine->GetMainWindow().GetClientSize())
        );

        for (auto& menuTab : mMainMenuTabs)
        {
            menuTab->Init();
        }

        for (auto& window : mPopUpWindows)
        {
            window->Init();
        }

        mNodeScriptingWindow.Init();

        mEngine->GetSceneManager().RegisterOnSceneLoadedFunction([this](Scene& scene)
            {
                mSceneWindow.OnSceneLoaded(scene);
                for (auto& popup : mPopUpWindows)
                {
                    popup->OnSceneLoaded(scene);
                }

                mNodeScriptingWindow.OnSceneLoaded(scene);
            });

        mImGuiStyleManager.Init();
    }

    void HandleWindowResizing(const InputState& input, const WindowView& windowView)
    {
        if (input.IsKeyPressed(eInputKey::F5))
        {
            windowView.ToggleFullScreen();
        }
        if (input.IsKeyPressed(eInputKey::F6))
        {
            windowView.SetSize(Dimension2u(720, 405), false);
        }
    }

    void HandleCommandInputs(const InputState& input, EditorCommandTracker& commandTracker)
    {
        if (input.IsKeyHeld(eInputKey::Ctrl))
        {
            if (input.IsKeyPressed(eInputKey::Z))
            {
                commandTracker.UndoCommand();
            }
            else if (input.IsKeyPressed(eInputKey::Y))
            {
                commandTracker.RedoCommand();
            }
        }
    }

    void Editor::Update()
    {
        PROFILER_FUNCTION(profiler::colors::Amber300);
        const InputState& input = mEngine->GetInputState();


        HandleWindowResizing(input, mEngine->GetMainWindow());
        HandleCommandInputs(input, mCommandTracker);
        

        Blackboard editorBlackboard;
        editorBlackboard.Insert<Key_IsPlaying>(mEngine->GetSceneManager().IsPlaying());
        editorBlackboard.Insert<Key_CommandTracker>(mCommandTracker);
        editorBlackboard.Insert<Key_ECSBuffer>(mECSBuffer);
        editorBlackboard.Insert<Key_SceneManager>(mEngine->GetSceneManager());
        editorBlackboard.Insert<Key_DeltaTime>(mEngine->GetDeltaTime());
        editorBlackboard.Insert<Key_Editor>(*this);
        editorBlackboard.Insert<Key_WindowView>(mEngine->GetMainWindow());
        editorBlackboard.Insert<Key_AssetManager>(*mEngine->GetAssetManager().lock());
        editorBlackboard.Insert<Key_DataTypeRegistry>(mEngine->GetDataTypeRegistry());
        editorBlackboard.Insert<Key_ECSRegistry>(mEngine->GetECSRegistry());
        editorBlackboard.Insert<Key_ImGuiStyleManager>(GetImGuiStyleManager());
        editorBlackboard.Insert<Key_GraphicsSettings>(mEngine->GetGraphicsSettings());
        editorBlackboard.Insert<Key_InputState>(mEngine->GetInputState());
        editorBlackboard.Insert<Key_OperatingSystem>(mEngine->GetOperatingSystem());
        editorBlackboard.Insert<Key_FreeFlyCameraSettings>(mFreeFlyCameraSettings);
        editorBlackboard.Insert<Key_EditorSceneSettings>(mEditorSceneSettings);
        editorBlackboard.Insert<Key_ShowUnitVectorInScene>(mEditorSceneSettings.showUnitVectorInScene);
        editorBlackboard.Insert<Key_EntityCompositionPopUp>(*mEntityCompositionWindow);
        editorBlackboard.Insert<Key_NodeScriptingWindow>(mNodeScriptingWindow);

        for (const std::shared_ptr<PopUp> popUp : mPopUpWindows)
        {
            popUp->Update(editorBlackboard);
        }

        mSceneWindow.UpdateInternal(editorBlackboard);
        mAssetBrowserWindow.Update(editorBlackboard);
        mEntityCompositionWindow->Update(editorBlackboard);
    }

    void Editor::Render()
    {
        PROFILER_FUNCTION(profiler::colors::Amber300);
        Blackboard editorBlackboard;

        editorBlackboard.Insert<Key_CommandTracker>(mCommandTracker);
        editorBlackboard.Insert<Key_ECSBuffer>(mECSBuffer);
        editorBlackboard.Insert<Key_SceneManager>(mEngine->GetSceneManager());
        editorBlackboard.Insert<Key_DeltaTime>(mEngine->GetDeltaTime());
        editorBlackboard.Insert<Key_Editor>(*this);
        editorBlackboard.Insert<Key_WindowView>(mEngine->GetMainWindow());
        editorBlackboard.Insert<Key_AssetManager>(*mEngine->GetAssetManager().lock());
        editorBlackboard.Insert<Key_DataTypeRegistry>(mEngine->GetDataTypeRegistry());
        editorBlackboard.Insert<Key_ECSRegistry>(mEngine->GetECSRegistry());
        editorBlackboard.Insert<Key_ImGuiStyleManager>(GetImGuiStyleManager());
        editorBlackboard.Insert<Key_GraphicsSettings>(mEngine->GetGraphicsSettings());
        editorBlackboard.Insert<Key_InputState>(mEngine->GetInputState());
        editorBlackboard.Insert<Key_IsPlaying>(mEngine->GetSceneManager().IsPlaying());
        editorBlackboard.Insert<Key_EditorSceneSettings>(mEditorSceneSettings);
        editorBlackboard.Insert<Key_ShowUnitVectorInScene>(mEditorSceneSettings.showUnitVectorInScene);
        editorBlackboard.Insert<Key_OperatingSystem>(mEngine->GetOperatingSystem());
        editorBlackboard.Insert<Key_EntityCompositionPopUp>(*mEntityCompositionWindow);
        editorBlackboard.Insert<Key_FreeFlyCameraSettings>(mFreeFlyCameraSettings);
        editorBlackboard.Insert<Key_NodeScriptingWindow>(mNodeScriptingWindow);

        for (auto& tab : mMainMenuTabs)
        {
            tab->Render();
        }

        for (auto& popUp : mPopUpWindows)
        {
            if (popUp->IsActive())
            {
                popUp->Render(editorBlackboard);
            }
        }

        ShowCameraSettingsWindow(mFreeFlyCameraSettings, mIsCameraSettingsPopUpActive);
        ShowGraphicsSettingsWindow(mGraphicsSettingsData, mIsGraphicsSettingsPopUpActive, editorBlackboard);
        mSceneWindow.Render(editorBlackboard);
        mAssetBrowserWindow.Render(editorBlackboard);
        mEntityCompositionWindow->Render(editorBlackboard);
        mNodeScriptingWindow.Render(editorBlackboard);
        static CubicBezierCurve2f demoCurve;
        static bool curveEditorActive = true;
        ShowCurveWindow(demoCurve, curveEditorActive, editorBlackboard);
    }

    void Editor::OnSceneBeginPlay(Scene& scene)
    {
        for (auto& window : mPopUpWindows)
        {
            window->OnSceneBeginPlay(scene);
        }

        mSceneWindow.OnSceneBeginPlay(scene);
    }

    void Editor::OnSceneEndPlay(Scene& scene)
    {
        for (auto& window : mPopUpWindows)
        {
            window->OnSceneEndPlay(scene);
        }

        mSceneWindow.OnSceneEndPlay(scene);
    }
}