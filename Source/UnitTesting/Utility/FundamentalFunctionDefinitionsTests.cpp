#include <External/Catch2/catch_amalgamated.hpp>
#include <string>
#include <type_traits>

#include "Engine/Utility/FundamentalFunctionDefinitions.hpp"

using namespace CLX;

struct TrivialType
{
    int value = 2;
};

struct NonTrivialType
{
    std::string value = "HelloThereStringStringStringStringStringStringString";
};

TEST_CASE("Inplace construct function")
{
    {
        // Trivial test

        static_assert(std::is_trivially_copyable_v<TrivialType>);

        InplaceConstructFunction f = CreateInplaceConstructFunction<TrivialType>();
        {
            std::byte bytes[sizeof(TrivialType)]{};
            void* firstByte = &bytes[0];
            f(firstByte, nullptr);

            TrivialType& s = *static_cast<TrivialType*>(firstByte);

            REQUIRE(s.value == 2);
        }

        {
            std::byte bytes[sizeof(TrivialType)]{};
            void* firstByte = &bytes[0];
            TrivialType defaultValue{ 12 };
            f(firstByte, &defaultValue);

            TrivialType& s = *static_cast<TrivialType*>(firstByte);

            REQUIRE(s.value == 12);
        }
    }

    {
        // Non-trivial test

        static_assert(!std::is_trivially_copyable_v<NonTrivialType>);

        InplaceConstructFunction f = CreateInplaceConstructFunction<NonTrivialType>();
        {
            std::byte bytes[sizeof(NonTrivialType)]{};
            void* firstByte = &bytes[0];
            f(firstByte, nullptr);

            NonTrivialType& s = *static_cast<NonTrivialType*>(firstByte);

            REQUIRE(s.value == "HelloThereStringStringStringStringStringStringString");
        }

        {
            std::byte bytes[sizeof(NonTrivialType)]{};
            void* firstByte = &bytes[0];
            NonTrivialType defaultValue{ "HelloThereStringStringStringStringStringStringString2" };
            f(firstByte, &defaultValue);

            NonTrivialType& s = *static_cast<NonTrivialType*>(firstByte);

            REQUIRE(s.value == "HelloThereStringStringStringStringStringStringString2");
        }
    }
}

struct DestroyTrivialType
{
    ~DestroyTrivialType()
    {
        wasCalled.get() = true;
    }

    std::reference_wrapper<bool> wasCalled;
};


struct DestroyNonTrivialType
{
    ~DestroyNonTrivialType()
    {
        wasCalled.get() = true;
    }

    std::reference_wrapper<bool> wasCalled;
    NonTrivialType nonTrivialType;
};

TEST_CASE("Destroy function")
{
    {
        // Trivial test

        static_assert(std::is_trivially_copyable_v<TrivialType>);

        DestroyFunction f = CreateDestroyFunction<TrivialType>();
        {
            TrivialType trivial;
            f(&trivial);

            REQUIRE(trivial.value == 2);
        }
    }

    {
        // Non-trivial test

        static_assert(!std::is_trivially_copyable_v<NonTrivialType>);

        DestroyFunction f = CreateDestroyFunction<NonTrivialType>();
        {
            NonTrivialType nonTrivial;
            f(&nonTrivial);

            REQUIRE(nonTrivial.value.empty());
        }
    }

    {
        {

            // Test that the destructor is called

            DestroyFunction f = CreateDestroyFunction<DestroyTrivialType>();
            bool wasCalled = false;
            DestroyTrivialType value{ std::ref(wasCalled) };

            f(&value);

            REQUIRE(wasCalled);
        }

        {
            // Test that the destructor is called

            DestroyFunction f = CreateDestroyFunction<DestroyNonTrivialType>();
            bool wasCalled = false;
            DestroyNonTrivialType value{ std::ref(wasCalled) };

            f(&value);

            REQUIRE(wasCalled);

        }
    }
}

