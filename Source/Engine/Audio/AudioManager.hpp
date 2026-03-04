#pragma once
#include <string>
#include <vector>

namespace CLX
{
	class AudioManager
	{
		struct Channel
		{
			Channel(std::string fileName, size_t channelIndex) : mChannelName(fileName), mChannelIndex(channelIndex) {}
			std::string mChannelName;
			size_t mChannelIndex = 0;
		};

	public:

		AudioManager() = default;
		~AudioManager();

		void Init();
		void Update();

		void ToggleMuteMusic();
		void ToggleMuteSFX();

		void PlaySFX(std::string fileName);
		void PlayMusic(std::string filename);

		void StopSFX(std::string fileName);
		void StopMusic(std::string fileName);
		void StopAllMusic();

	public:

		//Range: 0 - 1f.
		void ChangeSFXVolume(float volume);

		//Range: 0 - 1f.
		void ChangeMusicVolume(float volume);

		void SetMainMusic(const std::string& musicFileName);
	public:

		float GetMusicVolume() const;
		float GetSFXVolume() const;
		const std::string& GetMainMusicName() const;

	private:

		std::string mFilePathRoot;
		std::string mMainMusicName = "Ocarina of Time.mp3";

		float mSFXVolume = 1.f;
		float mMusicVolume = 1.f;
		float mSFXVolMute = 0.f;
		float mMusicVolMute = 0.f;

		bool mMusicMuted = false;
		bool mSFXMuted = false;

		std::vector<Channel> mMusicChannels;
		std::vector<Channel> mSFXChannels;
	};
}