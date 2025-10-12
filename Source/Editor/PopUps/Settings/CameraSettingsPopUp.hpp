#pragma once
#include "Editor/Core/PopUp.hpp"

namespace Simple
{
	class CameraSettingsPopUp final : public PopUp
	{
	public:

		CameraSettingsPopUp(const std::string& name);

		void Render(const Blackboard& blackboard) override;

	};
}
