#include "Engine/Utility/Probability/OutcomeTable.hpp"
#include <External/Catch2/catch_amalgamated.hpp>

using namespace CLX;

TEST_CASE("GetOutcome")
{
	OutcomeTableUInt<std::string_view> table;

	table.Insert("Emil", 500);
	table.Insert("Emil 2", 500);
	table.Insert("Emil 3", 500);

	REQUIRE(table.GetOutcome(0U) == "Emil");
	REQUIRE(table.GetOutcome(499) == "Emil");
	REQUIRE(table.GetOutcome(500) == "Emil 2");
	REQUIRE(table.GetOutcome(999) == "Emil 2");
	REQUIRE(table.GetOutcome(1000) == "Emil 3");
	REQUIRE(table.GetOutcome(1499) == "Emil 3");
}

TEST_CASE("WeightSum")
{
	OutcomeTableUInt<std::string_view> table;

	table.Insert("Emil", 500);
	table.Insert("Emil 2", 500);
	table.Insert("Emil 3", 500);

	REQUIRE(table.WeightSum() == 1500);
}