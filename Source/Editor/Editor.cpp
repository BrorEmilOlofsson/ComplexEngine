#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor.hpp"

#include "Editor/EditorSystem.hpp"

#include "Editor/Core/Tabs/MenuTabWindow.hpp"
#include "Editor/Core/Tabs/MenuTabDefault.hpp"

#include "Editor/PopUps/Help/CameraControlsGuidePopUp.hpp"
#include "Editor/PopUps/Settings/CameraSettingsPopUp.hpp"
#include "Editor/PopUps/Settings/AudioSettingsPopUp.hpp"
#include "Editor/PopUps/Settings/GraphicsSettingsPopUp.hpp"

#include "Editor/PopUps/Editor/SceneHierarchyPopUp.hpp"
#include "Editor/PopUps/Editor/SceneInspectorPopUp.hpp"
#include "Editor/PopUps/Editor/AssetBrowserPopUp.hpp"
#include "Editor/PopUps/Editor/SceneWindowPopUp.hpp"
#include "Editor/PopUps/Editor/EntityCompositionPopUp.hpp"
#include "Editor/FlyScript/NodeScriptingWindow.hpp"
#include "Editor/EditorWindows/CurveEditorWindow.hpp"

#include "Editor/Functions/SceneFileFunctions.hpp"
#include "Editor/Functions/EditorCallbackFunctions.hpp"

#include "Utility/File/FileUtility.hpp"
#include "Engine/Camera/CameraMovement.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Editor/Functions/EditorFunctions.hpp"

namespace Simple
{

    template<DerivedFromPopUpWindow T, typename ... Args>
    inline std::shared_ptr<T> AddPopUpWindow(auto& popUpWindows, const std::string& name, Args&& ... args)
    {
        const std::string tag = std::string("##") + ConvertTypeIndexNameToPrettyName(typeid(T).name());

        std::shared_ptr<T> window = std::make_shared<T>(name, std::forward<Args>(args)...);
        window->SetImGuiTag(tag.c_str());

        popUpWindows.push_back(window);
        return window;
    }

    template<DerivedFromMainMenuTabBase T>
    inline T& AddMenuTab(auto& mainMenuTabs, const std::string& name)
    {
        const std::string tag = ConvertTypeIndexNameToPrettyName(typeid(T).name());

        std::unique_ptr<T> tab = std::make_unique<T>(name, tag.c_str());
        T& ref = *tab.get();

        mainMenuTabs.push_back(std::move(tab));

        return ref;
    }

