#pragma once
#include <vector>
#include <External/imgui/imgui.h>
#include "NodeScript/Fly.hpp"

namespace FLY_NAMESPACE
{
	struct FlyCustomVectorType
	{
		template<typename T>
		using type = std::vector<T>;
	};
}

Fly::ViewAndEditResult ViewAndEditDataPtr(Fly::DataTypeID dataTypeID, void* dataPtr);
void SaveDataPtr(Fly::DataTypeID dataTypeID, const void* dataPtr, nlohmann::json& json);
void LoadDataPtr(Fly::DataTypeID dataTypeID, void* dataPtr, const nlohmann::json& json);


// Check if a type has `value_type` and `begin/end` member functions
template <typename T>
concept Container = requires(T a) 
{
	typename T::value_type;
	{ a.begin() };
	{ a.end() };
};

namespace std
{

	template<Container T>
	Fly::ViewAndEditResult ViewAndEdit(T& container)
	{
		using ValueType = typename T::value_type;
		static size_t currentPopupIndex = 0;
		static constexpr const char* VectorElementPopupStrID = "Vector Element Popup";
		Fly::ViewAndEditResult result;

		for (auto it = begin(container); it != end(container); ++it)
		{
			ValueType& data = *it;
			ImGui::PushID(&data);

			result.mIsItemActive |= ViewAndEditDataPtr(Fly::GetDataTypeID<ValueType>(), &data).mIsItemActive;


			ImGui::SameLine();

			const bool vectorElementBtnWasPressed = ImGui::Button("...");
			ImGui::PopID();

			if (vectorElementBtnWasPressed)
			{
				ImGui::OpenPopup(VectorElementPopupStrID);
				currentPopupIndex = std::distance(begin(container), it);
			}
		}

		if (ImGui::BeginPopup(VectorElementPopupStrID))
		{
			if (ImGui::MenuItem("Delete"))
			{
				container.erase(begin(container) + currentPopupIndex);
			}

			if (ImGui::MenuItem("Insert Before"))
			{
				container.insert(begin(container) + currentPopupIndex, ValueType());
			}

			if (ImGui::MenuItem("Reset Value"))
			{
				container[currentPopupIndex] = ValueType();
			}

			result.mIsItemActive = true;

			ImGui::EndPopup();
		}

		if (ImGui::Button("Add"))
		{
			container.emplace_back();

			result.mIsItemActive |= ImGui::IsItemActive();
		}

		return result;
	}

	/*template<typename T>
	Fly::ViewAndEditResult ViewAndEdit(std::vector<T>& aVector)
	{
		static size_t currentPopupIndex = 0;
		static constexpr const char* VectorElementPopupStrID = "Vector Element Popup";
		Fly::ViewAndEditResult result;
		for (size_t i = 0; i < aVector.size(); ++i)
		{
			ImGui::PushID((std::string(typeid(std::vector<T>).name()) + std::to_string(i)).c_str());

			T& data = aVector[i];
			result.mIsItemActive |= ViewAndEditDataPtr(Fly::GetDataTypeID<T>(), &data).mIsItemActive;

			ImGui::PopID();

			ImGui::SameLine();

			ImGui::PushID(&data);
			const bool vectorElementBtnWasPressed = ImGui::Button("...");
			ImGui::PopID();

			if (vectorElementBtnWasPressed)
			{
				ImGui::OpenPopup(VectorElementPopupStrID);
				currentPopupIndex = i;
			}

		}

		if (ImGui::BeginPopup(VectorElementPopupStrID, ImGuiWindowFlags_ChildWindow))
		{
			if (ImGui::MenuItem("Delete"))
			{
				aVector.erase(begin(aVector) + currentPopupIndex);
			}

			if (ImGui::MenuItem("Insert Before"))
			{
				aVector.insert(begin(aVector) + currentPopupIndex, T());
			}

			if (ImGui::MenuItem("Reset Value"))
			{
				aVector[currentPopupIndex] = T();
			}

			result.mIsItemActive = true;

			ImGui::EndPopup();
		}

		if (ImGui::Button("Add"))
		{
			aVector.emplace_back();

			result.mIsItemActive |= ImGui::IsItemActive();
		}

		return result;
	}*/

	template<typename T>
	void Save(const std::vector<T>& aVector, nlohmann::json& aJson)
	{
		nlohmann::json arrayJson = nlohmann::json::array();
		for (const T& data : aVector)
		{
			nlohmann::json element;

			SaveDataPtr(Fly::GetDataTypeID<T>(), &data, element);

			arrayJson.push_back(element);
		}
		aJson["Values"] = arrayJson;
	}

	template<typename T>
	void Load(std::vector<T>& aVector, const nlohmann::json& aJson)
	{
		const nlohmann::json& arrayJson = aJson["Values"];
		for (const nlohmann::json& element : arrayJson)
		{
			T& data = aVector.emplace_back();

			LoadDataPtr(Fly::GetDataTypeID<T>(), &data, element);
		}
	}

}