#pragma once
#include <memory>

namespace Simple
{

	template<typename T>
	class Asset final
	{
	public:

		constexpr Asset() = default;

		constexpr explicit Asset(std::shared_ptr<T> asset)
			: mAsset(std::move(asset))
		{
		}

		[[nodiscard]] std::shared_ptr<T> Get() const
		{
			return mAsset;
		}

		[[nodiscard]] constexpr operator bool() const
		{
			return mAsset != nullptr;
		}

		static Asset<T> Empty()
		{
			return Asset<T>();
		}

	private:

		std::shared_ptr<T> mAsset;
	};

}