#pragma once
#include <memory>
#include "Engine/Utility/MeshData.hpp"
#include "Engine/Math/Shapes/AABB3.hpp"

namespace CLX
{

    class Mesh final
    {
    public:

        template<typename T>
        explicit Mesh(T&& mesh)
            : mMesh(std::make_unique<MeshModel<T>>(std::forward<T>(mesh)))
        {}

        ~Mesh() = default;
        Mesh(Mesh&&) noexcept = default;
        Mesh(const Mesh&) = delete;
        Mesh& operator=(Mesh&&) noexcept = default;
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

        template<typename T>
        [[nodiscard]] T* Get()
        {
            if (auto* mesh = dynamic_cast<MeshModel<T>*>(mMesh.get()))
            {
                return &mesh->Get();
            }
            return nullptr;
        }

        template<typename T>
        [[nodiscard]] const T* Get() const
        {
            if (auto* mesh = dynamic_cast<MeshModel<T>*>(mMesh.get()))
            {
                return &mesh->Get();
            }
            return nullptr;
        }

        template<typename T>
        [[nodiscard]] T& GetUnsafe()
        {
            return reinterpret_cast<MeshModel<T>*>(mMesh.get())->Get();
        }

        template<typename T>
        [[nodiscard]] const T& GetUnsafe() const
        {
            return reinterpret_cast<const MeshModel<T>*>(mMesh.get())->Get();
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
        };

        template<typename T>
        class MeshModel final : public MeshConcept
        {
        public:

            explicit MeshModel(T&& mesh)
                : mMesh(std::move(mesh))
            {}

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

            [[nodiscard]] T& Get()
            {
                return mMesh;
            }

            [[nodiscard]] const T& Get() const
            {
                return mMesh;
            }

        private:

            T mMesh;
        };

    private:

        std::unique_ptr<MeshConcept> mMesh;
    };
}