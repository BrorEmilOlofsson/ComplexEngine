#pragma once
#include <print>

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

    struct CounterStructCounter final
    {
        size_t constructorCount = 0;
        size_t destructorCount = 0;
        size_t copyConstructorCount = 0;
        size_t copyAssignmentCount = 0;
        size_t moveConstructorCount = 0;
        size_t moveAssignmentCount = 0;
    };

    struct CounterStruct final
    {
        CounterStruct() = default;

        CounterStruct(std::weak_ptr<CounterStructCounter> counter)
            : mCounter(std::move(counter))
        {
            ASSERT(mCounter.expired() == false, "CounterStruct: Counter is expired");
            if (auto counterPtr = mCounter.lock())
            {
                counterPtr->constructorCount++;
            }
        }

        CounterStruct(std::string name, std::weak_ptr<CounterStructCounter> counter)
            : test(std::move(name))
            , mCounter(std::move(counter))
        {
            ASSERT(mCounter.expired() == false, "CounterStruct: Counter is expired");
            if (auto counterPtr = mCounter.lock())
            {
                counterPtr->constructorCount++;
            }
        }

        ~CounterStruct()
        {
            if (auto counterPtr = mCounter.lock())
            {
                counterPtr->destructorCount++;
            }
        }

        CounterStruct(const CounterStruct& other)
            : test(other.test)
            , mCounter(other.mCounter)
        {
            if (auto counterPtr = mCounter.lock())
            {
                counterPtr->copyConstructorCount++;
            }
        }

        CounterStruct& operator=(const CounterStruct& other)
        {
            test = other.test;
            mCounter = other.mCounter;
            if (auto counterPtr = mCounter.lock())
            {
                counterPtr->copyAssignmentCount++;
            }
            return *this;
        }

        CounterStruct(CounterStruct&& other)
            : test(std::move(other.test))
            , mCounter(std::move(other.mCounter))
        {
            if (auto counterPtr = mCounter.lock())
            {
                counterPtr->moveConstructorCount++;
            }
        }

        CounterStruct& operator=(CounterStruct&& other)
        {
            test = std::move(other.test);
            mCounter = std::move(other.mCounter);
            if (auto counterPtr = mCounter.lock())
            {
                counterPtr->moveAssignmentCount++;
            }
            return *this;
        }

        std::weak_ptr<CounterStructCounter> GetCounter() const
        {
            return mCounter;
        }

        const std::string& GetString() const
        {
            return test;
        }

    private:

        std::string test = "fddffldskfldsfksdöfsdkjfdkjfkl";
        std::weak_ptr<CounterStructCounter> mCounter;
    };
}