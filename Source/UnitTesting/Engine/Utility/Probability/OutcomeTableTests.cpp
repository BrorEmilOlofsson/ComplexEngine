#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Utility/Probability/OutcomeTable.hpp"

using namespace Simple;

int Add(int a, int b)
{
    return a + b;
}

int Subtract(int a, int b)
{
    return a - b;
}

TEST_CASE("OutcomeTable::GetOutcome", "[OutcomeTable]")
{
    OutcomeTable<int(*)(int, int), uint64_t> table;
    table.Insert(Add, 60);
    table.Insert(Subtract, 40);
    
    REQUIRE(table.Count() == 2);
    REQUIRE(table.GetOutcome(0) == Add);
    REQUIRE(table.GetOutcome(59) == Add);
    REQUIRE(table.GetOutcome(60) == Subtract);
    REQUIRE(table.GetOutcome(99) == Subtract);
}