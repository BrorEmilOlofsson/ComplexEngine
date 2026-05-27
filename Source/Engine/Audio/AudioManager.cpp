#include <Engine/Precompiled/EnginePch.hpp>
#include "AudioManager.hpp"
#include <External/fmod/studio/fmod_studio.hpp>

namespace CLX
{

    AudioManager::AudioManager()
    {
        FMOD::Studio::System::create(&mFMODStudioSystem);
        auto result = mFMODStudioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
        ASSERT(result == FMOD_OK);
    }

    void AudioManager::Play()
    {

    }

    void AudioManager::Update()
    {
        auto result = mFMODStudioSystem->update();
        if (result != FMOD_OK)
        {
            ASSERT(false);
        }
    }
}