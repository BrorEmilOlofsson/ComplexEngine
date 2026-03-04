#pragma once
#include <filesystem>

namespace Simple
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

		[[nodiscard]] const std::filesystem::path& GetPath() const
		{
			return mConcept->GetPath();
		}


	private:

		class Concept
		{
		public:

			virtual ~Concept() = default;

			virtual void Bind() = 0;
			virtual const std::filesystem::path& GetPath() const = 0;
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

			const std::filesystem::path& GetPath() const override
			{
				return mObject.GetPath();
			}

		private:

			T mObject;
		};

	private:


		std::unique_ptr<Concept> mConcept;
	};
}