    static void SetUpDefaultLayout(std::vector<std::shared_ptr<PopUp>>& popUpWindows,
        std::vector<std::unique_ptr<MainMenuTabBase>>& mainMenuTabs,
        Engine& engine,
        EntityCompositionPopUp*& p,
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
        MenuItemPopUp* nodeScriptingPopUpButton = windowsTab.AddPopUp("NodeScript");

        std::shared_ptr<NodeScriptingWindow> nodeScriptingPopUp = AddPopUpWindow<NodeScriptingWindow>(popUpWindows, "NodeScripting Window");

        //std::shared_ptr<GraphicsSettingsPopUp> graphicsSettingPopUp = AddPopUpWindow<GraphicsSettingsPopUp>(popUpWindows, "Graphics Settings");
        std::shared_ptr<AssetBrowserPopUp> assetBrowserPopUp2 = AddPopUpWindow<AssetBrowserPopUp>(popUpWindows, "Asset Browser", nodeScriptingPopUp.get(), &windowsTab, nodeScriptingPopUpButton);
        std::shared_ptr<SceneWindowPopUp> sceneWindowPopUp = AddPopUpWindow<SceneWindowPopUp>(popUpWindows, "Scene");
        std::shared_ptr<EntityCompositionPopUp> entityCompositionPopUp = AddPopUpWindow<EntityCompositionPopUp>(
            popUpWindows,
            "",
            engine.GetOperatingSystem().CreateRenderContext(engine.GetMainWindow().GetClientSize())
        );

        p = entityCompositionPopUp.get();

        {
            const std::vector<std::filesystem::path> scenePaths = FileUtility::GetPathsFromDirectory(std::filesystem::absolute(SIMPLE_DIR_SCENES));
            std::vector<std::string> sceneNames;
            std::ranges::transform(scenePaths, std::back_inserter(sceneNames), [](const auto& path) { return path.string(); });
            sceneLoadSelectable->SetStrings(sceneNames);
        }

        sceneSaveButton->SetCallback(SceneFileFunctions::Save(&engine.GetSceneManager(), &engine.GetDataTypeRegistry()));
        sceneLoadSelectable->SetCallback(SceneFileFunctions::Load(&engine.GetSceneManager()));
        sceneCreateNewButton->SetCallback(SceneFileFunctions::CreateNew(&engine.GetSceneManager()));
        sceneCreateCopyButton->SetCallback(SceneFileFunctions::CreateCopy(&engine.GetSceneManager()));
        sceneReloadButton->SetCallback(SceneFileFunctions::Reload(&engine.GetSceneManager()));
        sceneSetAsActiveButton->SetCallback(SceneFileFunctions::SetAsActive(&engine.GetSceneManager()));

        settingsCameraButton->AddCallback([&isCameraSettingsPopUpActive]()
            {
                isCameraSettingsPopUpActive = !isCameraSettingsPopUpActive;
            });
        settingsGraphicsButton->AddCallback([&isGraphicsSettingsPopUpActive]()
            {
                isGraphicsSettingsPopUpActive = !isGraphicsSettingsPopUpActive;
            });

        editorPopUpButton->AddCallback(EditorCallbacks::SetPopUpActive(assetBrowserPopUp2, &editorPopUpButton->GetIsActiveRef()));
        editorPopUpButton->AddCallback(EditorCallbacks::SetPopUpActive(sceneWindowPopUp, &editorPopUpButton->GetIsActiveRef()));
        editorPopUpButton->AddCallback(EditorCallbacks::SetPopUpActive(entityCompositionPopUp, &editorPopUpButton->GetIsActiveRef()));

        nodeScriptingPopUpButton->AddCallback(EditorCallbacks::SetPopUpActive(nodeScriptingPopUp, &nodeScriptingPopUpButton->GetIsActiveRef()));

        { //TO-DO(v11.4.5): Temp should be refactor
            editorPopUpButton->SetIsActive(true);
            editorPopUpButton->Invoke();
        }
    }

    Editor::Editor(Engine* engine)
        : mECSBuffer(ECSRegistry::Get())
        , mEngine(engine)
    {
        ECSRegistry::Get().RegisterSystem<EditorSystem>();
    }

    void Editor::Init()
    {
        SetUpDefaultLayout(mPopUpWindows, mMainMenuTabs, *mEngine, mEntityCompositionPopUp, mIsCameraSettingsPopUpActive, mIsGraphicsSettingsPopUpActive);

        for (auto& menuTab : mMainMenuTabs)
        {
            menuTab->Init();
        }

        for (auto& window : mPopUpWindows)
        {
            window->Init();
        }

        mEngine->GetSceneManager().AddOnScenePostLoadFunction([this](Scene& scene)
            {
                for (auto& popup : mPopUpWindows)
                {
                    popup->OnSceneLoaded(scene);
                }
            });

        mImGuiStyleManager.Init();
    }

