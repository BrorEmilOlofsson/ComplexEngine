#include "Engine/Precompiled/EnginePch.hpp"
#include "RenderSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/AnimationComponent.hpp"
#include "Engine/ECSEngine/Components/Sprite3DComponent.hpp"
#include "Engine/ECSEngine/Components/Sprite2DComponent.hpp"
#include "Engine/ECSEngine/Components/DirectionalLightComponent.hpp"
#include "Engine/ECSEngine/Components/MeshComponent.hpp"
#include "Engine/ECSEngine/Components/ModelComponent.hpp"
#include "Engine/ECSEngine/Components/SkyBoxComponent.hpp"
#include "Engine/ECSEngine/Components/ArrowComponent.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformUtility.hpp"
#include "Graphics/RenderList.hpp"
#include "Graphics/GraphicsSettings.hpp"
#include "Graphics/SkyBox/SkyBox.hpp"
#include "Graphics/RenderState.hpp"
#include "Utility/Asset/MeshAsset.hpp"
#include "Graphics/Mesh/Mesh.hpp"
#include "Graphics/Model/Model.hpp"

namespace Simple
{
	static void PushStaticAnimationLines(const Transform& transform, const AnimationComponent& animationPlayerComponent, RenderList& renderList)
	{
		const std::vector<Joint>& joints = animationPlayerComponent.skeleton->mJoints;
		const Point3f position = transform.GetPosition();
		const Vector3f scale = transform.GetScale();

		std::vector<DrawLine> staticSkeletonLines(joints.size());

		DrawLine line;
		line.color = Colors::Black;

		DrawSphere drawSphere;
		drawSphere.sphere.SetRadius(0.05f);
		drawSphere.color = Colors::Black;

		for (size_t index = 0; index < joints.size(); ++index)
		{
			Joint joint = joints[index];

			const Matrix4x4 boneWorldTransform = Matrix4x4f::GetInverse(joint.mBindPoseInverse);

			drawSphere.sphere.SetCenter(boneWorldTransform.GetTranslation() + Vector3f(position.x, position.y, position.z));

			renderList.AddSphere(drawSphere);

			if (joint.mParent == -1)
			{
				continue;
			}

			const Matrix4x4 boneWorldTransformNext = Matrix4x4f::GetInverse(joints[joint.mParent].mBindPoseInverse);

			const Point3f boneWorldPosition = boneWorldTransform.GetTranslation();
			const Point3f boneWorldPositionNext = boneWorldTransformNext.GetTranslation();
			line.startPosition = position + Vector3f(boneWorldPosition.x, boneWorldPosition.y, boneWorldPosition.z) * scale;
			line.endPosition = position + Vector3f(boneWorldPositionNext.x, boneWorldPositionNext.y, boneWorldPositionNext.z) * scale;

			staticSkeletonLines[index] = line;
		}

		renderList.AddLines(staticSkeletonLines);
	}

	static void PushAnimatedSkeletonLines(const Transform& transform, const AnimationComponent& animationPlayerComponent, RenderList& renderList)
	{
		const SkeletonAssetHandle skeleton = animationPlayerComponent.skeleton;
		const ModelSpacePose& modelSpacePose = animationPlayerComponent.animationPlayer.mModelSpacePose;
		const Matrix4x4f modelTransform = transform.GetMatrix();
		const Point3f position = transform.GetPosition();
		const Vector3f scale = transform.GetScale();
		const size_t jointSize = animationPlayerComponent.skeleton->mJoints.size();

		std::vector<DrawLine> animatedkeletonLines(jointSize);
		LocalSpacePose pose;

		skeleton->ConvertModelSpacePoseToLocalSpacePose(modelSpacePose, pose);

		DrawSphere sphere;
		sphere.sphere.SetRadius(0.05f);
		sphere.color = { 0.0f, 1.0f, 0.0f, 1.0f };

		DrawLine line;
		line.color = { 0.0f, 1.0f, 0.0f, 1.0f };

		for (size_t index = 0; index < jointSize; ++index)
		{
			Joint joint = skeleton->mJoints[index];

			const Matrix4x4 boneWorldTransform = pose.jointTransforms[index] * modelTransform;

			sphere.sphere.SetCenter(boneWorldTransform.GetTranslation());

			renderList.AddSphere(sphere);

			if (joint.mParent == -1)
			{
				continue;
			}

			const Matrix4x4f boneWorldTransformNext = pose.jointTransforms[joint.mParent] * modelTransform;

			line.startPosition = boneWorldTransform.GetTranslation();
			line.endPosition = boneWorldTransformNext.GetTranslation();

			animatedkeletonLines[index] = line;
		}

		renderList.AddLines(animatedkeletonLines);
	}

