#pragma once
#include "Editor/Core/Base/MenuItemBase.hpp"
#include <functional>
#include <vector>

namespace Simple
{

	class MenuItemPopUp final : public MenuItemBase
	{
	public:
		MenuItemPopUp(const std::string& name);

		void Render() override final;
		void Invoke();

	public:

		void AddCallback(std::function<void()>&& callback);
		void AddCallback(const std::function<void()>& callback);
		void SetIsActive(const bool shouldActive);

	public:

		bool& GetIsActiveRef();

	private:

		std::vector<std::function<void()>> mCallback;
		bool mIsActive = false;
	};
}