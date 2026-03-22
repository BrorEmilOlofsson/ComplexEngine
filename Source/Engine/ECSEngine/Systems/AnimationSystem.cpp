#include "Engine/Precompiled/EnginePch.hpp"
#include "AnimationSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/AnimatedModelComponent.hpp"
#include "Engine/Graphics/Model/AnimatedModel.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include "Engine/Graphics/Animation/Animation.hpp"
#include "Engine/Graphics/Animation/AnimationUtility.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include <External/imgui/imgui.h>

namespace CLX
{

	AnimationAsset CreateAnimationAsset()
	{
		Animation animation;
		animation.duration = 10.f;
		animation.framesPerSecond = 1.f;
		animation.name = "TestAnimation";
		animation.boneKeyFrames.resize(3);

		{
			// Parent bone

			BoneKeyFrames& parent = animation.boneKeyFrames[0];

			parent.rotationTimestamps = { 0.0f, 2.5f, 5.0f };

			constexpr RotationMatrix3f startMatrix = RotationMatrix3f::Identity();
			constexpr RotationMatrix3f middleMatrix = RotationMatrix3f::Identity();
			constexpr RotationMatrix3f endMatrix = RotationMatrix3f::Identity();


			parent.rotations = {
				ToQuaternion(startMatrix),
				ToQuaternion(middleMatrix),
				ToQuaternion(endMatrix)
			};
		}

		{
			// Child bone

			BoneKeyFrames& child = animation.boneKeyFrames[1];
			child.rotationTimestamps = { 0.0f, 2.5f, 5.0f, 7.5f, 10.f };

			const Quaternionf q = ToQuaternion(Matrix4x4f{});


			constexpr RotationMatrix3f m0 = RotationMatrix3f::Identity();
			constexpr RotationMatrix3f m1 = RotationMatrix3f::FromXY(UnitVector3f::Backward(), UnitVector3f::Up());
			constexpr RotationMatrix3f m2 = RotationMatrix3f::FromXY(UnitVector3f::Left(), UnitVector3f::Up());
			constexpr RotationMatrix3f m3 = RotationMatrix3f::FromXY(UnitVector3f::Forward(), UnitVector3f::Up());
			constexpr RotationMatrix3f m4 = RotationMatrix3f::Identity();

			child.rotations = {
				ToQuaternion(m0),
				ToQuaternion(m1),
				ToQuaternion(m2),
				ToQuaternion(m3),
				ToQuaternion(m4)
			};

		}

		{
			// Grand child bone

			// No animation for this bone


		}
		return AnimationAsset(std::move(animation), std::filesystem::path());
	}

	std::array<Bone, GlobalMaxBones> CreateBonesTest()
	{
		std::array<Bone, GlobalMaxBones> bones{};

		// Root bone
		{
			Bone& b = bones[0];
			b.name = "Root";
			b.parentIndex = std::numeric_limits<uint32_t>::max();
			Matrix4x4f globalBind = Matrix4x4f::CreateTranslationMatrix({ 0,0,0 });
			b.inverseBindPose = Matrix4x4f::GetFastInverse(globalBind);
			b.localBindPose = globalBind; // root local = root global
		}

		// Child bone
		{
			Bone& b = bones[1];
			b.name = "Child";
			b.parentIndex = 0;
			Matrix4x4f globalBind = Matrix4x4f::CreateTranslationMatrix({ 0,5,0 });
			b.inverseBindPose = Matrix4x4f::GetFastInverse(globalBind);

			// local = inv(parentGlobal) * global
			Matrix4x4f parentGlobal = Matrix4x4f::CreateTranslationMatrix({ 0,0,0 });
			b.localBindPose = Matrix4x4f::GetFastInverse(parentGlobal) * globalBind;
		}

		// Grandchild bone
		{
			Bone& b = bones[2];
			b.name = "GrandChild";
			b.parentIndex = 1;
			Matrix4x4f globalBind = Matrix4x4f::CreateTranslationMatrix({ 5,5,0 });
			b.inverseBindPose = Matrix4x4f::GetFastInverse(globalBind);

			Matrix4x4f parentGlobal = Matrix4x4f::CreateTranslationMatrix({ 0,5,0 }); // Child bind global

			b.localBindPose = Matrix4x4f::GetFastInverse(parentGlobal) * globalBind;
		}
		return bones;
	}

