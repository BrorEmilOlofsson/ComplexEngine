#pragma once
#include "Engine/Math/UnitVector2.hpp"
#include "Engine/Math/Vector2.hpp"
#include <array>

namespace Simple
{

    template<typename T>
    class RotationMatrix2
    {
    public:

        constexpr RotationMatrix2() = default;

        [[nodiscard]] static constexpr RotationMatrix2<T> FromValues(const std::array<T, 4>& values);

        [[nodiscard]] static constexpr RotationMatrix2<T> FromX(const UnitVector2<T>& xAxis);
        [[nodiscard]] static constexpr RotationMatrix2<T> FromY(const UnitVector2<T>& yAxis);
        
        [[nodiscard]] constexpr const UnitVector2<T>& GetRight() const noexcept;
        [[nodiscard]] constexpr const UnitVector2<T>& GetUp() const noexcept;

        [[nodiscard]] constexpr const T& operator[](const unsigned int index) const noexcept;

    private:

        constexpr RotationMatrix2(const UnitVector2<T>& right, const UnitVector2<T>& up);

    private:

        UnitVector2<T> mRight = UnitVector2<T>::Right();
        UnitVector2<T> mUp = UnitVector2<T>::Up();
    };

    using RotationMatrix2f = RotationMatrix2<float>;
    using RotationMatrix2d = RotationMatrix2<double>;

    template<typename T>
    constexpr RotationMatrix2<T>::RotationMatrix2(const UnitVector2<T>& right, const UnitVector2<T>& up)
        : mRight(right)
        , mUp(up)
    {
    }

    template<typename T>
    constexpr RotationMatrix2<T> RotationMatrix2<T>::FromValues(const std::array<T, 4>& values)
    {
        return RotationMatrix2<T>(UnitVector2<T>(values[0], values[1]), UnitVector2<T>(values[2], values[3]));
    }

    template<typename T>
    constexpr RotationMatrix2<T> RotationMatrix2<T>::FromX(const UnitVector2<T>& xAxis)
    {
        const UnitVector2<T> yAxis(-xAxis.Y(), xAxis.X());
        return RotationMatrix2<T>(xAxis, yAxis);
    }

    template<typename T>
    constexpr RotationMatrix2<T> RotationMatrix2<T>::FromY(const UnitVector2<T>& yAxis)
    {
        const UnitVector2<T> xAxis(yAxis.Y(), -yAxis.X());
        return RotationMatrix2<T>(xAxis, yAxis);
    }

    template<typename T>
    constexpr const UnitVector2<T>& RotationMatrix2<T>::GetRight() const noexcept
    {
        return mRight;
    }

    template<typename T>
    constexpr const UnitVector2<T>& RotationMatrix2<T>::GetUp() const noexcept
    {
        return mUp;
    }

    template<typename T>
    constexpr const T& RotationMatrix2<T>::operator[](const unsigned int index) const noexcept
    {
        static_assert(sizeof(RotationMatrix2<T>) == sizeof(T) * 2 * 2);
        return reinterpret_cast<const T*>(this)[index];
    }

    template<typename T>
    [[nodiscard]] constexpr Vector2<T> operator*(const Vector2<T>& vector, const RotationMatrix2<T>& matrix) noexcept
    {
        const UnitVector2<T>& right = matrix.GetRight();
        const UnitVector2<T>& up = matrix.GetUp();
        return Vector2<T>
            (
                vector.x * right.X() + vector.y * up.X(),
                vector.x * right.Y() + vector.y * up.Y()
            );
    }

    template<typename T>
    [[nodiscard]] constexpr UnitVector2<T> operator*(const UnitVector2<T>& vector, const RotationMatrix2<T>& matrix) noexcept
    {
        const UnitVector2<T>& right = matrix.GetRight();
        const UnitVector2<T>& up = matrix.GetUp();
        return UnitVector2<T>
            (
                vector.X() * right.X() + vector.Y() * up.X(),
                vector.X() * right.Y() + vector.Y() * up.Y()
            );
    }

    template<typename T>
    [[nodiscard]] constexpr bool operator==(const RotationMatrix2<T>& left, const RotationMatrix2<T>& right) noexcept
    {
        return left.GetRight() == right.GetRight() && left.GetUp() == right.GetUp();
    }
}