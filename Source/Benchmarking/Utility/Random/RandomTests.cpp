#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Utility/Random.hpp"
#include "Engine/Utility/Utility.hpp"

using namespace CLX;

std::vector<int> GetRandomInts(auto& randomEngine)
{
    std::vector<int> values;
    for (int i = 0; i < 100; ++i)
    {
        values.push_back(GetRandomNumber(1, 100, randomEngine));
    }
    return values;
}

std::vector<std::string> GetRandomStrings(auto& randomEngine)
{
    std::vector<std::string> values;
    for (int i = 0; i < 100; ++i)
    {
        const int length = GetRandomNumber(5, 10, randomEngine);
        std::string str;
        for (int j = 0; j < length; ++j)
        {
            const char c = static_cast<char>(GetRandomNumber(97, 122, randomEngine));
            str.push_back(c);
        }
        values.push_back(str);
    }
    return values;
}

TEST_CASE("Benchmarking Random")
{
    XorShift32 randomEngine(5);

    //std::vector<int> values = GetRandomInts(randomEngine);
    auto values = GetRandomStrings(randomEngine);

    auto defaultValue = GetRandomValue(values, randomEngine);
    auto filter = [defaultValue](const auto& value) 
        { 
            auto total = 0;
            for (const char& c : value)
            {
                total += static_cast<int>(std::tolower(c));
            }
            return IsOdd{}(total) || value != defaultValue;
        };

    BENCHMARK("GetRandomValue filtered 1")
    {
        return GetRandomValue(values, filter, randomEngine);
    };

    BENCHMARK("GetRandomValue filtered 2")
    {
        return GetRandomValue2(values, filter, randomEngine);
    };

    BENCHMARK("GetRandomValue filtered 3")
    {
        return GetRandomValue3(values, filter, randomEngine);
    };
}