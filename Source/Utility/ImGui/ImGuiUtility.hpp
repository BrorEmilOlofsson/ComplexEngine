#pragma once
#include <External/imgui/imgui.h>
#include "Utility/Math/Point2.hpp"
#include "Utility/Color.hpp"
#include <optional>
#include <string>

namespace Simple
{

	template<typename T = float>
	[[nodiscard]] constexpr Point2<T> ToPoint2(const ImVec2& vec2)
	{
		return Point2<T>(static_cast<T>(vec2.x), static_cast<T>(vec2.y));
	}

	template<typename T = float>
	[[nodiscard]] constexpr Vector2<T> ToVector2(const ImVec2& vec2)
	{
		return Vector2<T>(static_cast<T>(vec2.x), static_cast<T>(vec2.y));
	}

	template<typename T>
	[[nodiscard]] ImVec2 ToImVec2(const Point2<T>& point)
	{
		return ImVec2{ static_cast<float>(point.x), static_cast<float>(point.y) };
	}

	template<typename T>
	[[nodiscard]] ImVec2 ToImVec2(const Vector2<T>& vector)
	{
		return ImVec2{ static_cast<float>(vector.x), static_cast<float>(vector.y) };
	}

	[[nodiscard]] constexpr ImVec4 ToImVec4(const Color& color)
	{
		return ImVec4{ color.r, color.g, color.b, color.a };
	}

	template<typename T>
	[[nodiscard]] constexpr std::string GetStringID()
	{
		return (std::string("DD_") + typeid(T).name()).substr(0, 32);
	}

	template<typename T>
	[[nodiscard]] std::optional<T> ObjectTarget()
	{
		std::optional<T> obj;
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetStringID<T>().c_str());

			if (payload)
			{
				obj = *(T*)payload->Data;
			}

			ImGui::EndDragDropTarget();
		}

		return obj;
	}

	template<typename T>
	void ObjectSource(const T& value, const std::string& name = "Object")
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(GetStringID<T>().c_str(), &value, sizeof(T));
			ImGui::Button(name.c_str(), ImVec2(100, 20));
			ImGui::EndDragDropSource();
		}
	}


}

namespace Simple
{


	namespace ImGuiUtility
	{
		inline void SameLineDummy(float widthOffset, float heightOffset)
		{
			::ImGui::SameLine();
			::ImGui::Dummy(ImVec2(widthOffset, heightOffset));
			::ImGui::SameLine();
		}

		inline void SeparatorDummy(float widthOffset, float heightOffset)
		{
			::ImGui::Dummy(ImVec2(widthOffset, heightOffset));
			::ImGui::Separator();
			::ImGui::Dummy(ImVec2(widthOffset, heightOffset));
		}

		inline void SeparatorTextDummy(const char* text = "", float widthOffset = 0.0f, float heightOffset = 0.0f)
		{
			::ImGui::Dummy(ImVec2(widthOffset, heightOffset));
			::ImGui::PushStyleVar(ImGuiStyleVar_SeparatorTextAlign, ImVec2(0.50f, 0.5f));
			::ImGui::SeparatorText(text);
			::ImGui::PopStyleVar(1);
			::ImGui::Dummy(ImVec2(widthOffset, heightOffset));
		}

		inline void DrawRect(const Point2i& screenPos, const Color color)
		{
			::ImGui::GetForegroundDrawList()->AddRectFilled(ToImVec2(screenPos), ToImVec2(screenPos + Vector2i{ 2, 2 }), ::ImGui::ColorConvertFloat4ToU32(ToImVec4(color)));
		}
	}
}