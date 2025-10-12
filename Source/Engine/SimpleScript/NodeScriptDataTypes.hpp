#pragma once
#include "NodeScript/FlyRegistration.hpp"
#include "NodeScriptCustomTypes.hpp"
#include <External/imgui/imgui.h>
#include <nlohmann/json.hpp>

using uint32 = unsigned int;
using uint64 = unsigned long long;

Fly::ViewAndEditResult ViewAndEdit(bool& aValue);
void View(const bool& aValue);
void Save(const bool& aValue, nlohmann::json& aJson);
void Load(bool& aValue, const nlohmann::json& aJson);

Fly::ViewAndEditResult ViewAndEdit(int& aValue);
void View(const int& aValue);
void Save(const int& aValue, nlohmann::json& aJson);
void Load(int& aValue, const nlohmann::json& aJson);

Fly::ViewAndEditResult ViewAndEdit(float& aValue);
void View(const float& aValue);
void Save(const float& aValue, nlohmann::json& aJson);
void Load(float& aValue, const nlohmann::json& aJson);

Fly::ViewAndEditResult ViewAndEdit(uint32& aValue);
void View(const uint32& aValue);
void Save(const uint32& aValue, nlohmann::json& aJson);
void Load(uint32& aValue, const nlohmann::json& aJson);

Fly::ViewAndEditResult ViewAndEdit(uint64& aValue);
void View(const uint64& aValue);
void Save(const uint64& aValue, nlohmann::json& aJson);
void Load(uint64& aValue, const nlohmann::json& aJson);

Fly::ViewAndEditResult ViewAndEdit(char& aValue);
void View(const char& aValue);
void Save(const char& aValue, nlohmann::json& aJson);
void Load(char& aValue, const nlohmann::json& aJson);


namespace std
{
	Fly::ViewAndEditResult ViewAndEdit(std::string& aValue);
	void View(const std::string& aValue);
	void Save(const std::string& aValue, nlohmann::json& aJson);
	void Load(std::string& aValue, const nlohmann::json& aJson);
}

void RegisterFundamentalDataTypes();