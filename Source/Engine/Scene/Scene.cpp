#include "Engine/Precompiled/EnginePch.hpp"
#include "Scene.hpp"
#include <memory>
#include "Engine/Utility/CameraMath.hpp"
#include "Engine/Math/ShapeMath.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/ECS/ECSRegistry.hpp"

namespace CLX
{

	ECSOwningHandle CreateECS(std::weak_ptr<Blackboard> blackboard)
	{
        std::shared_ptr<Blackboard> sharedBlackboard = blackboard.lock();
        return CreateECS(sharedBlackboard->Get<Key_ECSManager>(), sharedBlackboard->Get<Key_ECSRegistry>(), sharedBlackboard->Get<Key_EntityIDGenerator>());
	}

	Scene::Scene(std::weak_ptr<Blackboard> blackboard)
		: mECS(CreateECS(blackboard))
		, mBackupECS(std::nullopt)
		, mBlackboard(blackboard)
	{
		mBlackboard.lock()->Insert<Key_IsPlaying>(false);
	}

	void Scene::BeginPlay()
	{
		mBackupECS = mECS;
		Blackboard& blackboard = *mBlackboard.lock();
		blackboard.Insert<Key_SceneRenderState>(mRenderState);
		blackboard.Insert<Key_CurrentRenderState>(mRenderState);
		blackboard.Insert<Key_Navmesh>(mNavmesh);
		blackboard.Insert<Key_IsPlaying>(true);
		mECS.Get().BeginPlay(blackboard);
	}

	void Scene::EndPlay()
	{
		Blackboard& blackboard = *mBlackboard.lock();
		blackboard.Insert<Key_SceneRenderState>(mRenderState);
		blackboard.Insert<Key_CurrentRenderState>(mRenderState);
		blackboard.Insert<Key_Navmesh>(mNavmesh);
		mECS.Get().EndPlay(blackboard);
		blackboard.Insert<Key_IsPlaying>(false);
		mECS = std::move(mBackupECS.value());
		mBackupECS = std::nullopt;
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> InvertY(const Point2<T>& point, const T& y)
	{
		return Point2<T>(point.x, y - point.y);
	}

	static Ray3f CalculateMouseRay(const Camera& camera, const AABB2i& renderAABB, const Dimension2u windowSize, const Point2i& mouseScreenPos)
	{
		const AABB2f renderAABBf = ToAABB<float>(renderAABB);

		const Point2f mousePos = Remap(Point2f(mouseScreenPos), renderAABBf, AABB2f::FromDefaultAndExtent(static_cast<Vector2f>(ToVector2(windowSize))));
		const Point2f correctedMousePos = InvertY(mousePos, static_cast<float>(windowSize.GetHeight()));
		return CalculateMouseRay(camera, Point2i(correctedMousePos), windowSize);
	}

	void Scene::BeginFrame(Dimension2u clientSize, Point2i mouseScreenPos)
	{
		if (mRenderState.GetCamera())
		{
			mMouseRay = CalculateMouseRay(
				*mRenderState.GetCamera(),
				 mRenderState.GetRenderRect().value_or(AABB2i::FromDefaultAndExtent(ToVector2(clientSize))),
				clientSize,
				mouseScreenPos
			);
		}
		mRenderState.Reset();
	}

	void Scene::Update(const float deltaTime)
	{
		Blackboard& blackboard = *mBlackboard.lock();
		blackboard.Insert<Key_SceneRenderState>(mRenderState);
		blackboard.Insert<Key_CurrentRenderState>(mRenderState);
		blackboard.Insert<Key_Navmesh>(mNavmesh);
		mECS.Get().EarlyUpdate(deltaTime, blackboard);
		mECS.Get().Update(deltaTime, blackboard);
	}

	void Scene::EditorUpdate()
	{
		Blackboard& blackboard = *mBlackboard.lock();
		blackboard.Insert<Key_SceneRenderState>(mRenderState);
		blackboard.Insert<Key_CurrentRenderState>(mRenderState);
		blackboard.Insert<Key_Navmesh>(mNavmesh);
		mECS.Get().EditorUpdate(blackboard);
	}

	void Scene::Render()
	{
		Blackboard& blackboard = *mBlackboard.lock();
		blackboard.Insert<Key_SceneRenderState>(mRenderState);
		blackboard.Insert<Key_CurrentRenderState>(mRenderState);
		blackboard.Insert<Key_Navmesh>(mNavmesh);
		mECS.Get().Render(blackboard);
	}
}