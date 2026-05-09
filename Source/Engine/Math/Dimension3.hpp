#pragma once
#include <cstdint>

#include "Engine/Math/Vector3.hpp"

namespace CLX
{

    template<typename T>
    class Dimension3 final
    {
    public:

        using value_type = T;

        constexpr Dimension3() noexcept = default;

        constexpr Dimension3(T width, T height, T depth) noexcept
            : mWidth(width)
            , mHeight(height)
            , mDepth(depth)
        {}

        [[nodiscard]] constexpr T GetWidth() const noexcept
        {
            return mWidth;
        }

        [[nodiscard]] constexpr T GetHeight() const noexcept
        {
            return mHeight;
        }

        [[nodiscard]] constexpr T GetDepth() const noexcept
        {
            return mDepth;
        }

        [[nodiscard]] static constexpr Dimension3<T> Zero() noexcept;

    private:

        T mWidth{};
        T mHeight{};
        T mDepth{};
    };

    using Dimension3f = Dimension3<float>;
    using Dimension3d = Dimension3<double>;
    using Dimension3i32 = Dimension3<int32_t>;
    using Dimension3u32 = Dimension3<uint32_t>;
    using Dimension3i = Dimension3i32;
    using Dimension3u = Dimension3u32;

    template<typename T>
    constexpr Dimension3<T> Dimension3<T>::Zero() noexcept
    {
        return Dimension3<T>(T{ 0 }, T{ 0 });
    }

    template<typename T>
    [[nodiscard]] constexpr Vector3<T> ToVector3(Dimension3 <T> dimension) noexcept
    {
        return Vector3<T>(dimension.GetWidth(), dimension.GetHeight(), dimension.GetDepth());
    }

    template<typename T>
    [[nodiscard]] constexpr Dimension3<T> ToDimension3(Vector3<T> vector) noexcept
    {
        return Dimension3<T>(vector.x, vector.y, vector.z);
    }

    template<typename T>
    [[nodiscard]] constexpr bool operator==(const Dimension3<T>& lhs, const Dimension3<T>& rhs) noexcept
    {
        return lhs.GetWidth() == rhs.GetWidth() && lhs.GetHeight() == rhs.GetHeight() && lhs.GetDepth() == rhs.GetDepth();
    }
}