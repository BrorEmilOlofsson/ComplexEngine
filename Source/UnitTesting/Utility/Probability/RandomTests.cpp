#include "Engine/Utility/Probability/Random.hpp"
#include <External/Catch2/catch_amalgamated.hpp>

using namespace Simple;

TEST_CASE("Get Random Number")
{
	{
		int i = GetRandomNumber(5, 10);
		REQUIRE(i >= 5);
		REQUIRE(i <= 10);
	}

	{
		const float f = GetRandomNumber(5.0f, 10.0f);
		REQUIRE(f >= 5.0f);
		REQUIRE(f <= 10.0f);
	}
}

TEST_CASE("GetRandomValueInOutcomeTable")
{

	OutcomeTableUInt<std::string_view> table;

	table.Insert("Emil", 500);
	table.Insert("Emil 2", 500);
	table.Insert("Emil 3", 500);

	const std::string_view s = GetRandomValue(table);
}