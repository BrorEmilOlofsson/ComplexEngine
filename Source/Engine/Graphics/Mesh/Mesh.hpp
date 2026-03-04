#pragma once
#include <memory>
#include "Engine/Utility/MeshData.hpp"
#include "Engine/Math/Shapes/AABB3.hpp"

namespace Simple
{

	class Mesh final
	{
	public:

		template<typename T>
		explicit Mesh(T&& mesh)
			: mMesh(std::make_unique<MeshModel<T>>(std::forward<T>(mesh)))
		{
		}

		~Mesh() = default;
		Mesh(Mesh&&) noexcept = delete;
		Mesh(const Mesh&) = delete;
		Mesh& operator=(Mesh&&) noexcept = delete;
		Mesh& operator=(const Mesh&) = delete;

		[[nodiscard]] const MeshData<Vertex>& GetMeshData() const
		{
			return mMesh->GetMeshData();
		}

		[[nodiscard]] const std::filesystem::path& GetName() const
		{
			return mMesh->GetName();
		}

		[[nodiscard]] const std::filesystem::path& GetPath() const
		{
			return mMesh->GetPath();
		}

		[[nodiscard]] AABB3f GetBoundingBox() const
		{
			return mMesh->GetBoundingBox();
		}

		void Render()
		{
			mMesh->Render();
		}

	private:

		class MeshConcept
		{
		public:

			MeshConcept() = default;
			virtual ~MeshConcept() = default;

			virtual const MeshData<Vertex>& GetMeshData() const = 0;
			virtual const std::filesystem::path& GetName() const = 0;
			virtual const std::filesystem::path& GetPath() const = 0;
			virtual AABB3f GetBoundingBox() const = 0;
			virtual void Render() = 0;
 		};

		template<typename T>
		class MeshModel final : public MeshConcept
		{
		public:

			explicit MeshModel(T&& mesh)
				: mMesh(std::move(mesh))
			{
			}

			const MeshData<Vertex>& GetMeshData() const override
			{
				return mMesh.GetMeshData();
			}

			const std::filesystem::path& GetName() const override
			{
				return mMesh.GetName();
			}

			const std::filesystem::path& GetPath() const override
			{
				return mMesh.GetPath();
			}

			AABB3f GetBoundingBox() const override
			{
				return mMesh.GetBoundingBox();
			}

			void Render() override
			{
				mMesh.Render();
			}

		private:

			T mMesh;
		};

	private:

		std::unique_ptr<MeshConcept> mMesh;
	};
}