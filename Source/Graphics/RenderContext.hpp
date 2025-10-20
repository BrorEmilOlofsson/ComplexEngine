#pragma once
#include <memory>
#include <string>
#include <vector>

namespace Simple
{

	struct GBufferData final
	{
		void* srv = nullptr;
		std::string name;
	};

	class RenderContext final
	{
	public:

		template<typename T>
		explicit RenderContext(const T& data)
			: mConcept(std::make_unique<Model<T>>(data))
		{
		}

		template<typename T>
		explicit RenderContext(T&& data)
			: mConcept(std::make_unique<Model<T>>(std::move(data)))
		{
		}

		RenderContext(const RenderContext&) = delete;
		RenderContext(RenderContext&&) noexcept = default;
		RenderContext& operator=(const RenderContext&) = delete;
		RenderContext& operator=(RenderContext&&) = default;

		[[nodiscard]] std::vector<void*> GetGBufferSRVs() const
		{
			return mConcept->GetGBufferSRVs();
		}

		[[nodiscard]] Vector2ui GetBufferSize() const
		{
			return mConcept->GetBufferSize();
		}

		[[nodiscard]] uint32_t GetObjectIDAt(const Point2i& point)
		{
			return mConcept->GetObjectIDAt(point);
		}

		[[nodiscard]] void* GetOutputRenderTarget()
		{
			return mConcept->GetOutputRenderTarget();
		}

		[[nodiscard]] void* GetOutputSRV()
		{
			return mConcept->GetOutputSRV();
		}

		void ResizeBuffers(const Vector2ui& size)
		{
			mConcept->ResizeBuffers(size);
		}

		void ClearBuffers()
		{
			mConcept->ClearBuffers();
		}

		void SetGBufferRenderTargets()
		{
			mConcept->SetGBufferRenderTargets();
		}
		
		void SetOutputRenderTarget()
		{
			mConcept->SetOutputRenderTarget();
		}

		void SetGBufferShaderResources()
		{
			mConcept->SetGBufferShaderResources();
		}

	private:

		class Concept
		{
		public:

			Concept() = default;
			virtual ~Concept() {}

			virtual std::vector<void*> GetGBufferSRVs() = 0;
			virtual Vector2ui GetBufferSize() = 0;
			virtual uint32_t GetObjectIDAt(const Point2i& point) = 0;
			virtual void* GetOutputRenderTarget() = 0;
			virtual void* GetOutputSRV() = 0;
			virtual void ResizeBuffers(const Vector2ui& size) = 0;
			virtual void ClearBuffers() = 0;
			virtual void SetOutputRenderTarget() = 0;
			virtual void SetGBufferRenderTargets() = 0;
			virtual void SetGBufferShaderResources() = 0;
		};

		template<typename T>
		class Model final : public Concept
		{
		public:

			template<typename T>
			explicit Model(const T& object)
				: mObject(object)
			{
			}

			template<typename T>
			explicit Model(T&& object)
				: mObject(std::move(object))
			{
			}

			std::vector<void*> GetGBufferSRVs() override
			{
				return mObject.GetGBufferSRVs();
			}

			Vector2ui GetBufferSize() override
			{
				return mObject.GetBufferSize();
			}

			uint32_t GetObjectIDAt(const Point2i& point) override
			{
				return mObject.GetObjectIDAt(point);
			}

			void* GetOutputRenderTarget() override
			{
				return &mObject.GetOutputRenderTarget();
			}

			void* GetOutputSRV() override
			{
				return mObject.GetOutputSRV();
			}

			void ResizeBuffers(const Vector2ui& size) override
			{
				mObject.ResizeBuffers(size);
			}

			void ClearBuffers() override
			{
				mObject.ClearBuffers();
			}

			void SetOutputRenderTarget()
			{
				mObject.SetOutputRenderTarget();
			}

			void SetGBufferRenderTargets()
			{
				mObject.SetGBufferRenderTargets();
			}

			void SetGBufferShaderResources() override
			{
				mObject.SetGBufferShaderResources();
			}
		private:

			T mObject = {};
		};

	private:

		std::unique_ptr<Concept> mConcept;
	};
}