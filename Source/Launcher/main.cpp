#include "Engine/Debugger/MemoryTracker/MemoryTracker.h"
#include "Utility/EasyProfilerOutput.hpp"
#include "Editor/EditorApplication.hpp"
#include "Game/GameMain.hpp"
#include "Engine/OperatingSystem/OperatingSystem.hpp"
#include <memory>

struct Profiler
{
	Profiler([[maybe_unused]] const std::string& name)
	{
		PROFILER_INIT();
		PROFILER_ENABLE();
		PROFILER_START_LISTEN();
		PROFILER_BEGIN(name.c_str());
	}

	~Profiler()
	{
		PROFILER_END();
		PROFILER_DISABLE();

		Simple::EasyProfilerOutput();
	}
};

#ifdef _WIN32

#include "Engine/Win/WinWindow.hpp"
#include "Engine/Win/WinOperatingSystem.hpp"

namespace Simple
{

	static OperatingSystem CreateWindowsOperatingSystem(HINSTANCE hInstance)
	{
		return OperatingSystem(Win_OperatingSystem(hInstance, L"SimpleWindowClass"));
	}

	static void Run(OperatingSystem&& operatingSystem);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, [[maybe_unused]] _In_ int nCmdShow)
{
	Profiler profiler("Main.cpp");
	Simple::Run(Simple::CreateWindowsOperatingSystem(hInstance));

	return 0;

}

#endif

namespace Simple
{
	using RunFunction = void(*)(OperatingSystem&&);

	static RunFunction GetRunFunction()
	{
#ifdef _DEBUG_EDITOR
		return RunEditor;
#elif _RELEASE_EDITOR
		return RunEditor;
#elif _DEBUG_GAME
		return RunGame;
#elif _RELEASE_GAME
		return RunGame;
#endif
	}

	static void Run(OperatingSystem&& operatingSystem)
	{
		RunFunction runFunction = GetRunFunction();

		runFunction(std::move(operatingSystem));
	}
}