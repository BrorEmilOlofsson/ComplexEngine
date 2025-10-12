#pragma once
#include "Editor/Core/PopUp.hpp"

namespace Simple
{
	class AudioSettingsPopUp final : public PopUp
	{
	public:

		AudioSettingsPopUp(const std::string& name);

		void Init() override;
		void Render(const Blackboard& blackboard) override;

	private:

		bool mMusicIsActive = false;
	};
}
