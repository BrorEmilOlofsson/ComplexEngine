#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Quadtree.hpp"

using namespace Simple;

TEST_CASE("QuadtreeRecursive::Default Constructor")
{
    QuadtreeRecursive<std::string> quadtree = QuadtreeRecursive<std::string>::CreateFromBounds(
        AABB2f::FromMinAndExtent(Point2f::Zero(), Vector2f(200.0f, 200.0f))
    );
    quadtree.Insert("TestValue1", Point2f(110.0f, 110.0f));
    quadtree.Insert("TestValue2", Point2f(110.0f, 110.0f));
    quadtree.Insert("TestValue3", Point2f(110.0f, 110.0f));
    quadtree.Insert("TestValue4", Point2f(110.0f, 110.0f));
    quadtree.Insert("TestValue25", Point2f(110.0f, 110.0f));
    quadtree.Insert("TestValue26", Point2f(110.0f, 110.0f));
    quadtree.Insert("TestValue5", Point2f(10.0f, 10.0f));
    quadtree.Insert("TestValue6", Point2f(10.0f, 101.0f));
    quadtree.Insert("TestValue7", Point2f(101.0f, 10.0f));
    quadtree.Insert("TestValue8", AABB2f::FromMinAndMax(Point2f(10.0f, 10.f), Point2f(101.f, 101.f)));
    auto values = quadtree.QueryValues(Point2f(109, 109));
}

TEST_CASE("Quadtree::Default Constructor")
{
    Quadtree<std::string> quadtree;

    quadtree.QueryValues(Point2f(0, 0));
}