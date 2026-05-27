#pragma once
#include <fmod/core/fmod.hpp>
#include <fmod/studio/fmod_studio.hpp>

namespace CLX
{

    class AudioManager
    {
    public:

        AudioManager();

        void Play();

        void Update();

    private:

        FMOD::Studio::System* mFMODStudioSystem = nullptr;
        FMOD::System* mFMODSystem = nullptr;

    };
}