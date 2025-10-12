#pragma once
#include "Graphics/Model/Skeleton.hpp"
#include "Graphics/Animation/Animation.hpp"

namespace Simple
{
	struct AnimationComponent;
}

namespace Simple
{
	enum class eAnimationState
	{
		NoAnimation,
		Playing,
		Paused,
		Finished
	};

	class AnimationPlayer
	{
	public:
		ModelSpacePose mModelSpacePose;
	public:

		AnimationPlayer() = default;
		//Will lerp active animation and setpose directly
		//void UpdateTest(AnimationComponent* animationPlayerComponent, const float deltaTime);

		//void LerpAnimationTest(const size_t currentFrame, const size_t nextFrame, const float delta);

		void Init(const Animation* animation, const Skeleton* skeleton);

		void Play(const bool shouldLoop = true); //NOTE(v9.34.0): Default to true because my smol brain keep forgetting and wondering why animation doesn't play everytime i rewrote system to test
		void Pause();
		void Stop();
		void Restart();

		void SetIsLooping(const bool shouldLoop);
		void SetCurrentFrame(const unsigned int currentFrame);

		const ModelSpacePose GetLocalSpacePose() const;
		eAnimationState GetAnimationState() const;
		float GetTime() const;
		unsigned int GetCurrentFrame() const;

	private:

		void CalculateFrame(size_t& currentFrame, size_t& nextFrame, float& delta);
		void UpdateTimer(const float deltaTime);
	private:

		const Animation* mAnimation = nullptr;
		const Skeleton* mSkeleton = nullptr;

		eAnimationState mState = eAnimationState::NoAnimation;

		float mTime = 0.0f;
		bool mIsLooping = false;
	};
}