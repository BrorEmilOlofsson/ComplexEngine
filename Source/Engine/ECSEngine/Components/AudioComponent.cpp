#include "Engine/Precompiled/EnginePch.hpp"
#include "AudioComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{


    void RegisterAudioComponent() {}

    REGISTER_COMPONENT(AudioComponent);
    REGISTER_MEMBER(AudioComponent::audioAsset);
    REGISTER_MEMBER(AudioComponent::volume);

}