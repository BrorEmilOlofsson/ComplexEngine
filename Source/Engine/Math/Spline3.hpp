#pragma once
#include <vector>
#include <tuple>
#include <utility>
#include "Engine/Math/Point3.hpp"

namespace CLX
{


    struct SplineReparamSample
    {
        float distance = 0.f;
        float inputKey = 0.f;
    };

    template<typename T>
    class Spline3 final
    {
    public:

        template<typename T>
        class Spline3Iterator
        {
        public:

            using value_type = std::tuple<Point3<T>&, Point3<T>&, Point3<T>&, Point3<T>&>;

            constexpr Spline3Iterator(Spline3<T>* spline, std::size_t index = 0) : mSpline(spline), mIndex(index) {}

            constexpr Spline3Iterator<T> operator++()
            {
                ++mIndex;
                return *this;
            }

            [[nodiscard]] constexpr value_type operator*()
            {
                return value_type(
                    mSpline->GetPointAt(mIndex),
                    mSpline->GetPointAt(mIndex + 1),
                    mSpline->GetPointAt(mIndex + 2),
                    mSpline->GetPointAt(mIndex + 3)
                );
            }

            constexpr bool operator==(const Spline3Iterator<T>& other) const
            {
                return mIndex == other.mIndex && mSpline == other.mSpline;
            }

            constexpr bool operator!=(const Spline3Iterator<T>& other) const
            {
                return !(*this == other);
            }

        private:

            std::size_t mIndex = 0;
            Spline3<T>* mSpline = nullptr;
        };
    public:

        constexpr Spline3() = default;

        constexpr void AddPoint(const Point3<T>& point)
        {
            mPoints.push_back(point);
        }

        [[nodiscard]] std::size_t GetNumSegments() const noexcept
        {
            if (mPoints.size() < 4)
            {
                return 0;
            }
            return (mPoints.size() - 1) / 3;
        }

        [[nodiscard]] constexpr const std::vector<Point3<T>>& GetPoints() const
        {
            return mPoints;
        }

        [[nodiscard]] constexpr Point3<T>& GetPointAt(std::size_t index)
        {
            return mPoints[index];
        }

        constexpr Spline3Iterator<T> begin()
        {
            return Spline3Iterator<T>(this);
        }

        constexpr Spline3Iterator<T> end()
        {
            return Spline3Iterator<T>(this, GetNumSegments());
        }

    private:

        std::vector<Point3<T>> mPoints;
        std::vector<SplineReparamSample> mReparamSamples;
    };

    using Spline3f = Spline3<float>;
    using Spline3d = Spline3<double>;


    [[nodiscard]] std::optional<float> FindSplineInputKey(const std::vector<SplineReparamSample>& samples, const float distance)
    {
        if (samples.empty())
        {
            return std::nullopt;
        }

        if (distance <= samples.front().distance)
        {
            return samples.front().inputKey;
        }

        if (distance >= samples.back().distance)
        {
            return samples.back().inputKey;
        }
        
        for (std::size_t i = 0; i < samples.size() - 1; ++i)
        {
            const SplineReparamSample& s0 = samples[i];
            const SplineReparamSample& s1 = samples[i + 1];
            if (IsInRangeInclusive(distance, s0.distance, s1.distance))
            {
                const float localT = Remap0To1(distance, s0.distance, s1.distance);
                return Lerp(s0.inputKey, s1.inputKey, localT);
            }
        }

        return std::nullopt;
    }
}