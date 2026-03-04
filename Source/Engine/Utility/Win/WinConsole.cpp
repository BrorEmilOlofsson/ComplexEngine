#include "Engine/Precompiled/EnginePch.hpp"
#include "WinConsole.hpp"
#include <Windows.h>
#include <consoleapi.h>
#include <consoleapi2.h>
#include <processenv.h>
#include <iostream>

namespace CLX
{
	Console::Console()
	{
#pragma warning(push)
#pragma warning( disable : 4996 )
		AllocConsole();
		freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		freopen_s((FILE**)stdout, "CONOUT$", "w", stderr);

		setbuf(stdin, NULL);
		setbuf(stdout, NULL);
		setbuf(stderr, NULL);

		SetConsoleTitle(L"Simple, it's just that easy");
#pragma warning( pop )
	}

	Console::~Console()
	{
#pragma warning( push )
#pragma warning( disable : 4996 )
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);
#pragma warning( pop )
		FreeConsole();
	}

#ifdef _DEBUG

	static WORD GetConsoleColor(const ConsoleTextColor color, WORD defaultAttribues)
	{
		switch (color)
		{
		case ConsoleTextColor::Red:
			return FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;
		case ConsoleTextColor::Green:
			return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;
		case ConsoleTextColor::Blue:
			return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			break;
		case ConsoleTextColor::Yellow:
			return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;
		case ConsoleTextColor::White:
		default:
			return defaultAttribues;
			break;
		}
	}

	void Console::Print(const std::string_view text, const ConsoleTextColor color, const bool shouldEndline)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO defaultInfo;
		GetConsoleScreenBufferInfo(hConsole, &defaultInfo);
		const WORD defaultAttributes = defaultInfo.wAttributes;

		SetConsoleTextAttribute(hConsole, GetConsoleColor(color, defaultAttributes));

		std::cout << text;

		if (shouldEndline)
		{
			std::cout << std::endl;
		}

		SetConsoleTextAttribute(hConsole, defaultAttributes);
	}
#else
	void Console::Print(const std::string_view, const ConsoleTextColor /*aColor*/, const bool /*aShouldEndline*/)
	{
	}
#endif
}