TEST_CASE("Copy construct function")
{
    {
        // Trivial test

        static_assert(std::is_trivially_copyable_v<TrivialType>);

        CopyConstructFunction f = CreateCopyConstructFunction<TrivialType>();
        {
            std::byte bytes[sizeof(TrivialType)]{};
            void* firstByte = &bytes[0];
            const TrivialType source{ 50 };
            f(firstByte, &source);

            TrivialType& trivial = *static_cast<TrivialType*>(firstByte);

            REQUIRE(trivial.value == 50);
            REQUIRE(source.value == 50);
        }
    }

    {
        // Non-trivial test

        static_assert(!std::is_trivially_copyable_v<NonTrivialType>);

        CopyConstructFunction f = CreateCopyConstructFunction<NonTrivialType>();
        {
            std::byte bytes[sizeof(NonTrivialType)]{};
            void* firstByte = &bytes[0];
            const NonTrivialType source{ "SFDKFFSJF" };
            f(firstByte, &source);

            NonTrivialType& nonTrivial = *static_cast<NonTrivialType*>(firstByte);

            REQUIRE(nonTrivial.value == "SFDKFFSJF");
            REQUIRE(source.value == "SFDKFFSJF");
        }
    }
}

TEST_CASE("Copy function")
{
    {
        // Trivial test

        static_assert(std::is_trivially_copyable_v<TrivialType>);

        CopyFunction f = CreateCopyFunction<TrivialType>();
        {
            TrivialType trivial;
            TrivialType source{ 50 };
            f(&trivial, &source);

            REQUIRE(trivial.value == 50);
            REQUIRE(source.value == 50);
        }
    }

    {
        // Non-trivial test

        static_assert(!std::is_trivially_copyable_v<NonTrivialType>);

        CopyFunction f = CreateCopyFunction<NonTrivialType>();
        {
            NonTrivialType nonTrivial;
            NonTrivialType source{ "SFDKFFSJF" };
            f(&nonTrivial, &source);

            REQUIRE(nonTrivial.value == "SFDKFFSJF");
            REQUIRE(source.value == "SFDKFFSJF");
        }
    }
}

TEST_CASE("Move construct function")
{
    {
        // Trivial test

        static_assert(std::is_trivially_copyable_v<TrivialType>);

        MoveConstructFunction f = CreateMoveConstructFunction<TrivialType>();
        {
            std::byte bytes[sizeof(TrivialType)]{};
            void* firstByte = &bytes[0];
            TrivialType source{ 50 };
            f(firstByte, &source);

            TrivialType& trivial = *static_cast<TrivialType*>(firstByte);

            REQUIRE(trivial.value == 50);
            REQUIRE(source.value == 50);
        }
    }

    {
        // Non-trivial test

        static_assert(!std::is_trivially_copyable_v<NonTrivialType>);

        MoveConstructFunction f = CreateMoveConstructFunction<NonTrivialType>();
        {
            std::byte bytes[sizeof(NonTrivialType)]{};
            void* firstByte = &bytes[0];
            NonTrivialType source{ "SFDKFFSJF" };
            f(firstByte, &source);

            NonTrivialType& nonTrivial = *static_cast<NonTrivialType*>(firstByte);

            REQUIRE(nonTrivial.value == "SFDKFFSJF");
            REQUIRE(source.value.empty());
        }
    }
}

TEST_CASE("Move function")
{
    {
        // Trivial test

        static_assert(std::is_trivially_copyable_v<TrivialType>);

        MoveFunction f = CreateMoveFunction<TrivialType>();
        {
            TrivialType trivial;
            TrivialType source{ 50 };
            f(&trivial, &source);

            REQUIRE(trivial.value == 50);
            REQUIRE(source.value == 50);
        }

    }

    {
        // Non-trivial test

        static_assert(!std::is_trivially_copyable_v<NonTrivialType>);

        MoveFunction f = CreateMoveFunction<NonTrivialType>();
        {
            NonTrivialType nonTrivial;
            NonTrivialType source{ "SFDKFFSJF" };
            f(&nonTrivial, &source);

            REQUIRE(nonTrivial.value == "SFDKFFSJF");
            REQUIRE(source.value.empty());
        }
    }
}

TEST_CASE("Swap function")
{
    {
        // Trivial test

        static_assert(std::is_trivially_copyable_v<TrivialType>);

        {
            SwapFunction f = CreateSwapFunction<TrivialType>();
            TrivialType t1{ 30 };
            TrivialType t2{ 50 };
            f(&t1, &t2);

            REQUIRE(t1.value == 50);
            REQUIRE(t2.value == 30);
        }

        {
            SwapFunction f = CreateSwapFunction<NonTrivialType>();
            NonTrivialType t1{ "Hello1" };
            NonTrivialType t2{ "Hello2" };
            f(&t1, &t2);

            REQUIRE(t1.value == "Hello2");
            REQUIRE(t2.value == "Hello1");
        }

    }
}