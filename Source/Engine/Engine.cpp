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
#include "Engine/Reflection/Reflection.hpp"
#include "Engine/Graphics/GraphicsFoundationCreator.hpp"
#include <External/nlohmann/json.hpp>
#include <External/imgui/imgui.h>
#include <fstream>

namespace CLX
{

    struct ProjectSettings
    {
        Bounds<uint64_t> usedEntityIDBounds;
    };

    ProjectSettings LoadProjectSettings()
    {
        const std::filesystem::path path = GetAbsoluteProjectSettingsPath();

        std::ifstream file(path);
        if (!file.is_open())
        {
            assert(false && "Failed To Open File");
            return {};
        }

        const nlohmann::json json = nlohmann::json::parse(file);
        file.close();

        const uint64_t min = json["Used EntityID Bounds"]["Min"];
        const uint64_t max = json["Used EntityID Bounds"]["Max"];

        ProjectSettings settings
        {
            .usedEntityIDBounds = Bounds<uint64_t>::FromMinAndMax(min, max)
        };
        return settings;
    }

    void SaveProjectSettings(const ProjectSettings& settings)
    {
        const std::filesystem::path path = GetAbsoluteProjectSettingsPath();

        std::ofstream file(path);
        if (!file.is_open())
        {
            assert(false && "Failed To Open File");
            return;
        }

        nlohmann::json json;
        json["Used EntityID Bounds"]["Min"] = settings.usedEntityIDBounds.GetMin();
        json["Used EntityID Bounds"]["Max"] = settings.usedEntityIDBounds.GetMax();
        file << json;
    }

    GameSettings LoadGameSettings()
    {
        const std::filesystem::path path = GetAbsoluteGameSettingsPath();
        std::ifstream file(path);
        if (!file.is_open())
        {
            assert(false && "Failed To Open File");
            return {};
        }

        const nlohmann::json json = nlohmann::json::parse(file);
        file.close();

        GameSettings settings;
        settings.windowSize = Dimension2u(json["Window Size"]["Width"], json["Window Size"]["Height"]);
        settings.resolution = Dimension2u(json["Resolution"]["Width"], json["Resolution"]["Height"]);
        settings.startScenePath = std::filesystem::path(std::string(json["Start Scene Path"]));
        settings.vSync = json["VSync"];
        settings.fullScreen = json["Full Screen"];
        settings.windowedFullScreen = json["Windowed Full Screen"];
        settings.windowTitle = ToWString(std::string(json["Window Title"]));
        return settings;
    }

    Engine::Engine(OperatingSystem& operatingSystem)
        : mOperatingSystem(operatingSystem)
        , mGraphicsFoundation(CreateGraphicsFoundation(eGraphicsAPI::DX11))
        , mAssetManager(std::make_shared<AssetManager>())
        , mBlackboard(std::make_shared<Blackboard>())
        , mGraphicsSettings(std::make_shared<GraphicsSettings>())
        , mSceneManager(mAssetManager)
    {
        RegisterEngineComponents();
        DummyRegister();
        RegisterEngineSystems(mECSRegistry);

        TypeRegistration::ExecuteRegistrations(mDataTypeRegistry, mECSRegistry);

        mDataTypeRegistry.Assert();
        mGraphicsFoundation.SetAssetManager(mAssetManager);
        mGraphicsFoundation.SetGraphicsSettings(mGraphicsSettings);

        std::shared_ptr<Blackboard> blackboard = mBlackboard;
        ECSRegistry* ecsRegistry = &mECSRegistry;
        ECSManager* ecsManager = &mECSManager;
        EntitySerializationIDGenerator* entityIDGenerator = &mEntityIDGenerator;
        auto entityCompositionLoader = [blackboard, ecsRegistry, ecsManager, entityIDGenerator](const std::filesystem::path& path)
            {
                EntityComposition entityComposition(*ecsManager, ecsManager->CreateECS(*ecsRegistry, *entityIDGenerator));
                LoadEntityComposition(path, entityComposition, *blackboard);
                return EntityCompositionAsset(std::move(entityComposition), path);
            };
        mAssetManager->GetAssetLoader().SetEntityCompositionLoader(entityCompositionLoader);
        mAssetManager->GetAssetLoader().SetAudioLoader([this](const std::filesystem::path& path)
            {
                if (auto audio = mAudioManager.LoadAudio(path))
                {
                    return AudioAsset(std::move(*audio), path);
                }
                return AudioAsset::Empty();
            });

        mBlackboard->Insert<Key_ECSRegistry>(mECSRegistry);

    }

