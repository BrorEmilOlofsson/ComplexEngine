#include "Engine/Precompiled/EnginePch.hpp"
#include "Engine/Engine.hpp"
#include "Engine/ECSEngine/Systems/EngineECSSystems.hpp"
#include "Engine/ECSEngine/Components/AllEngineComponents.hpp"
#include "Engine/Utility/File/FileUtility.hpp"
#include "Engine/Reflection/EngineReflectionRegistration.hpp"
#include "Engine/Utility/GraphicsBufferData.hpp"
#include "Engine/Utility/EngineDirectories.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Engine/ECS/ECSSerializer.hpp"
#include "Engine/Scene/SceneLoader.hpp"
#include <External/nlohmann/json.hpp>
#include <fstream>

namespace CLX
{

    Engine::Engine(OperatingSystem&& operatingSystem)
        : mOperatingSystem(std::move(operatingSystem))
        , mAssetManager(std::make_shared<AssetManager>())
        , mBlackboard(std::make_shared<Blackboard>())
        , mGraphicsSettings(std::make_shared<GraphicsSettings>())
        , mSceneManager(mAssetManager)
    {
        RegisterEngineComponents();


        mECSRegistry.RegisterSystem<RotatingMovementSystem>();
        mECSRegistry.RegisterSystem<RenderSystem>();
        mECSRegistry.RegisterSystem<RenderLightSystem>();
        mECSRegistry.RegisterSystem<CameraSystem>();
        mECSRegistry.RegisterSystem<AnimationSystem>();
        mECSRegistry.RegisterSystem<NavmeshSystem>();
        mECSRegistry.RegisterSystem<DebugShapeSystem>();

        TypeRegistration::ExecuteRegistrations(mDataTypeRegistry, mECSRegistry);


        mDataTypeRegistry.Assert();
        mOperatingSystem.GetGraphicsFoundation().SetAssetManager(mAssetManager);
        mOperatingSystem.GetGraphicsFoundation().SetGraphicsSettings(mGraphicsSettings);

        std::shared_ptr<Blackboard> blackboard = mBlackboard;
        ECSRegistry* ecsRegistry = &mECSRegistry;
        auto a = [blackboard, ecsRegistry](const std::filesystem::path& path)
            {
                EntityComposition entityComposition(*ecsRegistry);
                LoadEntityComposition(path, entityComposition, *blackboard);
                return EntityCompositionAsset(std::move(entityComposition), path);
            };
        mAssetManager->GetAssetLoader().SetEntityCompositionLoader(a);

        mBlackboard->Insert<Key_ECSRegistry>(mECSRegistry);

    }

    void LoadGraphicsSettings(bool& vSync)
    {
        const std::filesystem::path filename = std::filesystem::absolute(SIMPLE_SETTINGS_GAME);

        std::ifstream file(filename);
        if (!file.is_open())
        {
            return;
        }

        const nlohmann::json json = nlohmann::json::parse(file);
        file.close();

        vSync = json["Game_Settings"]["VSync"];
    }

    void Engine::Init()
    {
        mECSRegistry.SetBlackboard(mBlackboard);

        mBlackboard->Insert<Key_GraphicsSettings>(*mGraphicsSettings);
        mBlackboard->Insert<Key_AssetManager>(*mAssetManager);
        mBlackboard->Insert<Key_SceneManager>(mSceneManager);
        mBlackboard->Insert<Key_DataTypeRegistry>(mDataTypeRegistry);
        mBlackboard->Insert<Key_InputState>(mInputState);

        mAssetManager->GetAssetLoader().SetSceneLoader([this](const std::filesystem::path& path)
            {
                Scene scene(mBlackboard);
                SceneLoader::LoadScene(scene, path, *mBlackboard);
                
                return SceneAsset(std::move(scene), path);
            });

        mAssetManager->LoadAssets();
        mOperatingSystem.Init();
        mMainWindow = mOperatingSystem.MakeWindow(Dimension2u(1600, 900), L"SimpleEngine");

        //mAudioManager.Init();
        mFrameTimer.Start();
        mTotalTimer.Start();

        mNodeScript.Init();

        CheckAndCopySettingsFiles();
        LoadSettingsFromJson();
        LoadGraphicsSettings(mGraphicsSettings->vSync);
        mOperatingSystem.LoadCursors(std::string(Directory::Assets) + std::string("Cursors"));

        mOperatingSystem.GetWindow(mMainWindow).Show();
    }


