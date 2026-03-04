#pragma once
#include "Editor/Core/Base/MenuItemBase.hpp"
#include <memory>
#include <vector>

namespace CLX
{

	class MenuItemMenu final : public MenuItemBase
	{
	public:
		MenuItemMenu(const std::string& name);

		void Render() override final;

		template<DerivedFromMenuItem T>
		T* AddChild(const std::string& name);

	private:

		std::vector<std::unique_ptr<MenuItemBase>> mItems;
	};

	template<DerivedFromMenuItem T>
	inline T* MenuItemMenu::AddChild(const std::string& name)
	{
		std::unique_ptr<T> child = std::make_unique<T>(name);

		T* ptr = child.get();
		ptr->SetImGuiTag(std::string("##" + name).c_str());

		mItems.push_back(std::move(child));
		return ptr;
	}
}