    void Engine::Init()
    {
        mECSRegistry.SetBlackboard(mBlackboard);

        mBlackboard->Insert<Key_GraphicsSettings>(*mGraphicsSettings);
        mBlackboard->Insert<Key_AssetManager>(*mAssetManager);
        mBlackboard->Insert<Key_SceneManager>(mSceneManager);
        mBlackboard->Insert<Key_DataTypeRegistry>(mDataTypeRegistry);
        mBlackboard->Insert<Key_InputState>(mInputState);
        mBlackboard->Insert<Key_ECSManager>(mECSManager);
        mBlackboard->Insert<Key_EntityIDGenerator>(mEntityIDGenerator);
        mBlackboard->Insert<Key_AudioManager>(mAudioManager);

        mAssetManager->GetAssetLoader().SetSceneLoader([this](const std::filesystem::path& path)
            {
                Scene scene(mBlackboard);
                SceneLoader::LoadScene(scene, path, *mBlackboard);

                return SceneAsset(std::move(scene), path);
            });

        ProjectSettings projectSettings = LoadProjectSettings();
        mEntityIDGenerator.SetUsedIDBounds(projectSettings.usedEntityIDBounds);

        mLoadedGameSettings = LoadGameSettings();
        mGraphicsSettings->vSync = mLoadedGameSettings.vSync;

        mAssetManager->LoadAssets();
        mGraphicsFoundation.Init();
        mOperatingSystem.get().SetGraphicsFoundation(&mGraphicsFoundation);
        mMainWindow = mOperatingSystem.get().MakeWindow(mLoadedGameSettings.windowSize, mLoadedGameSettings.windowTitle);

        if (mLoadedGameSettings.fullScreen)
        {
            mOperatingSystem.get().GetWindow(mMainWindow).SetSize(FullScreen{});
        }
        else if (mLoadedGameSettings.windowedFullScreen)
        {
            mOperatingSystem.get().GetWindow(mMainWindow).SetSize(WindowedFullScreen{});
        }

        mFrameTimer.Start();
        mTotalTimer.Start();

        mNodeScript.Init();

        mOperatingSystem.get().LoadCursors(std::string(Directory::Assets) + std::string("Cursors"));

        mOperatingSystem.get().GetWindow(mMainWindow).Show();
    }

    void Engine::LateInit()
    {
        std::filesystem::path sceneFilePath = mLoadedGameSettings.startScenePath;

        SceneAssetHandle defaultScene = mAssetManager->GetScene(sceneFilePath);
        if (!defaultScene)
        {
            mAssetManager->CreateScene(std::filesystem::path(SIMPLE_DIR_SCENES) / (std::filesystem::path(std::wstring(L"DefaultScene") + std::wstring(AssetExtensions::Scene))));
        }
        mSceneManager.ChangeSceneDirectly(defaultScene);

        RenderContext r = mGraphicsFoundation.CreateRenderContext(mOperatingSystem.get().GetWindow(mMainWindow).GetClientSize());
        mSceneManager.GetActiveScene()->GetRenderState().SetRenderContext(std::move(r));

#ifndef _EDITOR
        mSceneManager.BeginPlay();
#endif
    }

