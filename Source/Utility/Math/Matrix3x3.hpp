#pragma once
#include <array>

namespace Simple
{

	template<typename T>
	class Matrix3x3 final
	{
	public:

		constexpr Matrix3x3() noexcept;
		constexpr explicit Matrix3x3(const std::array<T, 9>& aArray) noexcept;

		[[nodiscard]] constexpr T& operator()(const unsigned int aRow, const unsigned int aColumn);
		[[nodiscard]] constexpr const T& operator()(const unsigned int aRow, const unsigned int aColumn) const;

		[[nodiscard]] constexpr T* GetDataPtr() noexcept;
		[[nodiscard]] constexpr const T* GetDataPtr() const noexcept;

		[[nodiscard]] static constexpr Matrix3x3<T> Identity() noexcept;
		[[nodiscard]] static constexpr Matrix3x3<T> GetTransposed(const Matrix3x3<T>& aMatrix) noexcept;


	private:

		static constexpr std::size_t Dimensions = 3;
		static constexpr std::size_t ValueCount = 3 * 3;
		T mValues[ValueCount]{};
	};

	using Matrix3x3f = Matrix3x3<float>;

	template<typename T>
	constexpr Matrix3x3<T>::Matrix3x3() noexcept
	{
		*this = Identity();
	}

	template<typename T>
	constexpr Matrix3x3<T>::Matrix3x3(const std::array<T, 9>& aArray) noexcept
	{
		mValues[0] = aArray[0];
		mValues[1] = aArray[1];
		mValues[2] = aArray[2];
		mValues[3] = aArray[3];
		mValues[4] = aArray[4];
		mValues[5] = aArray[5];
		mValues[6] = aArray[6];
		mValues[7] = aArray[7];
		mValues[8] = aArray[8];
	}

	template<typename T>
	constexpr T& Matrix3x3<T>::operator()(const unsigned int aRow, const unsigned int aColumn)
	{
		const unsigned int index = aRow * Dimensions + aColumn;
		return mValues[index];
	}

	template<typename T>
	constexpr const T& Matrix3x3<T>::operator()(const unsigned int aRow, const unsigned int aColumn) const
	{
		const unsigned int index = aRow * Dimensions + aColumn;
		return mValues[index];
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
	constexpr Matrix3x3<T> Matrix3x3<T>::GetTransposed(const Matrix3x3<T>& aMatrix) noexcept
	{
		Matrix3x3<T> transposed;

		for (unsigned int i = 0; i < Dimensions; i++)
		{
			for (unsigned int j = 0; j < Dimensions; j++)
			{
				transposed(i, j) = aMatrix(j, i);
			}
		}

		return transposed;
	}
}