	static void UpdateSpriteTransforms(ECS& ecs)
	{
		for (auto [entityID, sprite3DComponent] : ecs.ViewUsingEntityID<Sprite3DComponent>())
		{
			sprite3DComponent.sprite.mTransform = GetWorldTransform(ecs, entityID);
		}
	}


	static void ProcessSprites(const ECS& ecs, RenderList& renderList)
	{
		ecs.ForEach([&renderList](const Sprite2DComponent& sprite2DComponent)
			{
				renderList.AddSprite(sprite2DComponent.sprite);
			});

		ecs.ForEach([&renderList](const Sprite3DComponent& sprite3DComponent)
			{
				renderList.AddSprite(sprite3DComponent.sprite);
			});
	}

	/*static void ProcessSkyBox(const ECS& ecs)
	{
		auto& graphicsEngine = GetGraphicsEngine();
		ID3D11DeviceContext* context = graphicsEngine.GetContext();

		const std::size_t skyBoxCount = ecs.GetComponentCount<SkyBoxComponent>();

		if (skyBoxCount == 0)
		{
			static ID3D11ShaderResourceView* nullview[1] = { nullptr };
			context->PSSetShaderResources(static_cast<unsigned int>(Global_Slot_CubeMap), 1, nullview);
		}
	}*/

	static void ProcessStaticModels(const ECS& ecs, RenderList& renderList, const GraphicsSettings& graphicsSettings)
	{
		const bool shouldRenderMesh = graphicsSettings.mShouldRenderMesh;
		const bool shouldRenderBoundingBox = graphicsSettings.mShouldRenderBoundingBox;

		ecs.ForEach([&](const EntityID entityID, const MeshComponent& meshComponent)
			{
				if (!meshComponent.mesh)
				{
					return;
				}
				const Transform worldTransform = GetWorldTransform(ecs, entityID);
				if (shouldRenderBoundingBox)
				{
					renderList.AddBoundingBox(DrawBoundingBox(worldTransform, meshComponent.mesh->GetBoundingBox(), Colors::LightGreen));
				}

				if (!shouldRenderMesh)
				{
					return;
				}

				renderList.AddModelInstance(ModelInstance(
					worldTransform,
					meshComponent.mesh,
					ModelAssetHandle(),
					meshComponent.textures[TextureSlots::Albedo],
					meshComponent.textures[TextureSlots::Normal],
					meshComponent.textures[TextureSlots::Material],
					entityID.id
				));
			});

		ecs.ForEach([&](const EntityID entityID, const ModelComponent& modelComponent)
			{
				if (!modelComponent.model)
				{
					return;
				}
				const Transform worldTransform = GetWorldTransform(ecs, entityID);
				if (shouldRenderBoundingBox)
				{
					renderList.AddBoundingBox(DrawBoundingBox(worldTransform, modelComponent.model->GetBoundingBox(), Colors::LightGreen));
				}

				if (!shouldRenderMesh)
				{
					return;
				}

				renderList.AddModelInstance(ModelInstance(
					worldTransform,
					MeshAssetHandle(),
					modelComponent.model,
					modelComponent.textures[TextureSlots::Albedo],
					modelComponent.textures[TextureSlots::Normal],
					modelComponent.textures[TextureSlots::Material],
					entityID.id
				));
			});
	}

