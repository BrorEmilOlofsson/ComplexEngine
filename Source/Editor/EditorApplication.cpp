#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorApplication.hpp"
#include "Engine/Engine.hpp"
#include "Editor/Editor.hpp"
#include "Game/Game.hpp"

namespace Simple
{

	void RunEditor(OperatingSystem&& operatingSystem)
	{
		PROFILER_FUNCTION(profiler::colors::Black)
		Engine engine(std::move(operatingSystem));
		engine.Init();

		Editor editor(&engine);
		editor.Init();

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
			editor.Update();

			engine.Render();
			editor.Render();

			engine.EndFrame();
			PROFILER_END();
		}

		engine.Shutdown();
	}
}
