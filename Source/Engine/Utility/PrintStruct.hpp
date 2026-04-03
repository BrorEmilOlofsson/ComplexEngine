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

        ~PrintStruct()
        {
            std::println("PrintStruct destructor");
        }

        PrintStruct(const PrintStruct&)
        {
            std::println("PrintStruct copy constructor");
        }

        PrintStruct& operator=(const PrintStruct&)
        {
            std::println("PrintStruct copy assignment operator");
            return *this;
        }

        PrintStruct(PrintStruct&&)
        {
            std::println("PrintStruct move constructor");
        }

        PrintStruct& operator=(PrintStruct&&)
        {
            std::println("PrintStruct move assignment operator");
            return *this;
        }
    };
}