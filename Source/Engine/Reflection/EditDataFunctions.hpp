#pragma once
#include "Engine/Reflection/MemberVariable.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Transform3.hpp"
#include "Engine/Math/UnitVector2.hpp"
#include "Engine/Math/UnitVector3.hpp"
#include "Engine/Utility/Color.hpp"
#include "Engine/Utility/RGBColor.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"
#include "Engine/Math/Shapes/AABB3.hpp"
#include "Engine/Math/Shapes/Cylinder.hpp"
#include "Engine/Math/Shapes/Ray3.hpp"
#include "Engine/ECS/EntityID.hpp"
#include "Engine/Reflection/DataTypeID.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/Utility/EnumUtility.hpp"
#include "Engine/Reflection/ViewAndEditResult.hpp"
#include "Engine/Asset/AssetTypes/AssetTypes.hpp"
#include "Engine/Graphics/Light/PointLight.hpp"
#include "Engine/Graphics/Light/DirectionalLight.hpp"
#include "Engine/Utility/Camera.hpp"

#include <string>
#include <array>
#include <External/imgui/imgui.h>

	//CLX::ViewAndEditResult ViewAndEditValue(float& value, const std::string& variableName);
namespace CLX
{

	ViewAndEditResult ViewAndEditDataPtr(const DataTypeID dataTypeID, void* dataPtr, const Blackboard& blackboard);

