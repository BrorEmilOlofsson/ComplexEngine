#include "Engine/Precompiled/EnginePch.hpp"
#include "Engine/Engine.hpp"
#include "Engine/ECSEngine/Systems/RenderSystem.hpp"
#include "Engine/ECSEngine/Systems/RenderLightSystem.hpp"
#include "Engine/ECSEngine/Systems/CameraSystem.hpp"
#include "Engine/ECSEngine/Systems/AnimationSystem.hpp"
#include "Engine/ECSEngine/Systems/RotatingMovementSystem.hpp"
#include "Engine/ECSEngine/Systems/NavmeshSystem.hpp"
#include "Engine/ECSEngine/Systems/DebugShapeSystem.hpp"
#include "Engine/ECSEngine/Components/AllEngineComponents.hpp"
#include "Utility/File/FileUtility.hpp"
#include "Engine/Reflection/EngineReflectionRegistration.hpp"
#include "Utility/GraphicsBufferData.hpp"
#include "Utility/EngineDirectories.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Engine/ECS/ECSSerializer.hpp"
#include <External/nlohmann/json.hpp>
#include <fstream>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

namespace Simple
{

	Engine::Engine(OperatingSystem&& operatingSystem)
		: mOperatingSystem(std::move(operatingSystem))
	{
		RegisterEngineComponents();
		ECSRegistry::Get().RegisterSystem<RotatingMovementSystem>();
		ECSRegistry::Get().RegisterSystem<RenderSystem>();
		ECSRegistry::Get().RegisterSystem<RenderLightSystem>();
		ECSRegistry::Get().RegisterSystem<CameraSystem>();
		ECSRegistry::Get().RegisterSystem<AnimationSystem>();
		ECSRegistry::Get().RegisterSystem<NavmeshSystem>();
		ECSRegistry::Get().RegisterSystem<DebugShapeSystem>();
		DataTypeRegistry::GetInstance().Assert();
		mBlackboard = std::make_shared<Blackboard>();
		mAssetManager = std::make_shared<AssetManager>();
		mGraphicsSettings = std::make_shared<GraphicsSettings>();
		mOperatingSystem.SetAssetManager(mAssetManager);
		mOperatingSystem.SetGraphicsSettings(mGraphicsSettings);

		std::shared_ptr<Blackboard> blackboard = mBlackboard;
		auto a = [blackboard](const std::filesystem::path& path)
			{
				std::shared_ptr<EntityComposition> entityComposition = std::make_shared<EntityComposition>(ECSRegistry::Get());
				LoadEntityComposition(path, *entityComposition, *blackboard);
				return EntityCompositionAsset(std::move(entityComposition));
			};
		mAssetManager->GetAssetLoader().SetEntityCompositionLoader(a);

        mDataTypeRegistry = std::move(DataTypeRegistry::GetInstance());
		DataTypeRegistry::Destroy();
	}

	Engine::~Engine()
	{
		ECSRegistry::Destroy();
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
		ECSRegistry::Get().SetBlackboard(mBlackboard);

		mBlackboard->Insert<Key_GraphicsSettings>(*mGraphicsSettings);
		mBlackboard->Insert<Key_AssetManager>(*mAssetManager);
		mBlackboard->Insert<Key_SceneManager>(mSceneManager);
		mBlackboard->Insert<Key_DataTypeRegistry>(mDataTypeRegistry);
		mBlackboard->Insert<Key_InputState>(mInputState);

		mAssetManager->LoadAssets();
		mOperatingSystem.Init();
		mMainWindow = mOperatingSystem.MakeWindow(Vector2ui(1600, 900), L"SimpleEngine");

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

	void Engine::LateInit()
	{
		mSceneManager.Init(mBlackboard);

        RenderContext r = mOperatingSystem.GetGraphicsFoundation().CreateRenderContext(mOperatingSystem.GetWindow(mMainWindow).GetClientSize());
		mSceneManager.GetCurrentScene().GetRenderState().SetRenderContext(std::move(r));
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

		mInputState = mOperatingSystem.GetWindow(mMainWindow).GetInputState();
		mInputManager.Update(mInputState);

		if (!windowFrameBuffer.droppedFiles.empty())
		{
			FileUtility::CopyFiles(windowFrameBuffer.droppedFiles, mCurrentDropPath);
		}

		mSceneManager.GetCurrentScene().BeginFrame(mOperatingSystem.GetWindow(mMainWindow).GetClientSize(), mOperatingSystem.GetOS().GetCursorScreenPosition());

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
		mSceneManager.GetCurrentScene().GetRenderState().SetAmbientLight(AmbientLight{});
		mBlackboard->Insert<Key_DeltaTime>(deltaTimeCapped);
		mSceneManager.Update(deltaTimeCapped);
		//mAudioManager.Update();
	}

	void Engine::Render()
	{
		PROFILER_FUNCTION(profiler::colors::Cyan700);

		mSceneManager.Render();

#ifndef _EDITOR
		mSceneManager.GetCurrentScene().GetRenderState().SetRenderRect(AABB2i::CreateFromDefaultAndExtent(Vector2i(mOperatingSystem.GetWindow(mMainWindow).GetClientSize())));
#endif
		mOperatingSystem.Render(mSceneManager.GetCurrentScene().GetRenderState());
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

		const nlohmann::json& windowSizeJson = json["Game_Settings"]["Window_Size"];
		const nlohmann::json& resolutionJson = json["Game_Settings"]["Resolution"];

		Vector2ui windowSize;
		windowSize.x = windowSizeJson["x"];
		windowSize.y = windowSizeJson["y"];

		Vector2ui resolution;
		resolution.x = resolutionJson["x"];
		resolution.y = resolutionJson["y"];
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