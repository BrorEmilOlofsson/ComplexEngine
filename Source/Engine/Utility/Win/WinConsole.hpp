#pragma once
#include <string_view>

namespace Simple
{
	enum class ConsoleTextColor
	{
		White,
		Red,
		Green,
		Blue,
		Yellow
	};

	class Console final
	{
	public:
		Console();
		~Console();

		// Delete copy constructor and copy assignment operator
		Console(const Console&) = delete;
		Console& operator=(const Console&) = delete;

		// Default move constructor and move assignment operator
		Console(Console&&) = delete;
		Console& operator=(Console&&) = delete;

		static void Print(const std::string_view text, const ConsoleTextColor color, const bool shouldEndline = true);
	};
}