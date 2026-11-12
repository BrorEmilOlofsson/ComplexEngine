#pragma once
#include <print>
#include "Engine/Utility/Assert.hpp"

namespace CLX
{

    struct PrintStruct final
    {
        PrintStruct()
        {
            std::println("PrintStruct default constructor");
        }

        PrintStruct(std::string name)
            : test(std::move(name))
        {
            std::println("PrintStruct parameterized constructor");
        }

        ~PrintStruct()
        {

            std::println("PrintStruct destructor");
        }

        PrintStruct(const PrintStruct& other)
            : test(other.test)
        {
            std::println("PrintStruct copy constructor");
        }

        PrintStruct& operator=(const PrintStruct& other)
        {
            test = other.test;
            std::println("PrintStruct copy assignment operator");
            return *this;
        }

        PrintStruct(PrintStruct&& other)
            : test(std::move(other.test))
        {
            std::println("PrintStruct move constructor");
        }

        PrintStruct& operator=(PrintStruct&& other)
        {
            test = std::move(other.test);
            std::println("PrintStruct move assignment operator");
            return *this;
        }

        const std::string& GetString() const
        {
            return test;
        }

    private:

        std::string test = "fddffldskfldsfksdöfsdkjfdkjfkl";
    };



    struct CounterStruct final
    {
        struct CounterData final
        {
            size_t constructorCount = 0;
            size_t destructorCount = 0;
            size_t copyConstructorCount = 0;
            size_t copyAssignmentCount = 0;
            size_t moveConstructorCount = 0;
            size_t moveAssignmentCount = 0;
        };

        [[nodiscard]] static std::shared_ptr<CounterData> CreateCounter()
        {
            return std::make_shared<CounterData>();
        }

        CounterStruct() = default;

        CounterStruct(std::weak_ptr<CounterData> counter)
            : mCounterData(std::move(counter))
        {
            ASSERT(mCounterData.expired() == false, "CounterStruct: Counter is expired");
            if (auto counterPtr = mCounterData.lock())
            {
                counterPtr->constructorCount++;
            }
        }

        CounterStruct(std::string name, std::weak_ptr<CounterData> counter)
            : test(std::move(name))
            , mCounterData(std::move(counter))
        {
            ASSERT(mCounterData.expired() == false, "CounterStruct: Counter is expired");
            if (auto counterPtr = mCounterData.lock())
            {
                counterPtr->constructorCount++;
            }
        }

        ~CounterStruct()
        {
            if (auto counterPtr = mCounterData.lock())
            {
                counterPtr->destructorCount++;
            }
        }

        CounterStruct(const CounterStruct& other)
            : test(other.test)
            , mCounterData(other.mCounterData)
        {
            if (auto counterPtr = mCounterData.lock())
            {
                counterPtr->copyConstructorCount++;
            }
        }

        CounterStruct& operator=(const CounterStruct& other)
        {
            test = other.test;
            mCounterData = other.mCounterData;
            if (auto counterPtr = mCounterData.lock())
            {
                counterPtr->copyAssignmentCount++;
            }
            return *this;
        }

        CounterStruct(CounterStruct&& other) noexcept
            : test(std::move(other.test))
            , mCounterData(other.mCounterData)
        {
            other.mHasMoved = true;
            if (auto counterPtr = mCounterData.lock())
            {
                counterPtr->moveConstructorCount++;
            }
        }

        CounterStruct& operator=(CounterStruct&& other) noexcept
        {
            test = std::move(other.test);
            mCounterData = other.mCounterData;
            other.mHasMoved = true;
            if (auto counterPtr = mCounterData.lock())
            {
                counterPtr->moveAssignmentCount++;
            }
            return *this;
        }

        [[nodiscard]] std::weak_ptr<CounterData> GetCounter() const
        {
            return mCounterData;
        }

        [[nodiscard]] const std::string& GetString() const
        {
            return test;
        }

        [[nodiscard]] bool HasMoved() const
        {
            return mHasMoved;
        }

        bool operator==(const CounterStruct& other) const
        {
            return test == other.test && mCounterData.lock() == other.mCounterData.lock() && mHasMoved == other.mHasMoved;
        }

        static constexpr const std::string_view DefaultString = "Default Value fdfdfsdfldfl";

    private:

        std::string test = std::string(DefaultString);
        std::weak_ptr<CounterData> mCounterData;
        bool mHasMoved = false;
    };
}