#pragma once
#include <vector>
#include <filesystem>
#include "Engine/Math/Shapes/AABB3.hpp"

namespace CLX
{


	class Model final
	{
	public:

		template<typename T>
		explicit Model(T&& model)
			: mConcept(std::make_unique<ModelModel<T>>(std::move(model)))
		{
		}

		~Model() = default;
		Model(Model&&) noexcept = default;
		Model(const Model&) = delete;
		Model& operator=(Model&&) noexcept = default;
		Model& operator=(const Model&) = delete;

		[[nodiscard]] const std::string& GetName() const
		{
			return mConcept->GetName();
		}

		[[nodiscard]] const std::filesystem::path& GetPath() const
		{
			return mConcept->GetPath();
		}

		[[nodiscard]] AABB3f GetBoundingBox() const
		{
			return mConcept->GetBoundingBox();
		}

		template<typename T>
		[[nodiscard]] T* Get()
		{
			if (auto* model = dynamic_cast<ModelModel<T>*>(mConcept.get()))
			{
				return &model->Get();
			}

			return nullptr;
		}

		template<typename T>
		[[nodiscard]] const T* Get() const
		{
			if (auto* model = dynamic_cast<ModelModel<T>*>(mConcept.get()))
			{
				return &model->Get();
			}
			
			return nullptr;
        }

	private:

		class ModelConcept
		{
		public:

			ModelConcept() = default;
			virtual ~ModelConcept() = default;

			virtual const std::string& GetName() const = 0;
			virtual const std::filesystem::path& GetPath() const = 0;
			virtual AABB3f GetBoundingBox() const = 0;
		};

		template<typename T>
		class ModelModel final : public ModelConcept
		{
		public:

			explicit ModelModel(T&& model)
				: mModel(std::move(model))
			{
			}

			const std::string& GetName() const override
			{
				return mModel.GetName();
			}

			const std::filesystem::path& GetPath() const override
			{
				return mModel.GetPath();
			}

			AABB3f GetBoundingBox() const override
			{
				return mModel.GetBoundingBox();
			}

			T& Get()
			{
				return mModel;
            }

			const T& Get() const
			{
				return &mModel;
            }


		private:

			T mModel;
		};

	private:

		std::unique_ptr<ModelConcept> mConcept;
	};
}