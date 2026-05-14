#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Utility/BufferQueue.hpp"
#include "Engine/Utility/Probability/Random.hpp"
#include <queue>

using namespace CLX;

TEST_CASE("BufferQueue push and size", "[BufferQueue]")
{
    buffer_queue<int> queue;
    SECTION("Push elements")
    {
        queue.push(1);
        queue.push(2);
        queue.push(3);
        REQUIRE(queue.size() == 3);
    }
}


TEST_CASE("BufferQueue push, push, push, pop, push, pop, pop", "[BufferQueue]")
{
    SECTION("Push elements")
    {
        buffer_queue<int> queue;
        queue.push(1);
        queue.push(2);
        queue.push(3);
        queue.pop();
        queue.push(4);
        queue.pop();
        queue.pop();
        REQUIRE(queue.size() == 1);
    }

    SECTION("Push elements 2")
    {
        buffer_queue<int> queue;
        queue.push(1);
        queue.pop();
        REQUIRE(queue.size() == 0);
    }

    SECTION("Push elements 3")
    {
        buffer_queue<int> queue;
        queue.push(1);
        queue.push(2);
        queue.push(3);
        queue.pop();
        queue.push(4);
        queue.push(5);
        queue.push(6);
        queue.push(7);
        REQUIRE(queue.size() == 6);
        REQUIRE(queue.front() == 2);
        queue.pop();
        REQUIRE(queue.front() == 3);
        queue.pop();
        REQUIRE(queue.front() == 4);
        queue.pop();
        REQUIRE(queue.front() == 5);
        queue.pop();
        REQUIRE(queue.front() == 6);
        queue.pop();
        REQUIRE(queue.front() == 7);
    }

    SECTION("Push elements 4")
    {
        buffer_queue<int> queue;
        queue.push(1);
        queue.push(2);
        queue.push(3);
        queue.pop();
        REQUIRE(queue.size() == 2);
        queue.pop();
        REQUIRE(queue.size() == 1);
        queue.pop();
        REQUIRE(queue.size() == 0);
        queue.push(4);
    }
    SECTION("Push elements 5")
    {
        buffer_queue<int> queue;
        queue.push(1);
        queue.push(2);
        queue.push(3);
        queue.pop();
        REQUIRE(queue.size() == 2);
        queue.pop();
        REQUIRE(queue.size() == 1);
        queue.push(4);
        REQUIRE(queue.size() == 2);
        queue.push(5);
        REQUIRE(queue.size() == 3);
    }

    SECTION("Push elements 5")
    {
        buffer_queue<int> queue;
        queue.push(1);
        queue.push(2);
        REQUIRE(queue.size() == 2);
        queue.pop();
        REQUIRE(queue.size() == 1);
        queue.push(3);
        REQUIRE(queue.size() == 2);
        queue.pop();
        REQUIRE(queue.size() == 1);
        queue.push(4);
        REQUIRE(queue.size() == 2);
        queue.push(5);
        REQUIRE(queue.size() == 3);
        queue.push(6);
        REQUIRE(queue.size() == 4);
        queue.push(7);
        REQUIRE(queue.size() == 5);
        queue.push(8);
        REQUIRE(queue.size() == 6);
        queue.pop();
        REQUIRE(queue.size() == 5);
        queue.pop();
        REQUIRE(queue.size() == 4);
        queue.pop();
        REQUIRE(queue.size() == 3);
        queue.push(9);
        REQUIRE(queue.size() == 4);
    }
}

TEST_CASE("BufferQueue clear", "[BufferQueue]")
{
    buffer_queue<int> queue;
    SECTION("Clear elements")
    {
        queue.push(1);
        queue.push(2);
        queue.push(3);
        queue.clear();
        REQUIRE(queue.size() == 0);
        REQUIRE(queue.is_empty());
    }
}