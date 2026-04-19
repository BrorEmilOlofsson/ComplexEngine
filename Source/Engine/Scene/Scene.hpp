#pragma once
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECS/ECSManager.hpp"
#include "Engine/ECS/ECSHandle.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/Navmesh/Navmesh.hpp"
#include "Engine/Math/Shapes/Ray3.hpp"

namespace CLX
{
	
	class Scene final
	{
	public:

		explicit Scene(std::weak_ptr<Blackboard> blackboard);

		void BeginPlay();
		void EndPlay();
		
		void BeginFrame(Dimension2u clientSize, Point2i mouseScreenPos);
		void Update(const float deltaTime);
		void EditorUpdate();
		void Render();

		[[nodiscard]] inline ECS& GetECS();
		[[nodiscard]] inline const ECS& GetECS() const;
		[[nodiscard]] constexpr ECSHandle GetECSHandle();

		[[nodiscard]] constexpr RenderState& GetRenderState();

		[[nodiscard]] constexpr Navmesh& GetNavmesh();
		[[nodiscard]] constexpr const Navmesh& GetNavmesh() const;

		[[nodiscard]] constexpr const std::filesystem::path& GetNavmeshPath() const;
		constexpr void SetNavmeshPath(std::filesystem::path path);

		[[nodiscard]] constexpr const Ray3f& GetMouseRay() const noexcept;

		[[nodiscard]] std::string GetName() const
		{
			return "Scene Test";
		}

	private:

		ECSOwningHandle mECSHandle;
		Navmesh mNavmesh;
		std::filesystem::path mNavmeshPath;
		ECSOwningHandle mBackupECSHandle;
		RenderState mRenderState;
		std::weak_ptr<Blackboard> mBlackboard;
		Ray3f mMouseRay;
	};

	inline ECS& Scene::GetECS()
	{
		return mECSHandle.Get();
	}

	inline const ECS& Scene::GetECS() const
	{
		return mECSHandle.Get();
	}

	constexpr ECSHandle Scene::GetECSHandle()
	{
		return ToHandle(mECSHandle);
	}

	[[nodiscard]] constexpr RenderState& Scene::GetRenderState()
	{
		return mRenderState;
	}

	constexpr Navmesh& Scene::GetNavmesh()
	{
		return mNavmesh;
	}

	constexpr const Navmesh& Scene::GetNavmesh() const
	{
		return mNavmesh;
	}

	constexpr const std::filesystem::path& Scene::GetNavmeshPath() const
	{
		return mNavmeshPath;
	}

	constexpr void Scene::SetNavmeshPath(std::filesystem::path path)
	{
		mNavmeshPath = std::move(path);
	}

	constexpr const Ray3f& Scene::GetMouseRay() const noexcept
	{
		return mMouseRay;
	}
}