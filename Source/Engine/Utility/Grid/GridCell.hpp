#pragma once
#include <unordered_set>

namespace Simple
{

	template<typename T>
	class GridCell final
	{
	public:

		constexpr GridCell() = default;

		[[nodiscard]] constexpr const std::unordered_set<T>& GetObjects() const noexcept;
		[[nodiscard]] constexpr std::unordered_set<T>& GetObjects() noexcept;
		constexpr void Add(const T& object);
		constexpr void Add(T&& object);
		constexpr void Remove(const T& object);
		constexpr void Clear() noexcept;
		[[nodiscard]] constexpr bool Contains(const T& object) const noexcept;

	private:

		std::unordered_set<T> mObjects;
	};

	template<typename T>
	constexpr const std::unordered_set<T>& GridCell<T>::GetObjects() const noexcept
	{
		return mObjects;
	}

	template<typename T>
	constexpr std::unordered_set<T>& GridCell<T>::GetObjects() noexcept
	{
		return mObjects;
	}

	template<typename T>
	constexpr void GridCell<T>::Add(const T& object)
	{
		mObjects.insert(object);
	}

	template<typename T>
	constexpr void GridCell<T>::Add(T&& object)
	{
		mObjects.insert(std::move(object));
	}

	template<typename T>
	constexpr void GridCell<T>::Remove(const T& object)
	{
		mObjects.erase(object);
	}

	template<typename T>
	constexpr void GridCell<T>::Clear() noexcept
	{
		mObjects.clear();
	}

	template<typename T>
	constexpr bool GridCell<T>::Contains(const T& object) const noexcept
	{
		return mObjects.find(object) != mObjects.end();
	}
}