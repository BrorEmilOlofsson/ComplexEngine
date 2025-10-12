#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/PopUps/Settings/AudioSettingsPopUp.hpp"
#include "Engine/Audio/AudioManager.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"

namespace Simple
{

	AudioSettingsPopUp::AudioSettingsPopUp(const std::string& name)
		: PopUp(name)
		, mMusicIsActive(false)
	{
	}

	void AudioSettingsPopUp::Init()
	{
		/*if (mMusicIsActive)
		{
			GetAudioManager().PlayMusic(GetAudioManager().GetMainMusicName());
		}*/
	}

	void AudioSettingsPopUp::Render(const Blackboard& blackboard)
	{
		AudioManager& audioManager = blackboard.Get<Key_AudioManager>();
		if (ImGui::Begin(mImGuiName.c_str(), &mIsActive))
		{

			const std::string musicName = audioManager.GetMainMusicName();

			if (ImGui::Checkbox(std::string("Play Music").append(mImGuiTag).c_str(), &mMusicIsActive))
			{
				if (mMusicIsActive)
				{
					audioManager.PlayMusic(musicName);
				}
				else
				{
					audioManager.StopAllMusic();
				}
			}

			char buffer[256];
			memset(buffer, '\0', sizeof(buffer));
			strncpy_s(buffer, musicName.c_str(), sizeof(buffer));
			buffer[sizeof(buffer) - 1] = '\0';

			ImGui::AlignTextToFramePadding();

			ImGui::Text("Music:");
			ImGui::SameLine();
			ImGui::PushItemWidth(200);

			ImGui::BeginDisabled();
			if (ImGui::InputTextWithHint(mImGuiTag.c_str(), "Drag Drop Music File", buffer, sizeof(buffer)))
			{
			}
			ImGui::EndDisabled();

			if (const ImGuiPayload* currentPayload = ImGui::GetDragDropPayload())
			{
				const std::filesystem::path payloadData = std::filesystem::path(reinterpret_cast<const char*>(currentPayload->Data));
				const std::filesystem::path extension = payloadData.extension();

				if (extension.string() == ".mp3")
				{
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Assets_Browser"))
						{
							const std::string newMusicName = payloadData.filename().string();
							audioManager.SetMainMusic(newMusicName);
						}
						ImGui::EndDragDropTarget();
					}
				}
			}

			float musicVolume = audioManager.GetMusicVolume();
			if (ImGui::DragFloat("Music Volume##SettingWindow", &musicVolume, 0.01f, 0.0f, 1.0f))
			{
				audioManager.ChangeMusicVolume(musicVolume);
			}
		}

		ImGui::End();
	}
}
