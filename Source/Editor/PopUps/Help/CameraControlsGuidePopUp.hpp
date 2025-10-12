#pragma once
#include "Editor/Core/PopUp.hpp"

namespace Simple
{

	class CameraControlsGuidePopUp final : public PopUp
	{
	public:
		CameraControlsGuidePopUp(const std::string& aName);
		void Render(const Blackboard& blackboard) override;
	};
}