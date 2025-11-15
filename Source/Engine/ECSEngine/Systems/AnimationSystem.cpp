#include "Engine/Precompiled/EnginePch.hpp"
#include "AnimationSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/AnimatedModelComponent.hpp"
#include "Graphics/Model/AnimatedModel.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Graphics/Animation/Animation.hpp"
#include "Graphics/Animation/AnimationUtility.hpp"
#include "Graphics/RenderState.hpp"
#include <External/imgui/imgui.h>

namespace Simple
{

	AnimationAsset CreateAnimationAsset()
	{
		Animation animation;
		animation.duration = 5.f;
		animation.framesPerSecond = 1.f;
		animation.name = "TestAnimation";
		animation.boneKeyFrames.resize(3);
		BoneKeyFrames& child = animation.boneKeyFrames[1];
		//child.positions = { Point3f(0.0f, 5.0f, 0.0f),   // t = 0.0
		//	Point3f(5.0f, 5.0f, 0.0f),   // t = 0.5
		//	Point3f(10.0f, 5.0f, 0.0f) };
		//child.positionTimestamps = { 0.0f, 2.5f, 5.0f };
		child.rotationTimestamps = { 0.0f, 2.5f, 5.0f };

		const Quaternionf q = ToQuaternion(Matrix4x4f{});


		constexpr RotationMatrix3f startMatrix = RotationMatrix3f::FromXY(UnitVector3f::Right(), UnitVector3f::Up());
		constexpr RotationMatrix3f middleMatrix = RotationMatrix3f::FromXY(UnitVector3f::Backward(), UnitVector3f::Up());
		constexpr RotationMatrix3f endMatrix = RotationMatrix3f::FromXY(UnitVector3f::Left(), UnitVector3f::Up());

		child.rotations = {
			ToQuaternion(startMatrix),
			ToQuaternion(middleMatrix),
			ToQuaternion(endMatrix)
		};

		child.scaleTimestamps = { 0.0f, 2.5f, 5.0f };
		child.scales = {
			Vector3f(1.0f, 1.0f, 1.0f),   // t = 0.0
			Vector3f(1.0f, 1.0f, 1.0f),   // t = 0.5
			Vector3f(1.0f, 1.0f, 1.0f)    // t = 1.0
		};

		return AnimationAsset(std::make_shared<Animation>(animation));
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

			Matrix4x4f parentGlobal =
				Matrix4x4f::CreateTranslationMatrix({ 0,5,0 }); // Child bind global

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

	void AnimationTest(const float deltaTime)
	{
		static unsigned int i = 0;
		i++;
		if (i % 2 == 0)
		{
			return;
		}

		animTest.mAnimationPlayer.Update(deltaTime, animTest.mBones);
	}

	void AnimationSystem::Update(ECS& ecs, const float deltaTime, const Blackboard& blackboard)
	{
		AnimationTest(deltaTime);
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
		ImGui::Begin("Animation System Debug");

		ImGui::SliderFloat("Animation Time", &animTest.mAnimationPlayer.mCurrentTime, 0.0f, animTest.mAnimationAsset->duration);
		ImGui::End();
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
			renderList.AddSphere(DrawSphere(Spheref(globalBoneMatrices[i].GetTranslation(), 0.1f), c), false);
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