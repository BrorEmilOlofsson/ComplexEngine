#pragma once
#include <filesystem>

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

		private:

			T mObject;
		};

	private:


		std::unique_ptr<Concept> mConcept;
	};
}