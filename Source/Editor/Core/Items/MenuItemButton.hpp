#pragma once
#include "Editor/Core/Base/MenuItemBase.hpp"
#include <functional>

namespace Simple
{
	class MenuItemButton final : public MenuItemBase
	{
	public:

		MenuItemButton(const std::string& name, std::function<void()> callback = nullptr);

		void Render() override final;

	public:

		void SetCallback(std::function<void()> callback);

	private:

		std::function<void()> mCallback;
	};
}
