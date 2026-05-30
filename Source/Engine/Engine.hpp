#pragma once
#include <memory>
#include "Engine/Utility/Win/WinConsole.hpp"
#include "Engine/Utility/SystemTimer.hpp"
#include "Engine/OperatingSystem/OperatingSystem.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/Input/InputState.hpp"
#include "Engine/ECS/ECSManager.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/Audio/AudioManager.hpp"
#include "Engine/SimpleScript/SimpleNodeScript.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include "Engine/Graphics/GraphicsSettings.hpp"
#include "Engine/OperatingSystem/WindowHandle.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include "Engine/ECS/ECSRegistry.hpp"
#include "Engine/Input/InputManager.hpp"
#include "Engine/ECS/EntitySerializationIDGenerator.hpp"

namespace CLX
{

	struct GameSettings
	{
		Dimension2u windowSize;
		Dimension2u resolution;
		std::filesystem::path startScenePath;
		bool vSync = false;
        bool fullScreen = false;
        bool windowedFullScreen = false;
        std::wstring windowTitle = L"ComplexEngine";
	};

	class Engine final
	{
	public:

		explicit Engine(OperatingSystem& operatingSystem);

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
		[[nodiscard]] OperatingSystem& GetOperatingSystem() { return mOperatingSystem.get(); }
		[[nodiscard]] const OperatingSystem& GetOperatingSystem() const { return mOperatingSystem.get(); }
		[[nodiscard]] WindowView GetMainWindow();
		[[nodiscard]] std::weak_ptr<AssetManager> GetAssetManager() { return mAssetManager; }
		[[nodiscard]] GraphicsSettings& GetGraphicsSettings() { return *mGraphicsSettings; }
		[[nodiscard]] class DataTypeRegistry& GetDataTypeRegistry();
		[[nodiscard]] const class DataTypeRegistry& GetDataTypeRegistry() const;
		[[nodiscard]] GraphicsFoundation& GetGraphicsFoundation();
        [[nodiscard]] const GraphicsFoundation& GetGraphicsFoundation() const;
        [[nodiscard]] ECSRegistry& GetECSRegistry() { return mECSRegistry; }
        [[nodiscard]] ECSManager& GetECSManager() { return mECSManager; }
        [[nodiscard]] EntitySerializationIDGenerator& GetEntityIDGenerator() { return mEntityIDGenerator; }
        [[nodiscard]] const ECSRegistry& GetECSRegistry() const { return mECSRegistry; }
		void SetShouldExit(bool shouldExit = true);
		void SetCurrentDropPath(const std::filesystem::path& path);

	private:

		DataTypeRegistry mDataTypeRegistry;
		std::reference_wrapper<OperatingSystem> mOperatingSystem;
		GraphicsFoundation mGraphicsFoundation;
        EntitySerializationIDGenerator mEntityIDGenerator;
		ECSRegistry mECSRegistry;
		ECSManager mECSManager;
		[[no_unique_address]] Console mConsole;
		std::shared_ptr<AssetManager> mAssetManager;
		SceneManager mSceneManager;
        AudioManager mAudioManager;
		InputManager mInputManager;

		SystemTimerf mFrameTimer;
		SystemTimerd mTotalTimer;
		WindowHandle mMainWindow;
		InputState mInputState;
        GameSettings mLoadedGameSettings;

		[[no_unique_address]] SimpleNodeScript mNodeScript;

		std::size_t mFrameCounter = 0;
		std::filesystem::path mCurrentDropPath;
		std::atomic_bool mShouldExit;
		std::shared_ptr<Blackboard> mBlackboard;
		std::shared_ptr<GraphicsSettings> mGraphicsSettings;
	};
}