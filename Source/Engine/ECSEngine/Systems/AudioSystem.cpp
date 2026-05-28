#include "Engine/Precompiled/EnginePch.hpp"
#include "AudioSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/AudioComponent.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Math/RotationMath.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"
#include "Engine/Audio/AudioManager.hpp"
#include "Engine/ECSEngine/Components/CameraComponent.hpp"

namespace CLX
{

    void UpdateAudio(ECS& ecs, AudioManager& audioManager)
    {
        Camera camera;

        ecs.ForEach([&camera](CameraComponent& cameraComponent)
            {
                camera = cameraComponent.camera;
            }
        );

        ecs.ForEach([&ecs, &camera, &audioManager](EntityID entityID, AudioComponent& audioComponent)
            {
                audioComponent;
                Transform entityTransform = GetEntityWorldTransform(ecs, entityID);

                Transform cameraSpaceTransform = ToLocalSpace(entityTransform, camera.GetTransform());

                if (audioComponent.channelId == InvalidID<AudioChannelID>())
                {
                    audioComponent.channelId = audioManager.Play(audioComponent.audioAsset, audioComponent.volume, cameraSpaceTransform.GetPosition());
                }

                audioManager.SetChannelPosition(audioComponent.channelId, cameraSpaceTransform.GetPosition());
            }
        );
    }

    void AudioSystem::Update(ECS& ecs, const float, const Blackboard& blackboard)
    {
        AudioManager& audioManager = blackboard.Get<Key_AudioManager>();
        UpdateAudio(ecs, audioManager);
    }

}