#pragma once
#include <iostream>
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/UnitVector3.hpp"
#include "Engine/Math/Point3.hpp"
#include "Engine/Math/Rotator.hpp"
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/RotationMath.hpp"

namespace CLX
{
	class Transform3 final
	{
	public:

		constexpr Transform3() = default;

		[[nodiscard]] static constexpr Transform3 FromPosition(const Point3f& position);
        [[nodiscard]] static constexpr Transform3 FromPositionRotationScale(const Point3f& position, const RotationMatrix3f& rotation, const Vector3f& scale);
        [[nodiscard]] static constexpr Transform3 FromMatrix(const Matrix4x4f& matrix);

	public:

		constexpr void SetPosition(const Point3f& position);
		constexpr void SetRotation(const Rotatorf& rotation);
		constexpr void SetRotation(const RotationMatrix3f& rotationMatrix);
		constexpr void SetScale(const Vector3f& scale);
		constexpr void SetMatrix(const Matrix4x4f& matrix);
		constexpr void MovePosition(const Vector3f& change);
		constexpr void RotateOnAxis(const UnitVector3f& axis, Radiansf angle);
		constexpr void RotateOnAxis(const UnitVector3f& axis, Degreesf angle);

	public:

		[[nodiscard]] constexpr const Matrix4x4f& GetMatrix() const;
		[[nodiscard]] constexpr Point3f GetPosition() const;
		[[nodiscard]] constexpr Rotatorf GetRotation() const;
		[[nodiscard]] constexpr Vector3f GetScale() const;
		[[nodiscard]] constexpr UnitVector3f GetRightVector() const;
		[[nodiscard]] constexpr UnitVector3f GetUpVector() const;
		[[nodiscard]] constexpr UnitVector3f GetForwardVector() const;
		[[nodiscard]] constexpr Transform3 ToWorld(const Transform3& parent) const;

	private:

		constexpr explicit Transform3(const Matrix4x4f& matrix);

	private:

		Matrix4x4f mMatrix;
	};

	using Transform = Transform3;

	constexpr Transform3::Transform3(const Matrix4x4f& matrix)
		: mMatrix(matrix)
	{
	}

	constexpr Transform3 Transform3::FromPosition(const Point3f& position)
	{
		Transform3 transform;
		transform.SetPosition(position);
        return transform;
	}

	constexpr Transform3 Transform3::FromPositionRotationScale(const Point3f& position, const RotationMatrix3f& rotation, const Vector3f& scale)
	{
		Transform3 transform;
		transform.SetScale(scale);
		transform.SetRotation(rotation);
		transform.SetPosition(position);
        return transform;
	}

	constexpr Transform3 Transform3::FromMatrix(const Matrix4x4f& matrix)
	{
		return Transform3(matrix);
    }

	constexpr void Transform3::SetPosition(const Point3f& position)
	{
		mMatrix.SetTranslation(position);
	}

	constexpr void Transform3::SetRotation(const Rotatorf& rotation)
	{
		const Vector3f scale = mMatrix.GetScale();
		const Point3f position = mMatrix.GetTranslation();
		const Rotatorf rotationInDegrees = GetNormalized360(rotation);
		Matrix4x4f matrix = CreateRotationMatrix(rotationInDegrees);

		mMatrix = Matrix4x4f::CreateScaleMatrix(scale) * matrix;
		mMatrix.SetTranslation(position);
	}

	constexpr void Transform3::SetRotation(const RotationMatrix3f& rotationMatrix)
	{
		const Vector3f scale = mMatrix.GetScale();
		const Point3f position = mMatrix.GetTranslation();
		mMatrix = Matrix4x4f::CreateRotationMatrix(rotationMatrix) * Matrix4x4f::CreateScaleMatrix(scale) * Matrix4x4f::CreateTranslationMatrix(position);
	}

	constexpr void Transform3::SetScale(const Vector3f& scale)
	{
		mMatrix.SetScale(scale);
	}

	constexpr void Transform3::SetMatrix(const Matrix4x4f& matrix)
	{
		mMatrix = matrix;
	}

	constexpr void Transform3::MovePosition(const Vector3f& change)
	{
		SetPosition(mMatrix.GetTranslation() + change);
	}

	constexpr void Transform3::RotateOnAxis(const UnitVector3f& axis, Radiansf angle)
	{
		RotateMatrixAroundAxis(mMatrix, axis, angle);
	}

	constexpr void Transform3::RotateOnAxis(const UnitVector3f& axis, Degreesf angle)
	{
		RotateMatrixAroundAxis(mMatrix, axis, angle);
	}

	constexpr const Matrix4x4f& Transform3::GetMatrix() const
	{
		return mMatrix;
	}

	constexpr Point3f Transform3::GetPosition() const
	{
		return mMatrix.GetTranslation();
	}

	constexpr Rotatorf Transform3::GetRotation() const
	{
		return ToRotator(mMatrix);
	}

	constexpr Vector3f Transform3::GetScale() const
	{
		return mMatrix.GetScale();
	}

	constexpr UnitVector3f Transform3::GetRightVector() const
	{
		return GetRotatorRight(GetRotation());
	}

	constexpr UnitVector3f Transform3::GetUpVector() const
	{
		return GetRotatorUp(GetRotation());
	}

	constexpr UnitVector3f Transform3::GetForwardVector() const
	{
		return GetRotatorForward(GetRotation());
	}

	constexpr Transform3 Transform3::ToWorld(const Transform3& parent) const
	{
		return Transform3(ToWorldSpace(mMatrix, parent.mMatrix));
	}

	inline std::ostream& operator<<(std::ostream& os, const Transform3& transform)
	{
		os << "Position: " << transform.GetPosition() << ", Rotation: " << transform.GetRotation() << ", Scale: " << transform.GetScale();
		return os;
	}
}