#include "Game/Precompiled/GamePch.hpp"
#include "GameMain.hpp"
#include "Engine/Engine.hpp"
#include "Game/Game.hpp"

namespace Simple
{

	void RunGame(OperatingSystem&& operatingSystem)
	{
		PROFILER_FUNCTION(profiler::colors::Black);
		Engine engine(std::move(operatingSystem));
		engine.Init();

		Game game;

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