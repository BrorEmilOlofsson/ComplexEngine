#pragma once
#include "../FlyDefines.hpp"
#include "../FlyRegistration.hpp"
#include "../Instance/FlyStructInstance.hpp"
#include <External/imgui/imgui.h>

namespace std
{
	template<typename T, typename... Rest>
	Fly::ViewAndEditResult ViewAndEditVariantType(auto& aVariant)
	{
		const Fly::DataTypeID dataTypeID = Fly::GetDataTypeID<T>();
		const Fly::DataType* dataType = Fly::Internal::GetDataTypeByID(dataTypeID);

		if (dataType)
		{

			if (T* value = std::get_if<T>(&aVariant))
			{
				return Fly::Internal::GetDataTypeManager().ViewAndEditData(dataTypeID, value, true);
			}
		}

		if constexpr (sizeof...(Rest) > 0)
		{
			return ViewAndEditVariantType<Rest...>(aVariant);
		}

		return Fly::ViewAndEditResult{};
	}

	template<typename T, typename... Rest>
	void ShowSelectableVariantTypes(auto& aVariant, const size_t aCurrentIndex = 0)
	{
		const std::string& name = Fly::Internal::GetDataTypeManager().Find<T>()->Name();

		const bool isSelected = aCurrentIndex == aVariant.index();
		if (ImGui::Selectable(name.c_str(), isSelected))
		{
			if (!isSelected)
			{
				aVariant = T{};
			}
		}

		if constexpr (sizeof...(Rest) > 0)
		{
			ShowSelectableVariantTypes<Rest...>(aVariant, aCurrentIndex + 1);
		}
	}

	template<typename... Types>
	Fly::ViewAndEditResult ViewAndEdit(std::variant<Types...>& aVariant)
	{
		Fly::ViewAndEditResult viewAndEditResult;
		const Fly::DataTypeManager& dataTypeManager = Fly::Internal::GetDataTypeManager();

		const std::string_view name = std::visit([&dataTypeManager](const auto& aValue) -> std::string_view
			{
				const Fly::DataTypeID dataTypeID = Fly::GetDataTypeID<decltype(aValue)>();
				if (const Fly::DataType* dataType = dataTypeManager.Find(dataTypeID))
				{
					return dataType->Name();
				}
				else
				{
					return "Null";
				}
			}, aVariant);
		if (ImGui::BeginCombo("##VariantType", std::string(name).c_str()))
		{

			ShowSelectableVariantTypes<Types...>(aVariant);
			viewAndEditResult.mIsItemActive = true;

			ImGui::EndCombo();
		}



		viewAndEditResult.mIsItemActive |= ViewAndEditVariantType<Types...>(aVariant).mIsItemActive;

		return viewAndEditResult;
	}
}

namespace FLY_NAMESPACE
{

	using TestVariant = std::variant<int, float, std::string>;

	inline Fly::RegisterType fly_valuetype11 = Fly::RegisterType::ValueType<TestVariant, eNodeOperatorTrait::None>("TestVariant", CustomName{ "TestVariant" });;



	void TestUtilityFunction();

	ViewAndEditResult ViewAndEdit(StructInstance& aStructInstance);

	StructInstance StructInstanceTest(const StructInstance& aStructInstance);

	FLY_FUNCTION(StructInstanceTest);

	inline Fly::RegisterType fly_valuetype191 = Fly::RegisterType::ValueType<StructInstance>("StructInstance");


}