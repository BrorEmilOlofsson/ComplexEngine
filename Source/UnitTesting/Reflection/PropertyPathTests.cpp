#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Reflection/PropertyPath.hpp"

using namespace CLX;

TEST_CASE("operator==(PropertyPath, PropertyPath)", "[PropertyPath]")
{
    DataTypeID dataTypeID1 = GetDataTypeID<int>();
    DataTypeID dataTypeID2 = GetDataTypeID<float>();
    PropertyPath path1{ dataTypeID1, { "member1", Index{ 0 } } };
    PropertyPath path2{ dataTypeID1, { "member1", Index{ 0 } } };
    PropertyPath path3{ dataTypeID1, { "member2", Index{ 0 } } };
    PropertyPath path4{ dataTypeID2, { "member1", Index{ 0 } } };
    PropertyPath path5{ dataTypeID1, { "member1", "member2", Index{ 0 }}};
    PropertyPath path6{ dataTypeID1, { "member1", "member2", Index{ 4 }}};
    PropertyPath path7{ dataTypeID1, { "member1", "member2", Index{ 4 }}};
    REQUIRE(path1 == path2);
    REQUIRE(path1 != path3);
    REQUIRE(path1 != path4);
    REQUIRE(path1 != path5);
    REQUIRE(path1 != path6);
    REQUIRE(path5 != path6);
    REQUIRE(path6 == path7);
}