#pragma once
#include <memory>
#include <string>
#include <filesystem>

namespace Simple
{
	class Texture
	{
	public:

		template<typename T>
		Texture(T&& data)
			: mConcept(std::make_unique<TextureModel<T>>(std::forward<T>(data)))
		{
		}

		Texture(const Texture&) = delete;
		Texture(Texture&&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) = delete;

		void SetSlot(unsigned int slot)
		{
			mConcept->SetSlot(slot);
		}

		void Bind(unsigned int slot)
		{
			mConcept->SetSlot(slot);
		}

		void Bind()
		{
			mConcept->Bind(mConcept->GetSlot());
		}

		void* GetShaderResourceView()
		{
			return mConcept->GetShaderResourceView();
		}

		int GetSlot() const
		{
			return mConcept->GetSlot();
		}

		const std::string& GetName() const
		{
			return mConcept->GetName();
		}

		const std::filesystem::path& GetRelativePath() const
		{
			return mConcept->GetRelativePath();
		}
	private:

		class TextureConcept
		{
		public:

			virtual ~TextureConcept() = default;
			virtual void SetSlot(unsigned int slot) = 0;
			virtual unsigned int GetSlot() const = 0;
			virtual void Bind(unsigned int slot) = 0;
			virtual void Bind() = 0;
			virtual void* GetShaderResourceView() = 0;
			virtual const std::string& GetName() const = 0;
			virtual const std::filesystem::path& GetRelativePath() const = 0;
		};

		template<typename T>
		class TextureModel : public TextureConcept
		{
		public:

			explicit TextureModel(T&& texture) 
				: mTexture(std::move(texture))
			{
			}

			void SetSlot(unsigned int slot) override
			{
				mTexture.SetSlot(slot);
			}

			unsigned int GetSlot() const override
			{
				return mTexture.GetSlot();
			}

			void Bind(unsigned int slot) override
			{
				mTexture.Bind(slot);
			}

			void Bind() override
			{
				mTexture.Bind();
			}

			void* GetShaderResourceView() override
			{
				return mTexture.GetShaderResourceView();
			}

			[[nodiscard]] const std::string& GetName() const override
			{
				return mTexture.GetName();
			}

			[[nodiscard]] const std::filesystem::path& GetRelativePath() const override
			{
				return mTexture.GetRelativePath();
			}
		private:

			T mTexture;
		};

	private:

		std::unique_ptr<TextureConcept> mConcept;
	};
}