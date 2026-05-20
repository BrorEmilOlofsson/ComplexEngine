#pragma once
#include <ranges>
#include <concepts>
#include "Grid2Utility.hpp"

namespace CLX
{

    template<std::integral T>
    class GridIndexRowBasedIterator final
    {
    public:

        using value_type = T;
        using difference_type = std::ptrdiff_t;

        constexpr GridIndexRowBasedIterator() = default;

        constexpr GridIndexRowBasedIterator(T index)
            : mIndex(index)
        {}

        [[nodiscard]] constexpr T operator*() const
        {
            return mIndex;
        }

        constexpr GridIndexRowBasedIterator& operator++()
        {
            ++mIndex;
            return *this;
        }

        constexpr GridIndexRowBasedIterator operator++(int)
        {
            GridIndexRowBasedIterator temp = *this;
            ++(*this);
            return temp;
        }

        [[nodiscard]] constexpr bool operator==(const GridIndexRowBasedIterator& other) const
        {
            return mIndex == other.mIndex;
        }

    private:

        T mIndex = 0;
    };

    static_assert(std::input_iterator<GridIndexRowBasedIterator<std::size_t>>, "GridIndexRowBasedIterator must satisfy the input iterator concept.");

    template<std::integral T, std::unsigned_integral U = std::size_t>
    class GridIndexRowBasedView final : public std::ranges::view_interface<GridIndexRowBasedView<T, U>>
    {
    public:

        constexpr GridIndexRowBasedView() = default;
        constexpr GridIndexRowBasedView(U width, U height)
            : mWidth(width)
            , mHeight(height)
        {}

        constexpr GridIndexRowBasedIterator<T> begin() const
        {
            return GridIndexRowBasedIterator<T>(0);
        }

        constexpr GridIndexRowBasedIterator<T> end() const
        {
            return GridIndexRowBasedIterator<T>(mWidth * mHeight);
        }

    private:

        U mWidth = 0;
        U mHeight = 0;
    };

    template<std::unsigned_integral U>
    GridIndexRowBasedView(U, U) -> GridIndexRowBasedView<std::size_t, U>;

    static_assert(std::ranges::range<GridIndexRowBasedView<std::size_t>>, "GridIndexRowBasedView must satisfy the range concept.");
    static_assert(std::ranges::view<GridIndexRowBasedView<std::size_t>>, "GridIndexRowBasedView must satisfy the view concept.");
    
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

        [[nodiscard]] constexpr bool operator==(const GridIndexColumnBasedIterator& other) const
        {
            return mRow == other.mRow && mColumn == other.mColumn && mWidth == other.mWidth && mHeight == other.mHeight;
        }

    private:

        U mWidth = 0;
        U mHeight = 0;
        U mRow = 0;
        U mColumn = 0;
    };

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

        constexpr GridIndexColumnBasedIterator<T, U> end() const
        {
            return GridIndexColumnBasedIterator<T, U>(mWidth, mHeight, 0, mWidth);
        }

    private:

        U mWidth = 0;
        U mHeight = 0;
    };


    static_assert(std::ranges::range<GridIndexColumnBasedView<std::size_t>>, "GridIndexColumnBasedView must satisfy the range concept.");
    static_assert(std::ranges::view<GridIndexColumnBasedView<std::size_t>>, "GridIndexColumnBasedView must satisfy the view concept.");

}