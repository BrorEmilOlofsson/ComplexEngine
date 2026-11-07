#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Utility/DynamicBitset.hpp"
#include <bitset>

using namespace CLX;

TEST_CASE("DynamicBitset::Default Constructor")
{
    DynamicBitset bitset;
    REQUIRE(bitset.Count() == 0);
}

TEST_CASE("DynamicBitset::FromRange")
{
    std::vector<bool> vec = { true, false, true };
    DynamicBitset bitset = DynamicBitset::FromRange(vec);
    REQUIRE(bitset.Count() == 3);
}

TEST_CASE("DynamicBitset::FromSize")
{
    DynamicBitset bitset = DynamicBitset::FromSize(5, true);
    REQUIRE(bitset.Count() == 5);
    REQUIRE(bitset.All() == true);
}

TEST_CASE("DynamicBitset::Count")
{
    DynamicBitset bitset;
    REQUIRE(bitset.Count() == 0);
}

TEST_CASE("DynamicBitset::All")
{
    DynamicBitset bitset;
    REQUIRE(bitset.All() == true);
}

TEST_CASE("DynamicBitset::None")
{
    DynamicBitset bitset;
    REQUIRE(bitset.None() == true);
}

TEST_CASE("DynamicBitset::Any")
{
    DynamicBitset bitset;
    REQUIRE(bitset.Any() == false);
}

TEST_CASE("DynamicBitset::operator[]")
{
    const DynamicBitset bitset = DynamicBitset::FromSize(3, true);
    REQUIRE(bitset[0] == true);
    REQUIRE(bitset[1] == true);
    REQUIRE(bitset[2] == true);
}

TEST_CASE("DynamicBitset::operator==")
{
    const DynamicBitset bitset1 = DynamicBitset::FromSize(3, true);
    const DynamicBitset bitset2 = DynamicBitset::FromSize(3, true);
    REQUIRE(bitset1 == bitset2);
}

TEST_CASE("DynamicBitset::operator== (different sizes)")
{
    DynamicBitset bitset1 = DynamicBitset::FromSize(3, true);
    DynamicBitset bitset2 = DynamicBitset::FromSize(4, true);
    REQUIRE(bitset1 != bitset2);
}

TEST_CASE("DynamicBitset::operator== (different values)")
{
    const DynamicBitset bitset1 = DynamicBitset::FromSize(3, true);
    const DynamicBitset bitset2 = DynamicBitset::FromSize(3, false);
    REQUIRE(bitset1 != bitset2);
}

TEST_CASE("DynamicBitset::operator!=")
{
    const DynamicBitset bitset1 = DynamicBitset::FromSize(3, true);
    const DynamicBitset bitset2 = DynamicBitset::FromSize(3, false);
    REQUIRE(bitset1 != bitset2);
}

TEST_CASE("DynamicBitset::operator!= (different sizes)")
{
    const DynamicBitset bitset1 = DynamicBitset::FromSize(3, true);
    const DynamicBitset bitset2 = DynamicBitset::FromSize(4, true);
    REQUIRE(bitset1 != bitset2);
}

TEST_CASE("DynamicBitset::operator!= (different values)")
{
    const DynamicBitset bitset1 = DynamicBitset::FromSize(3, true);
    const DynamicBitset bitset2 = DynamicBitset::FromSize(3, false);
    REQUIRE(bitset1 != bitset2);
}

TEST_CASE("DynamicBitset::operator&=")
{
    DynamicBitset bitset1 = DynamicBitset::FromRange(std::vector{ true, false, true, false });
    const DynamicBitset bitset2 = DynamicBitset::FromRange(std::vector{ true, true, false, false });
    bitset1 &= bitset2;
    const DynamicBitset expected = DynamicBitset::FromRange(std::vector{ true, false, false, false });
    REQUIRE(bitset1 == expected);
}

TEST_CASE("DynamicBitset::operator|=")
{
    DynamicBitset bitset1 = DynamicBitset::FromRange(std::vector{ true, false, true, false });
    const DynamicBitset bitset2 = DynamicBitset::FromRange(std::vector{ true, true, false, false });
    bitset1 |= bitset2;
    const  DynamicBitset expected = DynamicBitset::FromRange(std::vector{ true, true, true, false });
    REQUIRE(bitset1 == expected);
}

TEST_CASE("DynamicBitset::operator^=")
{
    DynamicBitset bitset1 = DynamicBitset::FromRange(std::vector{ true, false, true, false });
    const DynamicBitset bitset2 = DynamicBitset::FromRange(std::vector{ true, true, false, false });
    bitset1 ^= bitset2;
    const DynamicBitset expected = DynamicBitset::FromRange(std::vector{ false, true, true, false });
    REQUIRE(bitset1 == expected);
}

TEST_CASE("DynamicBitset::operator&")
{
    const DynamicBitset bitset1 = DynamicBitset::FromRange(std::vector{ true, true, false, false });
    const DynamicBitset bitset2 = DynamicBitset::FromRange(std::vector{ true, false, true, false });
    const DynamicBitset bitset3 = bitset1 & bitset2;
    const DynamicBitset expected = DynamicBitset::FromRange(std::vector{ true, false, false, false });
    REQUIRE(bitset3 == expected);
}

TEST_CASE("DynamicBitset::operator|")
{
    const DynamicBitset bitset1 = DynamicBitset::FromRange(std::vector{ true, true, false, false });
    const DynamicBitset bitset2 = DynamicBitset::FromRange(std::vector{ true, false, true, false });
    const DynamicBitset bitset3 = bitset1 | bitset2;
    const  DynamicBitset expected = DynamicBitset::FromRange(std::vector{ true, true, true, false });
    REQUIRE(bitset3 == expected);
}

TEST_CASE("DynamicBitset::operator^")
{
    const DynamicBitset bitset1 = DynamicBitset::FromRange(std::vector{ true, true, false, false });
    const DynamicBitset bitset2 = DynamicBitset::FromRange(std::vector{ true, false, true, false });
    const DynamicBitset bitset3 = bitset1 ^ bitset2;
    const  DynamicBitset expected = DynamicBitset::FromRange(std::vector{ false, true, true, false });
    REQUIRE(bitset3 == expected);
}

TEST_CASE("DynamicBitset::operator~")
{
    const DynamicBitset bitset = DynamicBitset::FromRange(std::vector{ true, false, true });
    const DynamicBitset bitset2 = ~bitset;
    const  DynamicBitset expected = DynamicBitset::FromRange(std::vector{ false, true, false });
    REQUIRE(bitset2 == expected);
}