    std::filesystem::path LoadSceneSettingsDefaultScene()
    {
        const std::filesystem::path filename = std::filesystem::absolute(SIMPLE_SETTINGS_GAME);

        std::ifstream file(filename);
        assert(file.is_open() && "Failed To Open File");

        const nlohmann::json jsonData = nlohmann::json::parse(file);
        const nlohmann::json gameSettings = jsonData["Game_Settings"];
        file.close();

        const std::string defaultSceneRelativePathStr = gameSettings["Start_Scene_Path"];
        const std::filesystem::path sceneRelativePath = std::filesystem::path(defaultSceneRelativePathStr);
        const std::filesystem::path sceneFilePath = GetAbsoluteAssetPath(sceneRelativePath);
        std::ifstream sceneFile(sceneFilePath);

        if (!sceneFile.is_open())
        {
            ASSERT("Failed to open default scene file at path");
        }

        return sceneRelativePath;
    }

    void Engine::LateInit()
    {
        std::filesystem::path sceneFilePath = LoadSceneSettingsDefaultScene();

        SceneAssetHandle defaultScene = mAssetManager->GetScene(sceneFilePath);
        if (!defaultScene)
        {
            mAssetManager->CreateScene(std::filesystem::path(SIMPLE_DIR_SCENES) / (std::filesystem::path(std::wstring(L"DefaultScene") + std::wstring(AssetExtensions::Scene))));
        }
        mSceneManager.ChangeSceneDirectly(defaultScene);

        //mSceneManager.Init(mBlackboard);

        RenderContext r = mOperatingSystem.GetGraphicsFoundation().CreateRenderContext(mOperatingSystem.GetWindow(mMainWindow).GetClientSize());
        mSceneManager.GetActiveScene()->GetRenderState().SetRenderContext(std::move(r));

#ifndef _EDITOR
        mSceneManager.BeginPlay();
#endif
    }

    void Engine::Shutdown()
    {
        mOperatingSystem.Shutdown();
    }

    bool Engine::BeginFrame()
    {
        PROFILER_FUNCTION(profiler::colors::DeepOrange);
        mFrameTimer.Update();

        GraphicsBufferData bufferData;
        bufferData.frameTimer = mFrameTimer;
        bufferData.totalTimer = mTotalTimer;
        mOperatingSystem.BeginFrame(bufferData);

        const WindowFrameBuffer& windowFrameBuffer = mOperatingSystem.GetWindow(mMainWindow).GetFrameBuffer();
        if (windowFrameBuffer.hasQuit)
        {
            return false;
        }

        mInputState = mOperatingSystem.GetInputState()/*.GetWindow(mMainWindow).GetInputState()*/;
        mInputManager.Update(mInputState);

        if (!windowFrameBuffer.droppedFiles.empty())
        {
            FileUtility::CopyFiles(windowFrameBuffer.droppedFiles, mCurrentDropPath);
        }

        mSceneManager.GetActiveScene()->BeginFrame(mOperatingSystem.GetWindow(mMainWindow).GetClientSize(), mOperatingSystem.GetCursorScreenPosition());

        return !mShouldExit.load();
    }

