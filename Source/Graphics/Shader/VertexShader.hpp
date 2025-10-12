#pragma once
#include <filesystem>

namespace Simple
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

		[[nodiscard]] const std::filesystem::path& GetPath() const
		{
			return mConcept->GetPath();
		}

	private:

		class VertexShaderConcept
		{
		public:

			virtual ~VertexShaderConcept() = default;

			virtual void Bind() = 0;
			virtual const std::filesystem::path& GetPath() const = 0;
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

			const std::filesystem::path& GetPath() const override
			{
				return mObject.GetPath();
			}

		private:

			T mObject;
		};

	private:


		std::unique_ptr<VertexShaderConcept> mConcept;
	};
}