#pragma once
#include <string>
#include <random>
#include <span>
#include "Engine/Utility/Random.hpp"

constexpr std::string GenerateName(const std::span<const std::string_view> pFirstNames, const std::span<const std::string_view> pLastNames)
{
    if (empty(pFirstNames) || empty(pLastNames))
    {
        return "Null Name";
    }
    
    auto randEngine = CLX::CreateDefaultRandomEngine();
    const std::size_t firstNameIndex = CLX::GetRandomNumber(std::size_t{ 0 }, size(pFirstNames) - 1, randEngine);
    const std::size_t lastNameIndex = CLX::GetRandomNumber(std::size_t{ 0 }, size(pLastNames) - 1, randEngine);

    return std::string(pFirstNames[firstNameIndex]) + " " + std::string(pLastNames[lastNameIndex]);
}