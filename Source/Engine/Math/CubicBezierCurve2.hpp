#pragma once
#include <vector>
#include <optional>
#include "Engine/Math/Point2.hpp"

namespace CLX
{

    template<typename T>
    class CubicBezierCurve2 final
    {
    public:

        constexpr CubicBezierCurve2() = default;

        [[nodiscard]] const std::vector<Point2<T>>& GetPoints() const noexcept;
        [[nodiscard]] constexpr std::array<Point2<T>, 4> GetControlPointsAt(std::size_t index) const;
        [[nodiscard]] std::size_t GetNumSegments() const noexcept
        {
            if (mPoints.size() < 4)
            {
                return 0;
            }
            return (mPoints.size() - 1) / 3;
        }

        [[nodiscard]] uint32_t GetPointIndex(uint32_t segmentIndex, uint32_t pointIndex) const noexcept
        {
            return segmentIndex * 3 + pointIndex;
        }

        void AddAnchorPoint(const Point2<T>& point) noexcept;

        void Clear();

        void SetPoints(const std::vector<Point2<T>>& points) noexcept
        {
            mPoints = points;
        }

        void SetPointAt(std::size_t index, const Point2<T>& point) noexcept
        {
            mPoints[index] = point;
        }

        void SetPointAt(std::size_t segmentIndex, uint32_t pointIndex, const Point2<T>& point) noexcept
        {
            SetPointAt(GetPointIndex(segmentIndex, pointIndex), point);
        }

    private:

        std::vector<Point2<T>> mPoints;
    };

    using CubicBezierCurve2f = CubicBezierCurve2<float>;
    using CubicBezierCurve2d = CubicBezierCurve2<double>;

    template<typename T>
    const std::vector<Point2<T>>& CubicBezierCurve2<T>::GetPoints() const noexcept
    {
        return mPoints;
    }

    template<typename T>
    constexpr std::array<Point2<T>, 4> CubicBezierCurve2<T>::GetControlPointsAt(std::size_t index) const
    {
        const std::size_t startIdx = index * 3;
        return std::array<Point2<T>, 4>{ mPoints[startIdx], mPoints[startIdx + 1], mPoints[startIdx + 2], mPoints[startIdx + 3] };
    }

    template<typename T>
    void CubicBezierCurve2<T>::AddAnchorPoint(const Point2<T>& point) noexcept
    {
        if (mPoints.empty())
        {
            mPoints.emplace_back(point);
            return;
        }
        else if (mPoints.size() == 1)
        {
            const Point2<T>& firstPoint = mPoints.front();
            const Point2<T> midPoint1 = Lerp(firstPoint, point, 0.25);
            const Point2<T> midPoint2 = Lerp(firstPoint, point, 0.75);
            mPoints.emplace_back(midPoint1);
            mPoints.emplace_back(midPoint2);
            mPoints.emplace_back(point);
        }
        else
        {
            const Point2<T>& lastPoint = mPoints.back();
            const Point2<T>& secondLastPoint = mPoints[mPoints.size() - 2];
            const Point2<T> midPoint1 = lastPoint + (lastPoint - secondLastPoint);
            const Point2<T> midPoint2 = Lerp(midPoint1, point, 0.5);
            mPoints.emplace_back(midPoint1);
            mPoints.emplace_back(midPoint2);
            mPoints.emplace_back(point);
        }
    }

    template<typename T>
    void CubicBezierCurve2<T>::Clear()
    {
        mPoints.clear();
    }
}