    static void SyncToFPSCap(const std::chrono::steady_clock::time_point& lastTimePoint, const unsigned int fpsCap)
    {
        PROFILER_FUNCTION(profiler::colors::LightGreenA100);
        if (fpsCap != 0)
        {
            const auto frameDuration = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                std::chrono::duration<double>(1.0 / fpsCap)
            );

            const auto targetTimepoint = lastTimePoint + frameDuration;
            while (std::chrono::high_resolution_clock::now() < targetTimepoint)
            {
                // Busy-wait until the target frame time is reached
            }
        }
    }

    void Engine::EndFrame()
    {
        PROFILER_FUNCTION(profiler::colors::Indigo800);
        RenderContext* renderContext = nullptr;
#ifndef _EDITOR
        renderContext = mSceneManager.GetCurrentScene().GetRenderState().GetRenderContext();
#endif
        mOperatingSystem.EndFrame(renderContext);
        if (!mGraphicsSettings->vSync)
        {
            SyncToFPSCap(mFrameTimer.GetLastTimepoint(), mGraphicsSettings->fPSCap);
        }
    }

    void Engine::Update()
    {
        PROFILER_FUNCTION(profiler::colors::Cyan700);

        const float deltaTimeCapped = Min(mFrameTimer.GetDeltaTime().count(), 0.1f);
        mSceneManager.GetActiveScene()->GetRenderState().SetAmbientLight(AmbientLight{});
        mBlackboard->Insert<Key_DeltaTime>(deltaTimeCapped);
        mSceneManager.Update(deltaTimeCapped);
        //mAudioManager.Update();
    }

    void Engine::Render()
    {
        PROFILER_FUNCTION(profiler::colors::Cyan700);

        mSceneManager.GetActiveScene()->Render();

#ifndef _EDITOR
        mSceneManager.GetActiveScene()->GetRenderState().SetRenderRect(AABB2i::CreateFromDefaultAndExtent(Vector2i(mOperatingSystem.GetWindow(mMainWindow).GetClientSize())));
#endif
        mOperatingSystem.GetGraphicsFoundation().Render(mSceneManager.GetActiveScene()->GetRenderState());
    }

    WindowView Engine::GetMainWindow()
    {
        return mOperatingSystem.GetWindow(mMainWindow);
    }

    DataTypeRegistry& Engine::GetDataTypeRegistry()
    {
        return mDataTypeRegistry;
    }

    const DataTypeRegistry& Engine::GetDataTypeRegistry() const
    {
        return mDataTypeRegistry;
    }

    GraphicsFoundation& Engine::GetGraphicsFoundation()
    {
        return mOperatingSystem.GetGraphicsFoundation();
    }

    const GraphicsFoundation& Engine::GetGraphicsFoundation() const
    {
        return mOperatingSystem.GetGraphicsFoundation();
    }

    void Engine::SetShouldExit(bool shouldExit)
    {
        return mShouldExit.store(shouldExit);
    }

    void Engine::SetCurrentDropPath(const std::filesystem::path& path)
    {
        mCurrentDropPath = path;
    }

    void Engine::LoadSettingsFromJson()
    {
        const std::filesystem::path filename = std::filesystem::absolute(SIMPLE_SETTINGS_GAME);

        std::ifstream file(filename);
        assert(file.is_open() && "Failed To Open File");

        const nlohmann::json json = nlohmann::json::parse(file);
        file.close();

        /*const nlohmann::json& windowSizeJson = json["Game_Settings"]["Window_Size"];
        const nlohmann::json& resolutionJson = json["Game_Settings"]["Resolution"];

        Dimension2u windowSize;
        windowSize. = windowSizeJson["x"];
        windowSize.y = windowSizeJson["y"];

        Dimension2u resolution;
        resolution.x = resolutionJson["x"];
        resolution.y = resolutionJson["y"];*/
    }

    void Engine::CheckAndCopySettingsFiles()
    {
        const std::filesystem::path binSettings = std::filesystem::absolute(SIMPLE_DIR_SETTINGS);
        const std::filesystem::path dependenciesSettings = std::filesystem::absolute(SIMPLE_DIR_DEPENDENCIES_SETTINGS);
        const std::filesystem::path forceDependenciesSettings = std::filesystem::absolute(SIMPLE_DIR_DEPENDENCIES_FORCE);

        std::vector<std::filesystem::path> binSettingsFileNames;
        std::vector<std::filesystem::path> dependenciesSettingsFileNames;
        std::vector<std::filesystem::path> forceDependenciesSettingsFileNames;

        for (const auto& entry : std::filesystem::directory_iterator(binSettings))
        {
            if (std::filesystem::is_regular_file(entry.path()))
            {
                binSettingsFileNames.push_back(entry.path().filename());
            }
        }

        for (const auto& entry : std::filesystem::directory_iterator(dependenciesSettings))
        {
            if (std::filesystem::is_regular_file(entry.path()))
            {
                dependenciesSettingsFileNames.push_back(entry.path().filename());
            }
        }

        for (const auto& entry : std::filesystem::directory_iterator(forceDependenciesSettings))
        {
            if (std::filesystem::is_regular_file(entry.path()))
            {
                forceDependenciesSettingsFileNames.push_back(entry.path().filename());
            }
        }

        std::vector<std::filesystem::path> missingFileNames;

        std::sort(dependenciesSettingsFileNames.begin(), dependenciesSettingsFileNames.end());
        std::sort(binSettingsFileNames.begin(), binSettingsFileNames.end());
        std::set_difference(dependenciesSettingsFileNames.begin(), dependenciesSettingsFileNames.end(), binSettingsFileNames.begin(), binSettingsFileNames.end(), std::inserter(missingFileNames, missingFileNames.begin()));

        for (const std::filesystem::path& name : missingFileNames)
        {
            const std::filesystem::path source = std::filesystem::absolute(SIMPLE_DIR_DEPENDENCIES_SETTINGS) / name;
            const std::filesystem::path destination = std::filesystem::absolute(SIMPLE_DIR_SETTINGS) / name;
            std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);

            Console::Print("Copied: ", ConsoleTextColor::White, false);
            Console::Print(name.string(), ConsoleTextColor::Green, true);
        }

        for (const std::filesystem::path& name : forceDependenciesSettingsFileNames)
        {
            const std::filesystem::path source = std::filesystem::absolute(SIMPLE_DIR_DEPENDENCIES_FORCE) / name;
            const std::filesystem::path destination = std::filesystem::absolute(SIMPLE_DIR_SETTINGS) / name;
            std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);

            Console::Print("Force copied: ", ConsoleTextColor::White, false);
            Console::Print(name.string(), ConsoleTextColor::Green, true);
        }
    }

    float Engine::GetDeltaTime() const
    {
        return mFrameTimer.GetDeltaTime().count();
    }

    double Engine::GetTotalTime() const
    {
        return mTotalTimer.GetDeltaTime().count();
    }
}