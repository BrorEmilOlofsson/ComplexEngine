#pragma once
#include <array>

namespace Simple
{

	template<typename T>
	class Matrix3x3 final
	{
	public:

		constexpr Matrix3x3() noexcept;
		constexpr explicit Matrix3x3(const std::array<T, 9>& array) noexcept;

		[[nodiscard]] constexpr T& operator[](const unsigned int index) noexcept;
		[[nodiscard]] constexpr const T& operator[](const unsigned int index) const noexcept;
		[[nodiscard]] constexpr T& operator()(const unsigned int row, const unsigned int column);
		[[nodiscard]] constexpr const T& operator()(const unsigned int row, const unsigned int column) const;

		[[nodiscard]] constexpr T* GetDataPtr() noexcept;
		[[nodiscard]] constexpr const T* GetDataPtr() const noexcept;

		[[nodiscard]] static constexpr Matrix3x3<T> Identity() noexcept;
		[[nodiscard]] static constexpr Matrix3x3<T> GetTransposed(const Matrix3x3<T>& aMatrix) noexcept;

		[[nodiscard]] static constexpr unsigned int GetIndex(const unsigned int row, const unsigned int column) noexcept
		{
			return row * Dimensions + column;
		}

	private:

		static constexpr std::size_t Dimensions = 3;
		static constexpr std::size_t ValueCount = 3 * 3;
		std::array<T, ValueCount> mValues{};
	};

	using Matrix3x3f = Matrix3x3<float>;

	template<typename T>
	constexpr Matrix3x3<T>::Matrix3x3() noexcept
	{
		*this = Identity();
	}

	template<typename T>
	constexpr Matrix3x3<T>::Matrix3x3(const std::array<T, 9>& array) noexcept
	{
		mValues[0] = array[0];
		mValues[1] = array[1];
		mValues[2] = array[2];
		mValues[3] = array[3];
		mValues[4] = array[4];
		mValues[5] = array[5];
		mValues[6] = array[6];
		mValues[7] = array[7];
		mValues[8] = array[8];
	}

	template<typename T>
	constexpr T& Matrix3x3<T>::operator[](const unsigned int index) noexcept
	{
		return mValues[index];
	}

	template<typename T>
	constexpr const T& Matrix3x3<T>::operator[](const unsigned int index) const noexcept
	{
		return mValues[index];
	}

	template<typename T>
	constexpr T& Matrix3x3<T>::operator()(const unsigned int row, const unsigned int column)
	{
		return mValues[GetIndex(row, column)];
	}

	template<typename T>
	constexpr const T& Matrix3x3<T>::operator()(const unsigned int row, const unsigned int column) const
	{
		return mValues[GetIndex(row, column)];
	}

	template<typename T>
	constexpr T* Matrix3x3<T>::GetDataPtr() noexcept
	{
		return &mValues[0];
	}

	template<typename T>
	constexpr const T* Matrix3x3<T>::GetDataPtr() const noexcept
	{
		return &mValues[0];
	}

	template<typename T>
	constexpr Matrix3x3<T> Matrix3x3<T>::Identity() noexcept
	{
		return Matrix3x3<T>
			(
				{
					1, 0, 0,
					0, 1, 0,
					0, 0, 1
				}
			);
	}

	template<typename T>
	constexpr Matrix3x3<T> Matrix3x3<T>::GetTransposed(const Matrix3x3<T>& matrix) noexcept
	{
		Matrix3x3<T> transposed;

		for (unsigned int i = 0; i < Dimensions; i++)
		{
			for (unsigned int j = 0; j < Dimensions; j++)
			{
				transposed(i, j) = matrix(j, i);
			}
		}

		return transposed;
	}

	[[nodiscard]] constexpr bool operator==(const Matrix3x3f& a, const Matrix3x3f& b) noexcept
	{
		for (unsigned int i = 0; i < 9; ++i)
		{
			if (a[i] != b[i])
			{
				return false;
			}
		}
		return true;
	}
}