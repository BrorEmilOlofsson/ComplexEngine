#pragma once
#include "Editor/Core/Base/MenuItemBase.hpp"
#include <functional>
#include <string>
#include <vector>

namespace Simple
{

	class MenuItemSelectable final : public MenuItemBase
	{
	public:

		MenuItemSelectable(const std::string& name, std::function<void(const std::string&)> callback = nullptr);

		void Render() override final;

	public:

		void SetStrings(const std::vector<std::string>& newStrings);
		void SetCallback(std::function<void(const std::string&)> callback);

	private:

		std::function<void(const std::string&)> mCallback;
		std::vector<std::string> mStrings;
	};
}