	struct AnimationTestStruct
	{
		AnimationTestStruct()
		{
			mAnimationAsset = CreateAnimationAsset();
			mAnimationPlayer.mAnimationAsset = AnimationAssetHandle(mAnimationAsset);
			mBones = CreateBonesTest();
		}
		AnimationPlayer mAnimationPlayer;
		AnimationAsset mAnimationAsset;
		std::array<Bone, GlobalMaxBones> mBones{};
	};


	static AnimationTestStruct animTest;

	static bool isGoodFrame = false;
	static bool pause = false;

	void AnimationTest(const float deltaTime)
	{
		isGoodFrame = !isGoodFrame;
		if (isGoodFrame)
		{
			return;
		}


		if (pause)
		{
			animTest.mAnimationPlayer.UpdateAnimation(animTest.mBones);
			return;
		}
		else
		{
			animTest.mAnimationPlayer.Update(deltaTime, animTest.mBones);
		}
	}

	void AnimationSystem::Update(ECS& ecs, const float deltaTime, const Blackboard& blackboard)
	{
		//AnimationTest(deltaTime);
		AssetManager& assetManager = blackboard.Get<Key_AssetManager>();
		ecs.ForEach([deltaTime, &assetManager](AnimatedModelComponent& animationComponent)
			{
				if (!animationComponent.modelHandle)
				{
					return;
				}

				if (!animationComponent.animationPlayer.mAnimationAsset.IsValid())
				{
					animationComponent.animationPlayer.mAnimationAsset = assetManager.GetAnimation(std::filesystem::path(SIMPLE_DIR_ASSETS) / "StellaQuest" / "Player" / "Stella_Idle.fbx");
				}

				animationComponent.animationPlayer.Update(deltaTime, animationComponent.modelHandle->GetSkeleton().GetBones());
			});
	}

	void AnimationSystem::EditorUpdate(ECS& ecs, const Blackboard& blackboard)
	{
		const float deltaTime = blackboard.Get<Key_DeltaTime>();
		Update(ecs, deltaTime, blackboard);
	}

	void AnimationSystem::Render(const ECS&, const Blackboard& blackboard)
	{
		if (isGoodFrame)
		{
			return;
		}
		/*ImGui::Begin("Animation System Debug");

		ImGui::SliderFloat("Animation Time", &animTest.mAnimationPlayer.mCurrentTime, 0.0f, animTest.mAnimationAsset->duration);
		ImGui::Checkbox("Pause Animation", &pause);
		ImGui::End();*/
		RenderList& renderList = blackboard.Get<Key_CurrentRenderState>().GetRenderList();

		auto globalBoneMatrices = ComputeGlobalTransforms(
			animTest.mAnimationPlayer.localBoneMatrices,
			animTest.mBones
		);


		std::array<Matrix4x4f, GlobalMaxBones> finalBoneMatrices = CalculateFinalBoneMatrices(
			globalBoneMatrices,
			animTest.mBones
		);

		finalBoneMatrices;

		for (size_t i = 0; i < 3; ++i)
		{
			Color c = Colors::Red;
			if (i == 0)
			{
				c = Colors::White;
			}
			renderList.AddSphere(DrawSphere(Spheref::FromCenterAndRadius(globalBoneMatrices[i].GetTranslation(), Radiusf(0.1f)), c), false);
		}

		for (size_t i = 0; i < 3; ++i)
		{
			if (animTest.mBones[i].parentIndex == std::numeric_limits<uint32_t>::max())
			{
				continue;
			}
			const Point3f start = globalBoneMatrices[i].GetTranslation();
			const Point3f end = globalBoneMatrices[animTest.mBones[i].parentIndex].GetTranslation();
			renderList.AddLine(DrawLine(start, end, Colors::LightBlue));
		}
	}
}