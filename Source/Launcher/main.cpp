#include <External/profiler.h>
#include "Engine/Utility/EasyProfilerOutput.hpp"
#include "Editor/EditorApplication.hpp"
#include "Game/GameMain.hpp"
#include "Engine/OperatingSystem/OperatingSystem.hpp"

#ifdef BUILD_WITH_EASY_PROFILER
#pragma message("BUILD_WITH_EASY_PROFILER is defined")
#else
#pragma message("BUILD_WITH_EASY_PROFILER is NOT defined")
#endif

struct Profiler
{
	Profiler([[maybe_unused]] const std::string& name)
	{
		EASY_PROFILER_ENABLE;
		PROFILER_INIT();
		PROFILER_ENABLE();
		PROFILER_START_LISTEN();
		PROFILER_BEGIN(name.c_str());
	}

	~Profiler()
	{
		PROFILER_END();
		PROFILER_DISABLE();

		CLX::EasyProfilerOutput();
	}
};

#ifdef _WIN32

#include <Windows.h>
#include "Engine/Win/WinOperatingSystem.hpp"

namespace CLX
{

	static OperatingSystem CreateWindowsOperatingSystem(HINSTANCE hInstance)
	{
		return OperatingSystem(Win_OperatingSystem(hInstance, L"WindowClass"));
	}

	static void Run(OperatingSystem& operatingSystem);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, [[maybe_unused]] _In_ int nCmdShow)
{
	Profiler profiler("Main.cpp");
    CLX::OperatingSystem operatingSystem = CLX::CreateWindowsOperatingSystem(hInstance);
	CLX::Run(operatingSystem);

	return 0;
}

#endif

namespace CLX
{
	using RunFunction = void(*)(OperatingSystem&);

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

	static void Run(OperatingSystem& operatingSystem)
	{
		RunFunction runFunction = GetRunFunction();

		runFunction(operatingSystem);
	}
}