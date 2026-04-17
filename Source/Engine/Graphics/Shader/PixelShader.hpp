#pragma once
#include <memory>
#include <utility>

namespace CLX
{

	class PixelShader final
	{
	public:

		template<typename T>
		PixelShader(T&& object)
			: mConcept(std::make_unique<Model<T>>(object))
		{
		}

		void Bind()
		{
			mConcept->Bind();
		}

		template<typename T>
		[[nodiscard]] T& GetUnsafe()
		{
			return reinterpret_cast<Model<T>*>(mConcept.get())->Get();
		}

		template<typename T>
		[[nodiscard]] const T& GetUnsafe() const
		{
			return reinterpret_cast<const Model<T>*>(mConcept.get())->Get();
		}

	private:

		class Concept
		{
		public:

			virtual ~Concept() = default;

			virtual void Bind() = 0;
		};

		template<typename T>
		class Model final : public Concept
		{
		public:

			template<typename T>
			Model(T&& object)
				: mObject(std::move(object))
			{
			}

			void Bind() override
			{
				mObject.Bind();
			}

			T& Get()
			{
				return mObject;
			}

			[[nodiscard]] T& Get() const
			{
				return mObject;
			}

		private:

			T mObject;
		};

	private:


		std::unique_ptr<Concept> mConcept;
	};
}