#pragma once
#include <optional>
#include <string>
#include <filesystem>
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECS/ECSManager.hpp"
#include "Engine/ECS/ECSHandle.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/Graphics/RenderContext.hpp"
#include "Engine/Navmesh/Navmesh.hpp"
#include "Engine/Math/Shapes/Ray3.hpp"
#include "Engine/Math/Point2.hpp"
#include "Engine/Math/Dimension2.hpp"
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{
	
	class Scene final
	{
	public:

		explicit Scene(std::weak_ptr<Blackboard> blackboard, RenderContext&& renderContext);

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
		[[nodiscard]] constexpr const RenderState& GetRenderState() const;
		[[nodiscard]] constexpr RenderContext& GetRenderContext();
		[[nodiscard]] constexpr const RenderContext& GetRenderContext() const;

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

		ECSOwningHandle mECS;
		Navmesh mNavmesh;
		std::filesystem::path mNavmeshPath;
		std::optional<ECSOwningHandle> mBackupECS;
		RenderState mRenderState;
        RenderContext mRenderContext;
		std::weak_ptr<Blackboard> mBlackboard;
		Ray3f mMouseRay;
	};

	inline ECS& Scene::GetECS()
	{
		return mECS.Get();
	}

	inline const ECS& Scene::GetECS() const
	{
		return mECS.Get();
	}

	constexpr ECSHandle Scene::GetECSHandle()
	{
		return ToHandle(mECS);
	}

	constexpr RenderState& Scene::GetRenderState()
	{
		return mRenderState;
	}

	constexpr const RenderState& Scene::GetRenderState() const
	{
		return mRenderState;
	}

	constexpr RenderContext& Scene::GetRenderContext()
	{
		return mRenderContext;
	}

	constexpr const RenderContext& Scene::GetRenderContext() const
	{
		return mRenderContext;
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