#pragma once
#include "Utility/Win/WinConsole.hpp"
#include "Utility/SystemTimer.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Audio/AudioManager.hpp"
#include "Engine/OperatingSystem/OperatingSystem.hpp"
#include "Engine/SimpleScript/SimpleNodeScript.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Graphics/GraphicsSettings.hpp"
#include "Engine/OperatingSystem/WindowHandle.hpp"
#include "Engine/Input/InputManager.hpp"
#include <memory>
#include <unordered_map>
#include <string>

namespace Simple
{

	class Engine final
	{
	public:

		explicit Engine(OperatingSystem&& operatingSystem);
		~Engine();

		void Init();
		void LateInit();
		void Shutdown();

		bool BeginFrame();
		void EndFrame();
		void Update();
		void Render();

	public:

		[[nodiscard]] double GetTotalTime() const;
		[[nodiscard]] float GetDeltaTime() const;
		[[nodiscard]] const InputState& GetInputState() const { return mInputState; }
		[[nodiscard]] SceneManager& GetSceneManager() { return mSceneManager; }
		[[nodiscard]] const SceneManager& GetSceneManager() const { return mSceneManager; }
		[[nodiscard]] AudioManager& GetAudioManager() { return mAudioManager; }
		[[nodiscard]] const AudioManager& GetAudioManager() const { return mAudioManager; }
		[[nodiscard]] OperatingSystem& GetOperatingSystem() { return mOperatingSystem; }
		[[nodiscard]] const OperatingSystem& GetOperatingSystem() const { return mOperatingSystem; }
		[[nodiscard]] WindowView GetMainWindow();
		[[nodiscard]] AssetManager& GetAssetManager() { return *mAssetManager; }
		[[nodiscard]] GraphicsSettings& GetGraphicsSettings() { return *mGraphicsSettings; }
		[[nodiscard]] class DataTypeRegistry& GetDataTypeRegistry();
		[[nodiscard]] const class DataTypeRegistry& GetDataTypeRegistry() const;
		void SetShouldExit(bool shouldExit = true);
		void SetCurrentDropPath(const std::filesystem::path& path);

	private:

		void LoadSettingsFromJson();
		void CheckAndCopySettingsFiles();
		
	private:

		OperatingSystem mOperatingSystem;
		[[no_unique_address]] Console mConsole;
		AudioManager mAudioManager;
		SceneManager mSceneManager;

		SystemTimerf mFrameTimer;
		SystemTimerd mTotalTimer;
		WindowHandle mMainWindow;
		InputState mInputState;

		[[no_unique_address]] SimpleNodeScript mNodeScript;

		std::size_t mFrameCounter = 0;
		std::filesystem::path mCurrentDropPath;
		std::atomic_bool mShouldExit;
		std::shared_ptr<Blackboard> mBlackboard;
		std::shared_ptr<AssetManager> mAssetManager;
		std::shared_ptr<GraphicsSettings> mGraphicsSettings;

		InputManager mInputManager;
	};
}