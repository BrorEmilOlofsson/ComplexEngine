#pragma once
#include <filesystem>

namespace CLX
{

	class VertexShader final
	{
	public:

		template<typename T>
		VertexShader(T&& object)
			: mConcept(std::make_unique<VertexShaderModel<T>>(object))
		{
		}

		void Bind()
		{
			mConcept->Bind();
		}

	private:

		class VertexShaderConcept
		{
		public:

			virtual ~VertexShaderConcept() = default;

			virtual void Bind() = 0;
		};

		template<typename T>
		class VertexShaderModel final : public VertexShaderConcept
		{
		public:

			template<typename T>
			VertexShaderModel(T&& object)
				: mObject(std::move(object))
			{
			}

			void Bind()
			{
				mObject.Bind();
			}

		private:

			T mObject;
		};

	private:


		std::unique_ptr<VertexShaderConcept> mConcept;
	};
}