    void Engine::Shutdown()
    {
        mGraphicsFoundation.Shutdown();

        SaveProjectSettings(ProjectSettings{ mEntityIDGenerator.GetUsedIDBounds().value_or({}) });
    }

    bool Engine::BeginFrame()
    {
        PROFILER_FUNCTION(profiler::colors::DeepOrange);
        mFrameTimer.Update();

        GraphicsBufferData bufferData;
        bufferData.frameTimer = mFrameTimer;
        bufferData.totalTimer = mTotalTimer;
        mGraphicsFoundation.BeginFrame(bufferData);
        mOperatingSystem.get().BeginFrame();

        const WindowFrameBuffer& windowFrameBuffer = mOperatingSystem.get().GetFrameBuffer();
        if (windowFrameBuffer.hasQuit)
        {
            return false;
        }

        mInputState = mOperatingSystem.get().GetInputState();

        mInputManager.Update(mInputState);

        if (!windowFrameBuffer.droppedFiles.empty())
        {
            FileUtility::CopyFiles(windowFrameBuffer.droppedFiles, mCurrentDropPath);
        }

        mSceneManager.GetActiveScene()->BeginFrame(mOperatingSystem.get().GetWindow(mMainWindow).GetClientSize(), mOperatingSystem.get().GetCursorScreenPosition());

        return !mShouldExit.load();
    }

    static void SyncToFPSCap(const std::chrono::steady_clock::time_point& lastTimePoint, const uint32_t fpsCap)
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
        renderContext = mSceneManager.GetActiveScene()->GetRenderState().GetRenderContext();
#endif
        mGraphicsFoundation.EndFrame();
        mOperatingSystem.get().EndFrame(renderContext);
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
        mAudioManager.Update();

        static AudioChannelID channelId = InvalidID<AudioChannelID>();

        if (channelId == InvalidID<AudioChannelID>())
        {
            AudioAssetHandle audioHandle = mAssetManager->GetAudio(GetAbsoluteAssetPath() / "Audio" / "StardewValley.mp3");
            channelId = mAudioManager.Play(audioHandle, 1.0f, { 0, 0, 0 });
        }

        static Point3f audioPosition = Point3f::Zero();
        static float audioVolume = 1.0f;
        static bool stop = true;
        mAudioManager.SetChannelPaused(channelId, stop);
        ImGui::Begin("Audio Test");
        if (ImGui::DragFloat3("Audio Position", &audioPosition.x, 0.001f))
        {
            mAudioManager.SetChannelPosition(channelId, audioPosition);

        }
        if (ImGui::DragFloat("Audio Volume", &audioVolume, 0.001f))
        {
            mAudioManager.SetChannelVolume(channelId, audioVolume);

        }
        if (ImGui::Checkbox("Stop Audio", &stop))
        {
            mAudioManager.SetChannelPaused(channelId, stop);
        }
        ImGui::End();
    }

    void Engine::Render()
    {
        PROFILER_FUNCTION(profiler::colors::Cyan700);

        mSceneManager.GetActiveScene()->Render();

#ifndef _EDITOR
        mSceneManager.GetActiveScene()->GetRenderState().SetRenderRect(AABB2i::FromDefaultAndExtent(ToVector2(mOperatingSystem.get().GetWindow(mMainWindow).GetClientSize())));
#endif
        mGraphicsFoundation.Render(mSceneManager.GetActiveScene()->GetRenderState());
    }

    WindowView Engine::GetMainWindow()
    {
        return mOperatingSystem.get().GetWindow(mMainWindow);
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
        return mGraphicsFoundation;
    }

    const GraphicsFoundation& Engine::GetGraphicsFoundation() const
    {
        return mGraphicsFoundation;
    }

    void Engine::SetShouldExit(bool shouldExit)
    {
        return mShouldExit.store(shouldExit);
    }

    void Engine::SetCurrentDropPath(const std::filesystem::path& path)
    {
        mCurrentDropPath = path;
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
