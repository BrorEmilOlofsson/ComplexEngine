#pragma once
#include <vector>
#include <filesystem>
#include "Utility/Shapes/AABB3.hpp"

namespace Simple
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
		Model(Model&&) noexcept = delete;
		Model(const Model&) = delete;
		Model& operator=(Model&&) noexcept = delete;
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

		void Render()
		{
			mConcept->Render();
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
			virtual void Render() = 0;
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

			void Render() override
			{
				mModel.Render();
			}

		private:

			T mModel;
		};

	private:

		std::unique_ptr<ModelConcept> mConcept;
	};
}