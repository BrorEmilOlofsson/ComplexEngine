#pragma once
#include "Engine/ECS/ECS.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/Navmesh/Navmesh.hpp"
#include "Engine/Math/Shapes/Ray3.hpp"

namespace Simple
{
	
	class Scene final
	{
	public:

		explicit Scene(std::weak_ptr<Blackboard> blackboard);

		void BeginPlay();
		void EndPlay();
		
		void BeginFrame(Vector2ui clientSize, Point2i mouseScreenPos);
		void Update(const float deltaTime);
		void EditorUpdate();
		void Render();

		[[nodiscard]] constexpr ECS& GetECS();
		[[nodiscard]] constexpr const ECS& GetECS() const;

		[[nodiscard]] constexpr RenderState& GetRenderState();

		[[nodiscard]] constexpr Navmesh& GetNavmesh();
		[[nodiscard]] constexpr const Navmesh& GetNavmesh() const;

		[[nodiscard]] constexpr const std::filesystem::path& GetNavmeshPath() const;
		constexpr void SetNavmeshPath(std::filesystem::path path);

		[[nodiscard]] constexpr const Ray3f& GetMouseRay() const noexcept;

	private:

		ECS mECS;
		Navmesh mNavmesh;
		std::filesystem::path mNavmeshPath;
		ECS mBackupECS;
		RenderState mRenderState;
		std::weak_ptr<Blackboard> mBlackboard;
		Ray3f mMouseRay;
	};

	constexpr ECS& Scene::GetECS()
	{
		return mECS;
	}

	constexpr const ECS& Scene::GetECS() const
	{
		return mECS;
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