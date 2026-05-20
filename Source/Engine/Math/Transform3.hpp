#pragma once
#include <format>
#include <concepts>
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/UnitVector3.hpp"
#include "Engine/Math/Point3.hpp"
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/RotationMatrix3.hpp"
#include "Engine/Math/Angle.hpp"

namespace CLX
{

	template<std::floating_point T>
	class Transform3 final
	{
	public:

		constexpr Transform3() = default;

		[[nodiscard]] static constexpr Transform3 FromPosition(const Point3<T>& position);
		[[nodiscard]] static constexpr Transform3 FromPositionRotationScale(const Point3<T>& position, const RotationMatrix3<T>& rotation, const Vector3<T>& scale);
        [[nodiscard]] static constexpr Transform3 FromMatrix(const Matrix4x4<T>& matrix);

	public:

		constexpr void SetPosition(const Point3<T>& position);
		constexpr void SetRotation(const RotationMatrix3<T>& rotationMatrix);
		constexpr void SetScale(const Vector3<T>& scale);
		constexpr void SetMatrix(const Matrix4x4<T>& matrix);
		constexpr void MovePosition(const Vector3<T>& change);

	public:

		[[nodiscard]] constexpr const Matrix4x4<T>& GetMatrix() const;
		[[nodiscard]] constexpr Point3<T> GetPosition() const;
		[[nodiscard]] constexpr RotationMatrix3<T> GetRotation() const;
		[[nodiscard]] constexpr Vector3<T> GetScale() const;
		[[nodiscard]] constexpr UnitVector3<T> GetRightVector() const;
		[[nodiscard]] constexpr UnitVector3<T> GetUpVector() const;
		[[nodiscard]] constexpr UnitVector3<T> GetForwardVector() const;

	private:

		constexpr explicit Transform3(const Matrix4x4<T>& matrix);

	private:

		Matrix4x4<T> mMatrix;
	};

    using Transform3f = Transform3<float>;
    using Transform3d = Transform3<double>;
	using Transform = Transform3f;

	template<std::floating_point T>
	constexpr Transform3<T>::Transform3(const Matrix4x4<T>& matrix)
		: mMatrix(matrix)
	{
	}

	template<std::floating_point T>
	constexpr Transform3<T> Transform3<T>::FromPosition(const Point3<T>& position)
	{
		Transform3<T> transform;
		transform.SetPosition(position);
        return transform;
	}

	template<std::floating_point T>
	constexpr Transform3<T> Transform3<T>::FromPositionRotationScale(const Point3<T>& position, const RotationMatrix3<T>& rotation, const Vector3<T>& scale)
	{
		Transform3<T> transform;
		transform.SetScale(scale);
		transform.SetRotation(rotation);
		transform.SetPosition(position);
        return transform;
	}

	template<std::floating_point T>
	constexpr Transform3<T> Transform3<T>::FromMatrix(const Matrix4x4<T>& matrix)
	{
		return Transform3<T>(matrix);
    }

	template<std::floating_point T>
	constexpr void Transform3<T>::SetPosition(const Point3<T>& position)
	{
		mMatrix.SetTranslation(position);
	}

	template<std::floating_point T>
	constexpr void Transform3<T>::SetRotation(const RotationMatrix3<T>& rotationMatrix)
	{
		const Vector3<T> scale = mMatrix.GetScale();
		const Point3<T> position = mMatrix.GetTranslation();
		mMatrix = Matrix4x4<T>::CreateRotationMatrix(rotationMatrix) * Matrix4x4<T>::CreateScaleMatrix(scale) * Matrix4x4<T>::CreateTranslationMatrix(position);
	}

	template<std::floating_point T>
	constexpr void Transform3<T>::SetScale(const Vector3<T>& scale)
	{
		mMatrix.SetScale(scale);
	}

	template<std::floating_point T>
	constexpr void Transform3<T>::SetMatrix(const Matrix4x4<T>& matrix)
	{
		mMatrix = matrix;
	}

	template<std::floating_point T>
	constexpr void Transform3<T>::MovePosition(const Vector3<T>& change)
	{
		SetPosition(mMatrix.GetTranslation() + change);
	}

	template<std::floating_point T>
	constexpr const Matrix4x4<T>& Transform3<T>::GetMatrix() const
	{
		return mMatrix;
	}

	template<std::floating_point T>
	constexpr Point3<T> Transform3<T>::GetPosition() const
	{
		return mMatrix.GetTranslation();
	}

	template<std::floating_point T>
	constexpr RotationMatrix3<T> Transform3<T>::GetRotation() const
	{
		return mMatrix.GetRotationMatrix();
	}

	template<std::floating_point T>
	constexpr Vector3<T> Transform3<T>::GetScale() const
	{
		return mMatrix.GetScale();
	}

	template<std::floating_point T>
	constexpr UnitVector3<T> Transform3<T>::GetRightVector() const
	{
		return mMatrix.GetRight();
	}

	template<std::floating_point T>
	constexpr UnitVector3<T> Transform3<T>::GetUpVector() const
	{
		return mMatrix.GetUp();
	}

	template<std::floating_point T>
	constexpr UnitVector3<T> Transform3<T>::GetForwardVector() const
	{
		return mMatrix.GetForward();
	}

	template<std::floating_point T>
    [[nodiscard]] constexpr bool operator==(const Transform3<T>& a, const Transform3<T>& b) noexcept
    {
        return a.GetMatrix() == b.GetMatrix();
    }
}

template<std::floating_point T>
struct std::formatter<CLX::Transform3<T>, char> : std::formatter<std::string, char>
{
    template<typename FormatContext>
    [[nodiscard]] auto format(const CLX::Transform3<T>& transform, FormatContext& context) const
    {
        const CLX::Point3<T> position = transform.GetPosition();
        const CLX::Rotator<T> rotation = transform.GetRotation();
        return std::format_to(context.out(), "Transform3(Position: {}, Rotation: {})", position, rotation);
    }
};