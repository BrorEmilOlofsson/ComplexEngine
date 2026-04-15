#pragma once
#include <cstdint>

#include "Engine/Math/Vector2.hpp"

namespace CLX
{

    template<typename T>
    class Dimension2 final
    {
    public:

        using value_type = T;

        constexpr Dimension2() noexcept = default;

        constexpr Dimension2(T width, T height) noexcept
            : mWidth(width)
            , mHeight(height)
        {
        }

        [[nodiscard]] constexpr T GetWidth() const noexcept
        {
            return mWidth;
        }
        [[nodiscard]] constexpr T GetHeight() const noexcept
        {
            return mHeight;
        }

        [[nodiscard]] static constexpr Dimension2<T> Zero() noexcept;

    private:

        T mWidth{};
        T mHeight{};
    };

    using Dimension2f = Dimension2<float>;
    using Dimension2d = Dimension2<double>;
    using Dimension2i32 = Dimension2<int32_t>;
    using Dimension2u32 = Dimension2<uint32_t>;
    using Dimension2i = Dimension2i32;
    using Dimension2u = Dimension2u32;

    template<typename T>
    constexpr Dimension2<T> Dimension2<T>::Zero() noexcept
    {
        return Dimension2<T>(T{ 0 }, T{ 0 });
    }

    template<typename T>
    [[nodiscard]] constexpr Vector2<T> ToVector2(Dimension2<T> dimension) noexcept
    {
        return Vector2<T>(dimension.GetWidth(), dimension.GetHeight());
    }

    template<typename T>
    [[nodiscard]] constexpr Dimension2<T> ToDimension2(Vector2<T> vector) noexcept
    {
        return Dimension2<T>(vector.x, vector.y);
    }

    template<typename T>
    [[nodiscard]] constexpr bool operator==(const Dimension2<T>& lhs, const Dimension2<T>& rhs) noexcept
    {
        return lhs.GetWidth() == rhs.GetWidth() && lhs.GetHeight() == rhs.GetHeight();
    }
}