	ViewAndEditResult ViewAndEditValue(char& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(char& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(bool& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(bool& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(int& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(int& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(float& value, const std::string& variableName, float speed, float min, float max);
	ViewAndEditResult ViewAndEditValue(float& value, const std::string& variableName);
	ViewAndEditResult ViewAndEditValue(float& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(std::string& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(std::string& value, const DataTypeMemberVariable* memberData = nullptr);

	ViewAndEditResult ViewAndEditValue(Vector2f& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(Vector2f& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(Vector2i& value, const std::string& variableName);
	inline ViewAndEditResult ViewAndEditValue(Vector2u& value, const std::string& variableName)
	{
        auto v = Vector2i(value);
		auto viewAndEditResult = ViewAndEditValue(v, variableName);
        value = Vector2u(v);
		return viewAndEditResult;
	}
	//ViewAndEditResult ViewAndEditValue(Vector2i& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(Vector3f& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(Vector3f& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(Vector4f& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(Vector4f& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(Point2f& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(Point2f& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(Point2i& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(Point2i& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(Point3f& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(Point3f& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(UnitVector2f& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(UnitVector2f& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult VisualizeInScene(UnitVector3f& value, const Point3f& origin, const Camera& camera, class RenderList& renderList);
	ViewAndEditResult ViewAndEditValue(UnitVector3f& value, const Blackboard& blackboard, const DataTypeMemberVariable* memberData);
	ViewAndEditResult ViewAndEditValue(Color& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(Color& value, const DataTypeMemberVariable* memberData = nullptr);
	ViewAndEditResult ViewAndEditValue(RGBColor& value, const std::string& variableName);
	//ViewAndEditResult ViewAndEditValue(RGBColor& value, const DataTypeMemberVariable* memberData = nullptr);
	
	template<typename T>
		requires requires(T& value)
	{
		{ ViewAndEditValue(value, std::string{}) } -> std::same_as<ViewAndEditResult>;
	}
    ViewAndEditResult ViewAndEditValue(T& value, const DataTypeMemberVariable* memberData)
	{
		return ViewAndEditValue(value, memberData ? memberData->customName : "");
	}

	template<typename T>
	ViewAndEditResult ViewAndEditValue(Degrees<T>& degrees, const std::string& variableName)
	{
		return ViewAndEditValue(degrees.Value(), variableName);
	}

	template<typename T>
	ViewAndEditResult ViewAndEditValue(Radians<T>& radians, const std::string& variableName)
	{
		Degrees<T> degrees = ToDegrees(radians);
		auto v = ViewAndEditValue(degrees, variableName);
		radians = ToRadians(degrees);
		return v;
	}

	template<typename T>
	ViewAndEditResult ViewAndEditValue(AABB2<T>& aabb, const std::string&)
	{
		ViewAndEditResult viewAndEditResult;

		{
			Point2<T> center = aabb.GetCenter();
			ViewAndEditResult centerViewAndEditResult = ViewAndEditValue(center, "Center");
			viewAndEditResult.isEdited |= centerViewAndEditResult.isEdited;
			viewAndEditResult.isActive |= centerViewAndEditResult.isActive;
			if (centerViewAndEditResult.isEdited)
			{
				aabb.SetCenter(center);
			}
		}
		{
			auto extent = aabb.GetExtent();
			ViewAndEditResult extentViewAndEditResult = ViewAndEditValue(extent, "Extent");
			viewAndEditResult.isEdited |= extentViewAndEditResult.isEdited;
			viewAndEditResult.isActive |= extentViewAndEditResult.isActive;
			if (extentViewAndEditResult.isEdited)
			{
				aabb.SetExtent(extent);
			}
		}

		return viewAndEditResult;
	}

	template<typename T>
    ViewAndEditResult ViewAndEditValue(Cylinder<T>& cylinder, const Blackboard& blackboard)
	{
		ViewAndEditResult viewAndEditResult;
		{
			T height = cylinder.GetHeight();
			ViewAndEditResult heightViewAndEditResult = ViewAndEditValue(height, "Height");
			viewAndEditResult.isEdited |= heightViewAndEditResult.isEdited;
			viewAndEditResult.isActive |= heightViewAndEditResult.isActive;
			if (heightViewAndEditResult.isEdited)
			{
				cylinder.SetHeight(height);
			}
		}
		{
			T radius = cylinder.GetRadius().Value();
			ViewAndEditResult radiusViewAndEditResult = ViewAndEditValue(radius, "Radius", 0.01f, 0.01f, FLT_MAX);
			viewAndEditResult.isEdited |= radiusViewAndEditResult.isEdited;
			viewAndEditResult.isActive |= radiusViewAndEditResult.isActive;
			if (radiusViewAndEditResult.isEdited)
			{
				cylinder.SetRadius(Radius<T>(radius));
			}
		}
		{
			Point3<T> center = cylinder.GetCenter();
			ViewAndEditResult centerViewAndEditResult = ViewAndEditValue(center, "Center");
			viewAndEditResult.isEdited |= centerViewAndEditResult.isEdited;
			viewAndEditResult.isActive |= centerViewAndEditResult.isActive;
			if (centerViewAndEditResult.isEdited)
			{
				cylinder.SetCenter(center);
			}

		}
		{
			UnitVector3<T> axis = cylinder.GetAxis();
            Blackboard axisBlackboard = blackboard;
            axisBlackboard.Insert<Key_VariableName>("Axis");
			ViewAndEditResult axisViewAndEditResult = ViewAndEditValue(axis, axisBlackboard, nullptr);
			viewAndEditResult.isEdited |= axisViewAndEditResult.isEdited;
			viewAndEditResult.isActive |= axisViewAndEditResult.isActive;
			if (axisViewAndEditResult.isEdited)
			{
				cylinder.SetAxis(axis);
			}
		}
		return viewAndEditResult;
	}

	ViewAndEditResult ViewAndEditValue(Transform& value, const DataTypeMemberVariable* memberData);
	ViewAndEditResult ViewAndEditValue(PointLight& pointLight, const DataTypeMemberVariable* memberData);
	ViewAndEditResult ViewAndEditValue(DirectionalLight& directionalLight, const Blackboard& blackboard);
	ViewAndEditResult ViewAndEditValue(Camera& camera, const Blackboard& blackboard);

	template<typename T>
	ViewAndEditResult ViewAndEditValue(AABB3<T>& aabb, const std::string&)
	{
		ViewAndEditResult viewAndEditResult;

		{
			Point3<T> center = aabb.GetCenter();
			ViewAndEditResult centerViewAndEditResult = ViewAndEditValue(center, "Center");
			viewAndEditResult |= centerViewAndEditResult;
			if (centerViewAndEditResult.isEdited)
			{
				aabb.SetCenter(center);
			}
		}
		{
			Vector3<T> extent = aabb.GetExtent();
			ViewAndEditResult extentViewAndEditResult = ViewAndEditValue(extent, "Extent");
			viewAndEditResult |= extentViewAndEditResult;
			if (extentViewAndEditResult.isEdited)
			{
				aabb.SetExtent(extent);
			}
		}

		return viewAndEditResult;
	}

	ViewAndEditResult ViewAndEditValue(Ray3f& ray, const Blackboard& blackboard);

	ViewAndEditResult ViewAndEditValue(EntityID& entityID, const Blackboard& blackboard, const DataTypeMemberVariable* memberData);

	ViewAndEditResult ViewAndEditValue(MeshAssetHandle& meshAsset, const Blackboard& blackboard);
	ViewAndEditResult ViewAndEditValue(ModelAssetHandle& modelAsset, const Blackboard& blackboard);
	ViewAndEditResult ViewAndEditValue(AnimatedModelAssetHandle& animatedModelAsset, const Blackboard& blackboard);
	ViewAndEditResult ViewAndEditValue(TextureAssetHandle& textureAsset, const Blackboard& blackboard);
	ViewAndEditResult ViewAndEditValue(PixelShaderAssetHandle& shaderAsset, const Blackboard& blackboard);
	ViewAndEditResult ViewAndEditValue(VertexShaderAssetHandle& shaderAsset, const Blackboard& blackboard);
	ViewAndEditResult ViewAndEditValue(AnimationAssetHandle& animationAsset, const Blackboard& blackboard);
    ViewAndEditResult ViewAndEditValue(SceneAssetHandle& sceneAsset, const Blackboard& blackboard, const DataTypeMemberVariable* memberData);

	ViewAndEditResult ViewAndEditValue(std::array<TextureAssetHandle, 3>& textureAssets, const Blackboard& blackboard);


	template<typename T> requires std::is_enum_v<T>
	ViewAndEditResult ViewAndEditValue(T& value, const std::string& variableName)
	{
		ViewAndEditResult viewAndEditResult;
		std::string_view currentName = ToString(value);
		if (ImGui::BeginCombo(variableName.c_str(), std::string(currentName).c_str()))
		{
			constexpr std::size_t enumCount = EnumCount<T>::value;
			for (size_t i = 0; i < enumCount; i++)
			{
				const T v = static_cast<T>(i);
				if (ImGui::Selectable(std::string(ToString(v)).c_str()))
				{
					value = v;
					viewAndEditResult.isEdited = true;
				}
			}
			viewAndEditResult.isActive = true;
			ImGui::EndCombo();
		}
		return viewAndEditResult;
	}

	template<typename T>
		requires requires(T& value, const DataTypeMemberVariable* memberData)
	{
		{ ViewAndEditValue(value, memberData) } -> std::same_as<ViewAndEditResult>;
	}
	ViewAndEditResult ViewAndEditValue(T& value, const Blackboard&, const DataTypeMemberVariable* memberData)
	{
		return ViewAndEditValue(value, memberData);
	}

	template<typename T>
		requires requires(T& value, const Blackboard& blackboard)
	{
		{ ViewAndEditValue(value, blackboard) } -> std::same_as<ViewAndEditResult>;
	}
	ViewAndEditResult ViewAndEditValue(T& value, const Blackboard& blackboard, const DataTypeMemberVariable*)
	{
		return ViewAndEditValue(value, blackboard);
	}

	template<typename T>
	ViewAndEditResult ViewAndEditValueVectorImpl(std::vector<T>& vector, const Blackboard& blackboard, const DataTypeMemberVariable* memberData = nullptr)
	{
		static size_t currentPopupIndex = 0;
		static constexpr const char* VectorElementPopupStrID = "Vector Element Popup";
		ViewAndEditResult result;

        const std::string variableName = memberData ? memberData->customName : "Vector";
        ImGui::Text(variableName.c_str());
		ImGui::SameLine();
		ImGui::BeginDisabled(vector.empty());
		if (ImGui::Button("Clear"))
		{
			vector.clear();
			result.isEdited = true;
            result.isActive = true;
			result.vectorEditOperation = VectorEditOperations::Clear{};
		}
        ImGui::EndDisabled();

		for (size_t i = 0; i < vector.size(); ++i)
		{
			bool vectorElementBtnWasPressed = false;
			if constexpr (std::same_as<T, bool>)
			{
				bool b = vector[i];
				ViewAndEditResult resultIteration = ViewAndEditDataPtr(GetDataTypeID<T>(), &b, blackboard);
				vector[i] = b;
				result.isEdited |= resultIteration.isEdited;
				result.isActive |= resultIteration.isActive;
                result.vectorEditOperation = VectorEditOperations::ModifyElement{ i };

				ImGui::SameLine();

				ImGui::PushID(&b);
				vectorElementBtnWasPressed = ImGui::Button("...");
				ImGui::PopID();

			}
			else
			{
				T& data = vector[i];
				ViewAndEditResult resultIteration = ViewAndEditDataPtr(GetDataTypeID<T>(), &data, blackboard);
				result.isEdited |= resultIteration.isEdited;
				result.isActive |= resultIteration.isActive;
				result.vectorEditOperation = VectorEditOperations::ModifyElement{ i };
				ImGui::SameLine();

				ImGui::PushID(&data);
				vectorElementBtnWasPressed = ImGui::Button("...");
				ImGui::PopID();
			}

			if (vectorElementBtnWasPressed)
			{
				ImGui::OpenPopup(VectorElementPopupStrID);
				currentPopupIndex = i;
			}
		}

		if (ImGui::BeginPopup(VectorElementPopupStrID))
		{
			if (ImGui::MenuItem("Delete"))
			{
				vector.erase(begin(vector) + currentPopupIndex);
				result.isEdited = true;
                result.vectorEditOperation = VectorEditOperations::EraseElement{ currentPopupIndex };
			}

			if (ImGui::MenuItem("Insert Before"))
			{
				vector.insert(begin(vector) + currentPopupIndex, T{});
				result.isEdited = true;
                result.vectorEditOperation = VectorEditOperations::Insert{ currentPopupIndex };
			}

			if (ImGui::MenuItem("Reset Value"))
			{
				vector[currentPopupIndex] = T{};
				result.isEdited = true;
				result.vectorEditOperation = VectorEditOperations::ResetElement{ currentPopupIndex };
			}

			result.isActive = true;

			ImGui::EndPopup();
		}

		if (ImGui::Button("Add"))
		{
			vector.emplace_back();

			result.isEdited = true;
			result.isActive = true;
			result.vectorEditOperation = VectorEditOperations::PushBack{};
		}

		return result;
	}
}

namespace std
{
	template<typename T>
	CLX::ViewAndEditResult ViewAndEditValue(std::vector<T>& vector, const CLX::Blackboard& blackboard, const CLX::DataTypeMemberVariable* memberData = nullptr)
	{
        return CLX::ViewAndEditValueVectorImpl(vector, blackboard, memberData);
	}
}