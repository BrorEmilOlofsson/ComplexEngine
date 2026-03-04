#include "Engine/Precompiled/EnginePch.hpp"
#include "AudioManager.hpp"
#include "Engine/Utility/Algorithm.hpp"
#include <External/TheGameAssembly/SoundEngine/SoundEngine.h>
#include <External/TheGameAssembly/SoundEngine/SoundEngineImpl.hpp>

namespace Simple
{

	AudioManager::~AudioManager()
	{
		SoundEngine::Release();
	}

	void AudioManager::Init()
	{
		ChangeMusicVolume(0.1f); //NOTE(v9.35.1): "StardewValley.mp3" test were too loud 

		//Dont touch
		SoundEngine::Init(std::filesystem::absolute(SIMPLE_DIR_AUDIO).string());

		//Music
		SoundEngine::LoadSoundFile("StardewValley.mp3", false, false, true); 
		SoundEngine::LoadSoundFile("Ocarina of Time.mp3", false, false, true);

		//SFX
		//SoundEngine::LoadSoundFile("");
	}

	void AudioManager::PlayMusic(std::string fileName)
	{
		mMusicChannels.push_back(Channel(fileName, SoundEngine::PlaySound(fileName, mMusicVolume)));
	}

	void AudioManager::ToggleMuteMusic()
	{
		if (!mMusicMuted)
		{
			ChangeMusicVolume(mMusicVolMute);
			mMusicMuted = true;
		}
		else
		{
			ChangeMusicVolume(mMusicVolume);
			mMusicMuted = false;
		}
	}

	void AudioManager::ToggleMuteSFX()
	{
		if (!mSFXMuted)
		{
			ChangeSFXVolume(mSFXVolMute);
			mSFXMuted = true;
		}
		else
		{
			ChangeSFXVolume(mSFXVolume);
			mSFXMuted = false;
		}
	}

	void AudioManager::PlaySFX(std::string fileName)
	{
		mSFXChannels.push_back(Channel(fileName, SoundEngine::PlaySound(fileName, mSFXVolume)));
	}

	void AudioManager::StopSFX(const std::string fileName)
	{
		for (size_t i = 0; i < mSFXChannels.size(); i++)
		{
			if (mSFXChannels[i].mChannelName == fileName)
			{
				SoundEngine::StopSound(mSFXChannels[i].mChannelIndex);
			}
		}
	}

	void AudioManager::StopMusic(const std::string fileName)
	{
		for (size_t i = 0; i < mMusicChannels.size(); i++)
		{
			if (mMusicChannels[i].mChannelName == fileName)
			{
				SoundEngine::StopSound(mMusicChannels[i].mChannelIndex);
				mMusicChannels.erase(mMusicChannels.begin() + i);
			}
		}
	}

	void AudioManager::StopAllMusic()
	{
		for (size_t i = 0; i < mMusicChannels.size(); i++)
		{
			SoundEngine::StopSound(mMusicChannels[i].mChannelIndex);
		}

		for (size_t i = 0; i < mMusicChannels.size(); i++)
		{
			mMusicChannels.erase(mMusicChannels.begin() + i);
		}
	}

	float AudioManager::GetSFXVolume() const
	{
		return mSFXVolume;
	}

	const std::string& AudioManager::GetMainMusicName() const
	{
		return mMainMusicName;
	}

	void AudioManager::ChangeSFXVolume(const float volume)
	{
		mSFXVolume = volume;

		for (size_t i = 0; i < mSFXChannels.size(); i++)
		{
			SoundEngine::SetVolume(static_cast<int>(mSFXChannels[i].mChannelIndex), mSFXVolume);
		}
	}

	float AudioManager::GetMusicVolume() const
	{
		return mMusicVolume;
	}

	void AudioManager::ChangeMusicVolume(const float volume)
	{
		mMusicVolume = volume;

		for (size_t i = 0; i < mMusicChannels.size(); i++)
		{
			SoundEngine::SetVolume(static_cast<int>(mMusicChannels[i].mChannelIndex), mMusicVolume);
		}
	}

	void AudioManager::SetMainMusic(const std::string& musicFileName)
	{
		StopMusic(mMainMusicName);
		mMainMusicName = musicFileName;
		PlayMusic(mMainMusicName);
	}

	void AudioManager::Update()
	{
		SoundEngine::Update();

		const auto soundEngine = SoundEngine::GetImpl();

		for (size_t i = 0; i < mMusicChannels.size(); ++i)
		{
			if (soundEngine->IsPlaying(mMusicChannels[i].mChannelIndex) == false)
			{
				mMusicChannels.erase(mMusicChannels.begin() + i);
			}
		}
	}
}