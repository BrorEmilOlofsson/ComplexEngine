#pragma once
#include "Engine/Math/Matrix3x3.hpp"
#include "Engine/Math/Point2.hpp"
#include "Engine/Math/RotationMatrix2.hpp"
#include "Engine/Math/RotationMath.hpp"

namespace Simple
{
    class Transform2
    {
    public:

        constexpr Transform2() = default;

        [[nodiscard]] static constexpr Transform2 FromPosition(const Point2f& position)
        {
            Transform2 transform;
            transform.SetPosition(position);
            return transform;
        }
        [[nodiscard]] static constexpr Transform2 FromMatrix(const Matrix3x3f& matrix)
        {
            Transform2 transform;
            transform.mMatrix = matrix;
            return transform;
        }

        [[nodiscard]] constexpr Point2f GetPosition() const;
        [[nodiscard]] constexpr RotationMatrix2f GetRotationMatrix() const;

        constexpr void SetPosition(const Point2f& position)
        {
            mMatrix(2, 0) = position.x;
            mMatrix(2, 1) = position.y;
        }


        [[nodiscard]] constexpr Transform2 ToWorld(const Transform2& parent) const
        {
            return Transform2::FromMatrix(ToWorldSpace(mMatrix, parent.mMatrix));
        }

    private:

        Matrix3x3f mMatrix = Matrix3x3f::Identity();
    };

    constexpr Point2f Transform2::GetPosition() const
    {
        return Point2f(mMatrix(2, 0), mMatrix(2, 1));
    }

    constexpr RotationMatrix2f Transform2::GetRotationMatrix() const
    {
        return RotationMatrix2f::FromValues(
            {
                mMatrix(0, 0), mMatrix(0, 1),
                mMatrix(1, 0), mMatrix(1, 1)
            }
        );
    }
}