	static void ProcessAnimatedModels(const ECS& ecs, RenderList& renderList, const GraphicsSettings& graphicsSettings)
	{
		const bool shouldRenderMesh = graphicsSettings.mShouldRenderMesh;
		const bool shouldRenderBoundingBox = graphicsSettings.mShouldRenderBoundingBox;

		ecs.ForEach([&](const EntityID entityID, const MeshComponent& meshComponent, const AnimationComponent& animationComponent)
			{
				const Transform worldTransform = GetWorldTransform(ecs, entityID);
				if (shouldRenderBoundingBox == true)
				{
					renderList.AddBoundingBox(DrawBoundingBox(worldTransform, meshComponent.mesh->GetBoundingBox()));
				}

				if (!animationComponent.skeleton || !animationComponent.pixelShader || !animationComponent.vertexShader)
				{
					return;
				}

				if (graphicsSettings.mShouldRenderSkeletonLines)
				{
					if (animationComponent.animationPlayer.mModelSpacePose.count > 0)
					{
						PushAnimatedSkeletonLines(worldTransform, animationComponent, renderList);
					}
					else
					{
						PushStaticAnimationLines(worldTransform, animationComponent, renderList);
					}
				}

				if (!shouldRenderMesh)
				{
					return;
				}

				//renderList.AddAnimatedModelInstance(AnimatedModelInstance(worldTransform, Model(meshComponent.textures, meshComponent.mesh, meshComponent.pixelShader, meshComponent.vertexShader), animationComponent.jointMatrices, animationComponent.pixelShader, animationComponent.vertexShader));
			});
	}

	static void ProcessSkyBox(const ECS& ecs, RenderState& renderState)
	{
		auto skyBoxView = ecs.View<SkyBoxComponent>();
		if (!skyBoxView.IsEmpty())
		{
			const SkyBoxComponent& skyBoxComponent = *skyBoxView.begin();
			SkyBox skyBox;

			skyBox.transform = skyBoxComponent.transform;
			skyBox.mesh = skyBoxComponent.mesh;
			skyBox.texture = skyBoxComponent.texture;
			skyBox.pixelShader = skyBoxComponent.pixelShader;
			skyBox.vertexShader = skyBoxComponent.vertexShader;
			skyBox.useSkyBoxFlooring = skyBoxComponent.useSkyBoxFlooring;
			skyBox.useUnityShader = skyBoxComponent.useUnityShader;
			renderState.SetSkyBox(skyBox);
		}
	}

	static void ProcessDirectionalLight(const ECS& ecs, RenderState& renderState)
	{
		auto directionalLightView = ecs.View<DirectionalLightComponent>();
		if (!directionalLightView.IsEmpty())
		{
			const DirectionalLightComponent& directionalLightComponent = *directionalLightView.begin();

			renderState.SetDirectionalLight(directionalLightComponent.directionalLight);
		}
	}

	static void ProcessModels(const ECS& ecs, RenderState& renderState, const GraphicsSettings& graphicsSettings)
	{
		ProcessSkyBox(ecs, renderState);
		ProcessDirectionalLight(ecs, renderState);
		ProcessStaticModels(ecs, renderState.GetRenderList(), graphicsSettings);
		ProcessAnimatedModels(ecs, renderState.GetRenderList(), graphicsSettings);
	}

	static void ProcessArrows(const ECS& ecs, RenderList& renderList)
	{
		ecs.ForEach([&](const EntityID entityID, const ArrowComponent& arrowComponent)
			{
				const Transform& worldTransform = GetWorldTransform(ecs, entityID);
				DrawArrow arrow
				{
					.startPoint = worldTransform.GetPosition(),
					.endPoint = worldTransform.GetPosition() + worldTransform.GetForwardVector() * arrowComponent.length,
					.color = arrowComponent.color,
					.thickness = arrowComponent.thickness,
				};
				renderList.AddArrow(arrow);
			});
	}

	void RenderSystem::Update(ECS& ecs, const float, const Blackboard&)
	{
		UpdateSpriteTransforms(ecs);
	}

	void RenderSystem::Render(const ECS& ecs, const Blackboard& blackboard)
	{
		RenderState& renderState = blackboard.Get<Key_CurrentRenderState>();
		GraphicsSettings& graphicsSettings = blackboard.Get<Key_GraphicsSettings>();
		ProcessSprites(ecs, renderState.GetRenderList());
		ProcessModels(ecs, renderState, graphicsSettings);
		ProcessArrows(ecs, renderState.GetRenderList());
	}

	void RenderSystem::EditorUpdate(ECS& ecs, const Blackboard& blackboard)
	{
		//LateUpdate(ecs, 0.0f, blackboard);
		ecs;
		blackboard;
	}

}