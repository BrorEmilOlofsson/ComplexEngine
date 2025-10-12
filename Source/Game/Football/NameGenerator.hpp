#pragma once
#include <string>
#include <random>
#include <span>
#include "Random.hpp"

constexpr std::string GenerateName(const std::span<const std::string_view> pFirstNames, const std::span<const std::string_view> pLastNames)
{
    if (empty(pFirstNames) || empty(pLastNames))
    {
        return "Null Name";
    }
    
    const std::size_t firstNameIndex = GetRandomNumber(std::size_t{ 0 }, size(pFirstNames) - 1);
    const std::size_t lastNameIndex = GetRandomNumber(std::size_t{ 0 }, size(pLastNames) - 1);

    return std::string(pFirstNames[firstNameIndex]) + " " + std::string(pLastNames[lastNameIndex]);
}