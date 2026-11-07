#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Utility/Random.hpp"
#include <print>
#include <algorithm>
#include <unordered_set>

#include "Engine/Utility/PrintStruct.hpp"
#include "Engine/Utility/Utility.hpp"

using namespace CLX;

TEST_CASE("GetRandomNumber (int, int)")
{
    auto randomEngine = CreateDefaultRandomEngine();
    int i = GetRandomNumber(5, 10, randomEngine);
    REQUIRE(i >= 5);
    REQUIRE(i <= 10);
}

TEST_CASE("GetRandomNumber (float, float)")
{
    auto randomEngine = CreateDefaultRandomEngine();
    const float f = GetRandomNumber(5.0f, 10.0f, randomEngine);
    REQUIRE(f >= 5.0f);
    REQUIRE(f <= 10.0f);
}

TEST_CASE("GetRandomBool")
{
    auto randomEngine = CreateDefaultRandomEngine();
    const bool b = GetRandomBool(randomEngine);
    REQUIRE((b == true || b == false));
}

TEST_CASE("GetRandomNumber (Bounds<int>)")
{
    auto randomEngine = CreateDefaultRandomEngine();
    const Bounds<int> bounds = Bounds<int>::FromMinAndMax(5, 10);
    const int i = GetRandomNumber(bounds, randomEngine);
    REQUIRE(i >= 5);
    REQUIRE(i <= 10);
}

TEST_CASE("GetRandomNumber (Bounds<float>)")
{
    auto randomEngine = CreateDefaultRandomEngine();
    const Bounds<float> bounds = Bounds<float>::FromMinAndMax(5.0f, 10.0f);
    const float f = GetRandomNumber(bounds, randomEngine);
    REQUIRE(f >= 5.0f);
    REQUIRE(f <= 10.0f);
}

TEST_CASE("GetRandomIndex (std::vector<int>)")
{
    auto engine = CreateDefaultRandomEngine();
    const std::vector<int> vec = { 1, 2, 3, 4, 5 };
    const size_t i = GetRandomIndex(vec, engine);
    REQUIRE((i >= 0 && i <= 4));
}

TEST_CASE("GetRandomIndex (std::map<int, int>)")
{
    auto engine = CreateDefaultRandomEngine();
    const std::map<int, int> map = { { 1, 1 },{ 2, 2 },{ 3, 3 },{ 4, 4 },{ 5, 5 } };
    const size_t i = GetRandomIndex(map, engine);
    REQUIRE((i >= 0 && i <= 4));
}

TEST_CASE("GetRandomValue (std::vector<int>)")
{
    auto&& engine = CreateDefaultRandomEngine();
    const std::vector<int> vec = { 1, 2, 3, 4, 5 };
    const int i = GetRandomValue(vec, engine);
    REQUIRE((i == 1 || i == 2 || i == 3 || i == 4 || i == 5));
}

TEST_CASE("GetRandomValue (std::set<int>)")
{
    auto engine = CreateDefaultRandomEngine();
    const std::set<int> set = { 1, 2, 3, 4, 5 };
    const int i = GetRandomValue(set, engine);
    REQUIRE((i >= 1 && i <= 5));
}

TEST_CASE("GetRandomValue (std::map<int, int>)")
{
    auto engine = CreateDefaultRandomEngine();
    const std::map<int, int> map = { { 1, 1 },{ 2, 2 },{ 3, 3 },{ 4, 4 },{ 5, 5 } };
    const std::pair<const int, int> i = GetRandomValue(map, engine);
    REQUIRE((i.first >= 1 && i.first <= 5));
    REQUIRE((i.second >= 1 && i.second <= 5));
}

TEST_CASE("GetRandomValue (std::vector<int>, filtered)")
{
    auto engine = CreateDefaultRandomEngine();

    std::vector<int> vec = { 1, 2, 3, 4, 5 };

    std::vector<uint32_t> counts(5, 0);
    for (int i = 0; i < 1000000; ++i)
    {
        const int result = GetRandomValue(vec, IsOdd{}, engine);
        counts[result - 1]++;
        //REQUIRE((result == 2 || result == 4));
    }
}

TEST_CASE("GetRandomValue (std::vector<int>, WeightRange)")
{
    auto engine = CreateDefaultRandomEngine();
    {
        const std::vector<int> vec = { 1, 2, 3, 4, 5 };
        const std::vector<long> weights = { 1, 2, 3, 4, 5 };
        const int& i = GetRandomValue(vec, weights, engine);
        REQUIRE((i >= 1 && i <= 5));
        REQUIRE((&i == &vec[0] || &i == &vec[1] || &i == &vec[2] || &i == &vec[3] || &i == &vec[4]));
    }
    {
        const std::vector<int> vec = { 1, 2, 3, 4, 5 };
        const std::vector<long> weights = { 0, 0, 0, 0, 1 };
        const int& i = GetRandomValue(vec, weights, engine);
        REQUIRE(i == 5);
        REQUIRE(&i == &vec[4]);
    }
}

