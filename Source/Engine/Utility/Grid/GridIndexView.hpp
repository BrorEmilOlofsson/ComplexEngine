#pragma once
#include <ranges>
#include <concepts>
#include "Grid2Utility.hpp"

namespace CLX
{
    struct GridIndexRowBasedViewGenerator
    {
        template<std::integral U>
        constexpr auto operator()(U width, U height) const
        {
            const auto totalSize = static_cast<U>(width * height);
            return std::views::iota(static_cast<U>(0), totalSize);
        }

        template<std::integral U>
        constexpr auto operator()(U size) const
        {
            return std::views::iota(static_cast<U>(0), size);
        }
    };

    inline constexpr GridIndexRowBasedViewGenerator GridIndexRowBasedView;

    struct GridIndexRowBasedOffsetViewGenerator
    {
        template<typename T, typename U, typename V, typename W>
        constexpr auto operator()(T width, U height, V startRow, W startCol) const
        {
            using A = std::conditional_t<std::is_signed<V>::value || std::is_signed<W>::value, std::make_signed_t<std::common_type_t<T, U, V, W>>, std::common_type_t<T, U, V, W>>;
            const auto totalSize = width * height;
            return std::views::iota(static_cast<T>(0), totalSize)
                | std::views::transform([=](auto index)
                    {
                        using Ret = std::pair<A, A>;
                        const auto row = GetRow(index, width);
                        const auto col = GetColumn(index, width);
                        return Ret{ row + startRow, col + startCol };
                    });
        }
    };

    inline constexpr GridIndexRowBasedOffsetViewGenerator GridIndexRowBasedOffsetView;

    template<std::integral T, std::unsigned_integral U = std::size_t>
    class GridIndexColumnBasedIterator final
    {
    public:

        using value_type = T;
        using difference_type = std::ptrdiff_t;

        constexpr GridIndexColumnBasedIterator() = default;

        constexpr GridIndexColumnBasedIterator(U width, U height, U row, U column)
            : mWidth(width)
            , mHeight(height)
            , mRow(row)
            , mColumn(column)
        {}

        [[nodiscard]] constexpr T operator*() const
        {
            return static_cast<T>(GetIndex(mRow, mColumn, mWidth));
        }

        constexpr GridIndexColumnBasedIterator& operator++()
        {
            mRow++;
            if (mRow >= mHeight)
            {
                mColumn++;
                mRow = 0;
            }
            return *this;
        }

        constexpr GridIndexColumnBasedIterator operator++(int)
        {
            GridIndexColumnBasedIterator temp = *this;
            ++(*this);
            return temp;
        }

        [[nodiscard]] constexpr auto GetWidth() const { return mWidth; }
        [[nodiscard]] constexpr auto GetHeight() const { return mHeight; }
        [[nodiscard]] constexpr auto GetRow() const { return mRow; }
        [[nodiscard]] constexpr auto GetColumn() const { return mColumn; }

    private:

        U mWidth = 0;
        U mHeight = 0;
        U mRow = 0;
        U mColumn = 0;
    };

    struct GridIndexColumnBasedViewSentinel
    {
    };

    template<typename T, typename U>
    [[nodiscard]] constexpr bool operator==(const GridIndexColumnBasedIterator<T, U>& other, const GridIndexColumnBasedViewSentinel&) 
    {
        return other.GetColumn() == other.GetWidth();
    }


    static_assert(std::input_iterator<GridIndexColumnBasedIterator<std::size_t>>, "GridIndexColumnBasedIterator must satisfy the input iterator concept.");


    template<typename T, typename U = std::size_t>
    class GridIndexColumnBasedView final : public std::ranges::view_interface<GridIndexColumnBasedView<T, U>>
    {
    public:

        constexpr GridIndexColumnBasedView() = default;
        constexpr GridIndexColumnBasedView(U width, U height)
            : mWidth(width)
            , mHeight(height)
        {}

        constexpr GridIndexColumnBasedIterator<T, U> begin() const
        {
            return GridIndexColumnBasedIterator<T, U>(mWidth, mHeight, 0, 0);
        }

        constexpr GridIndexColumnBasedViewSentinel end() const
        {
            return GridIndexColumnBasedViewSentinel{};
        }

    private:

        U mWidth = 0;
        U mHeight = 0;
    };


    static_assert(std::ranges::range<GridIndexColumnBasedView<std::size_t>>, "GridIndexColumnBasedView must satisfy the range concept.");
    static_assert(std::ranges::view<GridIndexColumnBasedView<std::size_t>>, "GridIndexColumnBasedView must satisfy the view concept.");

}