#include "Engine/Precompiled/EnginePch.hpp"
#include "NodeScriptDataTypes.hpp"
#include <string>


// Bool
Fly::ViewAndEditResult ViewAndEdit(bool& aValue)
{
	ImGui::Checkbox("##", &aValue);
	Fly::ViewAndEditResult result;
	result.mIsItemActive = ImGui::IsItemActive();
	return result;
}

void View(const bool& aValue)
{
	bool v = aValue;
	ImGui::BeginDisabled();
	ImGui::Checkbox("##", &v);
	ImGui::EndDisabled();
}

void Save(const bool& aValue, nlohmann::json& aJson)
{
	aJson["Value"] = aValue;
}

void Load(bool& aValue, const nlohmann::json& aJson)
{
	aValue = aJson["Value"];
}

// Int
Fly::ViewAndEditResult ViewAndEdit(int& aValue)
{
	ImGui::DragInt("##", &aValue);
	Fly::ViewAndEditResult result;
	result.mIsItemActive = ImGui::IsItemActive();
	return result;
}

void View(const int& aValue)
{
	int v = aValue;
	ImGui::BeginDisabled();
	ImGui::DragInt("##", &v);
	ImGui::EndDisabled();
}

void Save(const int& aValue, nlohmann::json& aJson)
{
	aJson["Value"] = aValue;
}

void Load(int& aValue, const nlohmann::json& aJson)
{
	aValue = aJson["Value"];
}

// Float
Fly::ViewAndEditResult ViewAndEdit(float& aValue)
{
	ImGui::DragFloat("##", &aValue);
	Fly::ViewAndEditResult result;
	result.mIsItemActive = ImGui::IsItemActive();
	return result;
}

void View(const float& aValue)
{
	float v = aValue;
	ImGui::BeginDisabled();
	ImGui::DragFloat("##", &v);
	ImGui::EndDisabled();
}

void Save(const float& aValue, nlohmann::json& aJson)
{
	aJson["Value"] = aValue;
}

void Load(float& aValue, const nlohmann::json& aJson)
{
	aValue = aJson["Value"];
}

// Unsigned Int
Fly::ViewAndEditResult ViewAndEdit(uint32& aValue)
{
	int intValue = static_cast<int>(aValue);
	if (ImGui::DragInt("##", &intValue, 1.f, 0, INT_MAX))
	{
		aValue = static_cast<uint32>(intValue);
	}
	Fly::ViewAndEditResult result;
	result.mIsItemActive = ImGui::IsItemActive();
	return result;
}

void View(const uint32& aValue)
{
	int intValue = static_cast<int>(aValue);
	ImGui::BeginDisabled();
	ImGui::DragInt("##", &intValue, 1.f, 0, INT_MAX);
	ImGui::EndDisabled();
}

void Save(const uint32& aValue, nlohmann::json& aJson)
{
	aJson["Value"] = aValue;
}

void Load(uint32& aValue, const nlohmann::json& aJson)
{
	aValue = aJson["Value"];
}

// Unsigned Long Long
Fly::ViewAndEditResult ViewAndEdit(uint64& aValue)
{
	int intValue = static_cast<int>(aValue);
	if (ImGui::DragInt("##", &intValue, 1.f, 0, INT_MAX))
	{
		aValue = static_cast<uint64>(intValue);
	}

	Fly::ViewAndEditResult result;
	result.mIsItemActive = ImGui::IsItemActive();
	return result;
}

void View(const uint64& aValue)
{
	int intValue = static_cast<int>(aValue);
	ImGui::BeginDisabled();
	ImGui::DragInt("##", &intValue, 1.f, 0, INT_MAX);
	ImGui::EndDisabled();
}

void Save(const uint64& aValue, nlohmann::json& aJson)
{
	aJson["Value"] = aValue;
}

void Load(uint64& aValue, const nlohmann::json& aJson)
{
	aValue = aJson["Value"];
}

// Char
Fly::ViewAndEditResult ViewAndEdit(char& aValue)
{
	char c[2] = { aValue, '\0' };

	if (ImGui::InputText("##", c, 2))
	{
		aValue = c[0];
	}
	Fly::ViewAndEditResult result;
	result.mIsItemActive = ImGui::IsItemActive();
	return result;
}

void View(const char& aValue)
{
	char c[2] = { aValue, '\0' };
	ImGui::BeginDisabled();
	ImGui::InputText("##", c, 2);
	ImGui::EndDisabled();
}

void Save(const char& aValue, nlohmann::json& aJson)
{
	int i = aValue;
	aJson["char"] = i;
}

void Load(char& aValue, const nlohmann::json& aJson)
{
	int i = aJson["char"];
	aValue = (char)i;
}

namespace std
{

	Fly::ViewAndEditResult ViewAndEdit(std::string& aValue)
	{
		char buffer[32]{};
		strcpy_s(buffer, aValue.c_str());

		if (ImGui::InputText("##", buffer, IM_ARRAYSIZE(buffer)))
		{
			aValue = buffer;
		}
		Fly::ViewAndEditResult result;
		result.mIsItemActive = ImGui::IsItemActive();
		return result;
	}

	void View(const std::string& aValue)
	{
		char buffer[32]{};
		strcpy_s(buffer, aValue.c_str());

		ImGui::BeginDisabled();
		ImGui::InputText("##", buffer, IM_ARRAYSIZE(buffer));
		ImGui::EndDisabled();
	}

	void Save(const std::string& aValue, nlohmann::json& aJson)
	{
		aJson["Value"] = aValue;
	}

	void Load(std::string& aValue, const nlohmann::json& aJson)
	{
		aValue = aJson["Value"];
	}
}

void RegisterFundamentalDataTypes()
{
	Fly::RegisterType::ValueType<bool>("bool", Fly::CustomName{ "Bool" }, Fly::Color(1.f, 0.1f, 0.1f));;
	Fly::RegisterType::ValueType<int>("int", Fly::CustomName{ "Int" }, Fly::Color(0.f, 0.2f, 1.f));;
	Fly::RegisterType::ValueType<float>("float", Fly::CustomName{ "Float" }, Fly::Color(0.55f, 0.2f, 0.65f));;
	Fly::RegisterType::ValueType<uint32>("uint32", Fly::CustomName{ "UInt32" }, Fly::Color(0.f, 0.4f, 1.f));;
	Fly::RegisterType::ValueType<uint64>("uint64", Fly::CustomName{ "UInt64" }, Fly::Color(0.1f, 0.5f, 1.f));;
	Fly::RegisterType::ValueType<char>("char", Fly::CustomName{ "Char" }, Fly::Color(0.2f, 0.7f, 0.4f));;
	Fly::RegisterType::ValueType<std::string>("std::string", Fly::CustomName{ "String" }, Fly::Color(0.3f, 0.8f, 0.2f));;
	Fly::RegisterType::ValueType<Simple::Vector2f>("Vector", Fly::Color(0.3f, 0.8f, 0.2f));
}