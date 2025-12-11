#pragma once
#include <vector>
#include "Utility/Math/Point2.hpp"

namespace Simple
{

    template<typename T>
    class Curve2 final
    {
    public:

        constexpr Curve2() = default;

        [[nodiscard]] const std::vector<Point2<T>>& GetPoints() const noexcept;

        void AddAnchorPoint(const Point2<T>& point) noexcept;

        void Clear();

        void SetPoints(const std::vector<Point2<T>>& points) noexcept
        {
            mPoints = points;
        }

    private:

        std::vector<Point2<T>> mPoints;
    };

    using Curve2f = Curve2<float>;
    using Curve2d = Curve2<double>;

    template<typename T>
    [[nodiscard]] const std::vector<Point2<T>>& Curve2<T>::GetPoints() const noexcept
    {
        return mPoints;
    }

    template<typename T>
    void Curve2<T>::AddAnchorPoint(const Point2<T>& point) noexcept
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
    void Curve2<T>::Clear()
    {
        mPoints.clear();
    }
}