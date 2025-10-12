#pragma once
#include "Editor/Core/PopUp.hpp"
#include <functional>
#include <memory>

namespace Simple
{

	class EditorCallbacks
	{
	public:
		static std::function<void()> SetPopUpActive(std::shared_ptr<PopUp> aPopUp, bool* aBoolean)
		{
			return [=]() -> void
				{
					aPopUp->IsActive() = *aBoolean;
				};
		}
	};
}