#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Utility/Memory/HivePtr.hpp"
#include <memory>

using namespace CLX;

TEST_CASE("QueenPtr basic functionality", "[QueenPtr]")
{
    //std::shared_ptr<int> s;

    //s.use_count

    queen_ptr<int> ptr1 = make_queen_ptr(5);
    REQUIRE(*ptr1 == 5);
    REQUIRE(ptr1.use_count() == 1);
    queen_ptr<int> ptr2 = ptr1;
    REQUIRE(*ptr2 == 5);
    REQUIRE(ptr1.use_count() == 1);
    REQUIRE(ptr2.use_count() == 1);
    queen_ptr<int> ptr3 = std::move(ptr1);
    REQUIRE(*ptr3 == 5);
    REQUIRE(ptr1.use_count() == 0);
    REQUIRE(ptr3.use_count() == 1);
    //ptr2.reset();
    //REQUIRE(ptr2.use_count() == 0);
    //REQUIRE(ptr3.use_count() == 1);
    //ptr3.reset();
    //REQUIRE(ptr3.use_count() == 0);
}