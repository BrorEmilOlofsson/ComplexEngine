#pragma once
#include <memory>
#include <string>
#include "Engine/Math/Shapes/AABB3.hpp"
#include "Engine/Graphics/Model/Skeleton.hpp"

namespace Simple
{

	class AnimatedModel final
	{
	public:

		template<typename T>
		explicit AnimatedModel(T&& model)
			: mConcept(std::make_unique<AnimatedModelModel<T>>(std::move(model)))
		{
		}

		~AnimatedModel() = default;
		AnimatedModel(const AnimatedModel&) = delete;
		AnimatedModel(AnimatedModel&&) noexcept = default;
		AnimatedModel& operator=(const AnimatedModel&) = delete;
		AnimatedModel& operator=(AnimatedModel&&) noexcept = default;

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

		const Skeleton& GetSkeleton() const
		{
			return mConcept->GetSkeleton();
		}

		void Render()
		{
			mConcept->Render();
		}

	private:

		class AnimatedModelConcept
		{
		public:

			AnimatedModelConcept() = default;
			virtual ~AnimatedModelConcept() = default;

			virtual const std::string& GetName() const = 0;
			virtual const std::filesystem::path& GetPath() const = 0;
			virtual AABB3f GetBoundingBox() const = 0;
			virtual const Skeleton& GetSkeleton() const = 0;
			virtual void Render() = 0;
		};

		template<typename T>
		class AnimatedModelModel final : public AnimatedModelConcept
		{
		public:

			explicit AnimatedModelModel(T&& model)
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

			const Skeleton& GetSkeleton() const override
			{
				return mModel.GetSkeleton();
			}

			void Render() override
			{
				mModel.Render();
			}

		private:

			T mModel;
		};

	private:

		std::unique_ptr<AnimatedModelConcept> mConcept;
	};
}