TEST_CASE("GetRandomValue (OutcomeTable)")
{
    OutcomeTableUInt32<std::string_view> table;
    table.Insert("Emil", 500);
    table.Insert("Emil 2", 500);
    table.Insert("Emil 3", 500);
    const std::string_view s = GetRandomValue(table, CreateDefaultRandomEngine());
    REQUIRE((s == "Emil" || s == "Emil 2" || s == "Emil 3"));
}

TEST_CASE("GetRandomValueAndRemove (std::vector<int>)")
{
    std::vector<int> values = { 1, 2, 3, 4, 5 };
    auto engine = CreateDefaultRandomEngine();
    const int value = GetRandomValueAndRemove(values, engine);
    REQUIRE((value >= 1 && value <= 5));
    REQUIRE(std::find(values.begin(), values.end(), value) == values.end());
    std::vector<int> sortedValues = values;
    std::ranges::sort(sortedValues, std::less{});
    REQUIRE(values == sortedValues);
}

TEST_CASE("GetRandomValueAndRemove (std::set<int>)")
{
    std::set<int> values = { 1, 2, 3, 4, 5 };
    auto engine = CreateDefaultRandomEngine();
    const int value = GetRandomValueAndRemove(values, engine);
    REQUIRE((value >= 1 && value <= 5));
    REQUIRE(std::find(values.begin(), values.end(), value) == values.end());
}

TEST_CASE("GetRandomList (std::vector<int>)")
{
    std::vector<int> values = { 1, 2, 3, 4, 5 };
    auto engine = CreateDefaultRandomEngine();
    const std::vector<int> randomValues = GetRandomList(std::move(values), 3, engine);
    REQUIRE(randomValues.size() == 3);
    for (const int value : randomValues)
    {
        REQUIRE((value >= 1 && value <= 5));
    }
}

TEST_CASE("GetRandomList (std::set<int>)")
{
    std::set<int> values = { 1, 2, 3, 4, 5 };
    auto engine = CreateDefaultRandomEngine();
    const std::vector<int> randomValues = GetRandomList(std::move(values), 3, engine);
    REQUIRE(randomValues.size() == 3);
    for (const int value : randomValues)
    {
        REQUIRE((value >= 1 && value <= 5));
    }
}

TEST_CASE("GetRandomUniqueList && (std::vector<int>)")
{
    std::vector<int> values = { 1, 2, 3, 4, 5 };
    auto engine = CreateDefaultRandomEngine();
    const std::vector<int> uniqueValues = GetRandomUniqueList(std::move(values), 3, engine);

    REQUIRE(uniqueValues.size() == 3);
    for (const int value : uniqueValues)
    {
        REQUIRE((value >= 1 && value <= 5));
    }
    std::unordered_set<int> uniqueValuesSet(std::ranges::begin(uniqueValues), std::ranges::end(uniqueValues));
    REQUIRE(uniqueValuesSet.size() == 3);
}

TEST_CASE("GetRandomUniqueList && (std::set<int>)")
{
    std::set<int> values = { 1, 2, 3, 4, 5 };
    auto engine = CreateDefaultRandomEngine();
    const std::vector<int> uniqueValues = GetRandomUniqueList(std::move(values), 3, engine);

    REQUIRE(uniqueValues.size() == 3);
    for (const int value : uniqueValues)
    {
        REQUIRE((value >= 1 && value <= 5));
    }
    std::unordered_set<int> uniqueValuesSet(std::ranges::begin(uniqueValues), std::ranges::end(uniqueValues));
    REQUIRE(uniqueValuesSet.size() == 3);
}


TEST_CASE("GetRandomUniqueList const & (std::vector<int>)")
{
    const std::vector<int> values = { 1, 2, 3, 4, 5 };
    auto engine = CreateDefaultRandomEngine();
    const std::vector<int> uniqueValues = GetRandomUniqueList(values, 3, engine);

    REQUIRE(uniqueValues.size() == 3);
    for (const int value : uniqueValues)
    {
        REQUIRE((value >= 1 && value <= 5));
    }
    std::unordered_set<int> uniqueValuesSet(std::ranges::begin(uniqueValues), std::ranges::end(uniqueValues));
    REQUIRE(uniqueValuesSet.size() == 3);
}

TEST_CASE("GetRandomUniqueList const & (std::set<int>)")
{
    const std::set<int> values = { 1, 2, 3, 4, 5 };
    auto engine = CreateDefaultRandomEngine();
    const std::vector<int> uniqueValues = GetRandomUniqueList(values, 3, engine);

    REQUIRE(uniqueValues.size() == 3);
    for (const int value : uniqueValues)
    {
        REQUIRE((value >= 1 && value <= 5));
    }

    std::unordered_set<int> uniqueValuesSet(std::ranges::begin(uniqueValues), std::ranges::end(uniqueValues));
    REQUIRE(uniqueValuesSet.size() == 3);
}


TEST_CASE("XorShift")
{
    XorShift32 state;

    const int value = GetRandomNumber(1, 2, state);
    REQUIRE((value == 1 || value == 2));
}