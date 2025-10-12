#include "Game/Precompiled/GamePch.hpp"
#include "GameMain.hpp"
#include "Engine/Engine.hpp"
#include "Game/GameWorld.hpp"

namespace Simple
{

	void RunGame(std::unique_ptr<OperatingSystem> aOperatingSystem)
	{
		PROFILER_FUNCTION(profiler::colors::Black);
		Engine engine(std::move(aOperatingSystem));
		engine.Init();

		GameWorld gameWorld;
		gameWorld.Init();

		engine.LateInit();

		while (true)
		{
			PROFILER_BEGIN("Main loop");
			if (!engine.BeginFrame())
			{
				break;
			}
			engine.Update();
			engine.Render();
			engine.EndFrame();
			PROFILER_END();
		}

		engine.Shutdown();
	}
}