#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Utility/Memory/HeapObject.hpp"

using namespace CLX;

TEST_CASE("HeapObject::Constructor")
{
    HeapObject<int> o;

    REQUIRE((*o) == 0);
}

TEST_CASE("HeapObject::Copy Constructor")
{
    HeapObject<int> o1(2);
    HeapObject<int> o2 = o1;

    REQUIRE((*o1) == (*o2));
    REQUIRE(&(*o1) != &(*o2));
}

TEST_CASE("HeapObject::Copy Assigment Operator")
{
    HeapObject<int> o1(2);
    HeapObject<int> o2;
    o2 = o1;

    REQUIRE((*o1) == (*o2));
    REQUIRE(&(*o1) != &(*o2));
}