    void Editor::Update()
    {
        PROFILER_FUNCTION(profiler::colors::Amber300);
        const InputState& input = mEngine->GetInputState();
        if (input.IsKeyPressed(eInputKey::F5))
        {
            mEngine->GetMainWindow().ToggleFullScreen();
        }
        if (input.IsKeyPressed(eInputKey::F6))
        {
            mEngine->GetMainWindow().SetSize(Vector2ui(720, 405), false);
        }

        if (input.IsKeyHeld(eInputKey::Ctrl))
        {
            if (input.IsKeyPressed(eInputKey::Z))
            {
                mCommandTracker.UndoCommand();
            }
            else if (input.IsKeyPressed(eInputKey::Y))
            {
                mCommandTracker.RedoCommand();
            }
        }

        Blackboard editorBlackboard;
        editorBlackboard.Insert<Key_IsPlaying>(mEngine->GetSceneManager().IsPlaying());
        editorBlackboard.Insert<Key_CommandTracker>(mCommandTracker);
        editorBlackboard.Insert<Key_ECSBuffer>(mECSBuffer);
        editorBlackboard.Insert<Key_SceneManager>(mEngine->GetSceneManager());
        editorBlackboard.Insert<Key_DeltaTime>(mEngine->GetDeltaTime());
        editorBlackboard.Insert<Key_Editor>(*this);
        editorBlackboard.Insert<Key_WindowView>(mEngine->GetMainWindow());
        editorBlackboard.Insert<Key_AssetManager>(mEngine->GetAssetManager());
        editorBlackboard.Insert<Key_DataTypeRegistry>(mEngine->GetDataTypeRegistry());
        editorBlackboard.Insert<Key_ImGuiStyleManager>(GetImGuiStyleManager());
        editorBlackboard.Insert<Key_GraphicsSettings>(mEngine->GetGraphicsSettings());
        editorBlackboard.Insert<Key_InputState>(mEngine->GetInputState());
        editorBlackboard.Insert<Key_OSView>(mEngine->GetOperatingSystem().GetOS());
        editorBlackboard.Insert<Key_FreeFlyCameraSettings>(mFreeFlyCameraSettings);
        editorBlackboard.Insert<Key_EditorSceneSettings>(mEditorSceneSettings);
        editorBlackboard.Insert<Key_ShowUnitVectorInScene>(mEditorSceneSettings.showUnitVectorInScene);
        editorBlackboard.Insert<Key_EntityCompositionPopUp>(*mEntityCompositionPopUp);

        for (const std::shared_ptr<PopUp> popUp : mPopUpWindows)
        {
            popUp->Update(editorBlackboard);
        }
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
        editorBlackboard.Insert<Key_AssetManager>(mEngine->GetAssetManager());
        editorBlackboard.Insert<Key_DataTypeRegistry>(mEngine->GetDataTypeRegistry());
        editorBlackboard.Insert<Key_ImGuiStyleManager>(GetImGuiStyleManager());
        editorBlackboard.Insert<Key_GraphicsSettings>(mEngine->GetGraphicsSettings());
        editorBlackboard.Insert<Key_InputState>(mEngine->GetInputState());
        editorBlackboard.Insert<Key_IsPlaying>(mEngine->GetSceneManager().IsPlaying());
        editorBlackboard.Insert<Key_OSView>(mEngine->GetOperatingSystem().GetOS());
        editorBlackboard.Insert<Key_EditorSceneSettings>(mEditorSceneSettings);
        editorBlackboard.Insert<Key_ShowUnitVectorInScene>(mEditorSceneSettings.showUnitVectorInScene);
        editorBlackboard.Insert<Key_OperatingSystem>(mEngine->GetOperatingSystem());
        editorBlackboard.Insert<Key_EntityCompositionPopUp>(*mEntityCompositionPopUp);
        editorBlackboard.Insert<Key_FreeFlyCameraSettings>(mFreeFlyCameraSettings);

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

        ShowCameraSettings(mFreeFlyCameraSettings, mIsCameraSettingsPopUpActive);
        ShowGraphicsSettings(mGraphicsSettingsData, mIsGraphicsSettingsPopUpActive, editorBlackboard);
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
    }

    void Editor::OnSceneEndPlay(Scene& scene)
    {
        for (auto& window : mPopUpWindows)
        {
            window->OnSceneEndPlay(scene);
        }
    }
}