#include "Engine/Precompiled/EnginePch.hpp"
#include "Scene.hpp"
#include "Utility/CameraMath.hpp"
#include "Utility/ShapeMath.hpp"
#include "Utility/Blackboard.hpp"
#include <memory>
#include "Engine/ECS/ECSRegistry.hpp"

namespace Simple
{

	Scene::Scene(std::weak_ptr<Blackboard> blackboard)
		: mECS(blackboard.lock()->Get<Key_ECSRegistry>())
		, mBackupECS(blackboard.lock()->Get<Key_ECSRegistry>())
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
		mECS.BeginPlay(blackboard);
	}

	void Scene::EndPlay()
	{
		Blackboard& blackboard = *mBlackboard.lock();
		blackboard.Insert<Key_SceneRenderState>(mRenderState);
		blackboard.Insert<Key_CurrentRenderState>(mRenderState);
		blackboard.Insert<Key_Navmesh>(mNavmesh);
		mECS.EndPlay(blackboard);
		blackboard.Insert<Key_IsPlaying>(false);
		mECS = mBackupECS;
	}

	template<typename T>
	[[nodiscard]] constexpr Point2<T> InvertY(const Point2<T>& point, const T& y)
	{
		return Point2<T>(point.x, y - point.y);
	}

	static Ray3f CalculateMouseRay(const Camera& camera, const AABB2i& renderAABB, const Vector2ui windowSize, const Point2i& mouseScreenPos)
	{
		const AABB2f renderAABBf = ToAABB<float>(renderAABB);

		const Point2f mousePos = Remap(Point2f(mouseScreenPos), renderAABBf, AABB2f::FromDefaultAndExtent(Vector2f(windowSize)));
		const Point2f correctedMousePos = InvertY(mousePos, static_cast<float>(windowSize.y));
		return CalculateMouseRay(camera, Point2i(correctedMousePos), windowSize);
	}

	void Scene::BeginFrame(Vector2ui clientSize, Point2i mouseScreenPos)
	{
		if (mRenderState.GetCamera())
		{
			mMouseRay = CalculateMouseRay(
				*mRenderState.GetCamera(),
				 mRenderState.GetRenderRect().value_or(AABB2i::FromDefaultAndExtent(Vector2i(clientSize))),
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
		mECS.EarlyUpdate(deltaTime, blackboard);
		mECS.Update(deltaTime, blackboard);
	}

	void Scene::EditorUpdate()
	{
		Blackboard& blackboard = *mBlackboard.lock();
		blackboard.Insert<Key_SceneRenderState>(mRenderState);
		blackboard.Insert<Key_CurrentRenderState>(mRenderState);
		blackboard.Insert<Key_Navmesh>(mNavmesh);
		mECS.EditorUpdate(blackboard);
	}

	void Scene::Render()
	{
		Blackboard& blackboard = *mBlackboard.lock();
		blackboard.Insert<Key_SceneRenderState>(mRenderState);
		blackboard.Insert<Key_CurrentRenderState>(mRenderState);
		blackboard.Insert<Key_Navmesh>(mNavmesh);
		mECS.Render(blackboard);
	}
}