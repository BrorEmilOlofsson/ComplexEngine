#include "Engine/Utility/Probability/OutcomeTable.hpp"
#include <External/Catch2/catch_amalgamated.hpp>

using namespace CLX;

TEST_CASE("OutcomeTable::GetOutcome")
{
	OutcomeTable<std::string_view> table;

	table.Insert("Emil", 500);
	table.Insert("Emil 2", 500);
	table.Insert("Emil 3", 500);

    std::vector<std::string_view> expectedValues = { "Emil", "Emil 2", "Emil 3" };
    std::vector<uint32_t> expectedWeights = { 500, 500, 500 };

    REQUIRE(std::equal(table.Values().begin(), table.Values().end(), expectedValues.begin()));
    REQUIRE(std::equal(table.Weights().begin(), table.Weights().end(), expectedWeights.begin()));
}

TEST_CASE("OutcomeTable::Count")
{
	OutcomeTableUInt<std::string_view> table;
	REQUIRE(table.Count() == 0);
	table.Insert("Emil", 500);
	REQUIRE(table.Count() == 1);
	table.Insert("Emil 2", 400);
	REQUIRE(table.Count() == 2);
	table.Insert("Emil 3", 300);
	REQUIRE(table.Count() == 3);
}

TEST_CASE("OutcomeTable::Erase")
{
	OutcomeTableUInt<std::string_view> table;

	table.Insert("Emil", 500);
	table.Insert("Emil 2", 400);
	table.Insert("Emil 3", 300);

	REQUIRE(table.Outcomes() == std::unordered_map<std::string_view, uint32_t>{{"Emil", 500}, { "Emil 2", 400 }, { "Emil 3", 300 }});

    table.Erase("Emil 2");
	
    REQUIRE(table.Outcomes() == std::unordered_map<std::string_view, uint32_t>{{"Emil", 500}, { "Emil 3", 300 }});
	REQUIRE(table.Count() == 2);
}

TEST_CASE("OutcomeTable::Clear")
{
    OutcomeTable<std::string_view> table;
    table.Insert("Emil", 500);
    table.Insert("Emil 2", 400);
    table.Insert("Emil 3", 300);
	REQUIRE(table.Count() == 3);
    table.Clear();
    REQUIRE(table.Count() == 0);
}