#pragma once
#include <memory>

namespace CLX
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
		Texture(Texture&&) = default;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) = default;

		void SetSlot(uint32_t slot)
		{
			mConcept->SetSlot(slot);
		}

		void Bind(uint32_t slot)
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

		uint32_t GetSlot() const
		{
			return mConcept->GetSlot();
		}

	private:

		class TextureConcept
		{
		public:

			virtual ~TextureConcept() = default;
			virtual void SetSlot(uint32_t slot) = 0;
			virtual uint32_t GetSlot() const = 0;
			virtual void Bind(uint32_t slot) = 0;
			virtual void Bind() = 0;
			virtual void* GetShaderResourceView() = 0;
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

			uint32_t GetSlot() const override
			{
				return mTexture.GetSlot();
			}

			void Bind(uint32_t slot) override
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

		private:

			T mTexture;
		};

	private:

		std::unique_ptr<TextureConcept> mConcept;
	};
}