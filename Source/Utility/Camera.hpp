#pragma once
#include "Utility/Math/Transform3.hpp"
#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Angle.hpp"

namespace Simple
{
	enum class eCameraType
	{
		Perspective,
		Orthographic
	};

	class Camera final
	{
	public:

		constexpr Camera();
		constexpr Camera(const Radiansf horizontalFoVRad, const float nearPlane, const float farPlane);

		constexpr void UpdateProjection(const Vector2ui& resolution);
		constexpr void UpdateProjection(const float aspectRatio);

	public:

		constexpr void SetOrtographicProjection(const float halfSize, const float nearPlane, const float farPlane);

		constexpr void SetCameraType(const eCameraType cameraType, const Vector2ui& resolution);

		constexpr void SetPosition(const Point3f& position);
		constexpr void SetRotation(const Rotatorf& rotationInDegree);
		constexpr void SetRotation(const Matrix4x4f& rotationMatrix);
		constexpr void SetNearPlane(const float nearPlane, const Vector2ui& resolution);
		constexpr void SetFarPlane(const float farPlane, const Vector2ui& resolution);
		constexpr void SetHorizontalFoV(const Radiansf horizontalFoVRad, const Vector2ui& resolution);
		constexpr void SetResolution(const Vector2ui& resolution);

	public:

		[[nodiscard]] constexpr Matrix4x4f GetWorldToClipMatrix() const noexcept;
		[[nodiscard]] constexpr const Matrix4x4f& GetProjectionMatrix() const noexcept;
		[[nodiscard]] constexpr Matrix4x4f GetViewMatrix() const noexcept;

		[[nodiscard]] constexpr Transform& GetTransform() noexcept;
		[[nodiscard]] constexpr const Transform& GetTransform() const noexcept;

		[[nodiscard]] constexpr Point3f GetPosition() const noexcept;
		[[nodiscard]] constexpr Rotatorf GetRotation() const noexcept;
		[[nodiscard]] constexpr UnitVector3f GetForward() const noexcept;
		[[nodiscard]] constexpr UnitVector3f GetUp() const noexcept;
		[[nodiscard]] constexpr UnitVector3f GetRight() const noexcept;

		[[nodiscard]] constexpr float GetNearPlane() const noexcept;
		[[nodiscard]] constexpr float GetFarPlane() const noexcept;
		[[nodiscard]] constexpr Radiansf GetHorizontalFoV() const noexcept;

		[[nodiscard]] constexpr static Matrix4x4f CreatePerspectiveProjectionMatrix(const Radiansf horizontalFoV, const float nearPlane, const float farPlane, const float aspectRatio);
		[[nodiscard]] constexpr static Matrix4x4f CreatePerspectiveProjectionMatrix(const Radiansf horizontalFoV, const float nearPlane, const float farPlane, const Vector2ui resolution);
		[[nodiscard]] constexpr static Matrix4x4f CreateOrthographicProjectionMatrix(const float halfSize, const float nearPlane, const float farPlane);

	private:

		Radiansf mHorizontalFoVRad = ToRadians(Degreesf(90.f));
		float mNearPlane = 0.1f;
		float mFarPlane = 30000.f;

		eCameraType mCameraType = eCameraType::Perspective;
		Transform mTransform;
		Matrix4x4f mProjectionMatrix;

		static constexpr float DefaultAspectRatio = 16.f / 9.f;
	};

	constexpr Camera::Camera()
		: mProjectionMatrix(CreatePerspectiveProjectionMatrix(mHorizontalFoVRad, mNearPlane, mFarPlane, DefaultAspectRatio))
		, mCameraType(eCameraType::Perspective)
	{
	}

	constexpr Camera::Camera(const Radiansf horizontalFoVRad, const float nearPlane, const float farPlane)
		: mHorizontalFoVRad(horizontalFoVRad)
		, mFarPlane(farPlane)
		, mNearPlane(nearPlane)
		, mProjectionMatrix(CreatePerspectiveProjectionMatrix(mHorizontalFoVRad, mNearPlane, mFarPlane, DefaultAspectRatio))
		, mCameraType(eCameraType::Perspective)
	{
	}

	constexpr Matrix4x4f Camera::GetWorldToClipMatrix() const noexcept
	{
		return mProjectionMatrix * Matrix4x4f::GetFastInverse(mTransform.GetMatrix());
	}

	constexpr const Matrix4x4f& Camera::GetProjectionMatrix() const noexcept
	{
		return mProjectionMatrix;
	}

	constexpr Matrix4x4f Camera::GetViewMatrix() const noexcept
	{
		return Matrix4x4f::GetInverse(mTransform.GetMatrix());
	}

	constexpr Transform& Camera::GetTransform() noexcept
	{
		return mTransform;
	}

	constexpr const Transform& Camera::GetTransform() const noexcept
	{
		return mTransform;
	}

	constexpr Point3f Camera::GetPosition() const noexcept
	{
		return mTransform.GetPosition();
	}

	constexpr Rotatorf Camera::GetRotation() const noexcept
	{
		return mTransform.GetRotation();
	}

	constexpr float Camera::GetNearPlane() const noexcept
	{
		return mNearPlane;
	}

	constexpr float Camera::GetFarPlane() const noexcept
	{
		return mFarPlane;
	}

	constexpr Radiansf Camera::GetHorizontalFoV() const noexcept
	{
		return mHorizontalFoVRad;
	}

	constexpr UnitVector3f Camera::GetForward() const noexcept
	{
		return mTransform.GetMatrix().GetForward();
	}

	constexpr UnitVector3f Camera::GetUp() const noexcept
	{
		return mTransform.GetMatrix().GetUp();
	}

	constexpr UnitVector3f Camera::GetRight() const noexcept
	{
		return mTransform.GetMatrix().GetRight();
	}

	constexpr Matrix4x4f Camera::CreatePerspectiveProjectionMatrix(const Radiansf horizontalFoV, const float nearPlane, const float farPlane, const float aspectRatio)
	{
		const float Q = farPlane / (farPlane - nearPlane);
		const Radiansf verticalFoVRad = ATan(Tan(horizontalFoV / 2.f) * (1.f / aspectRatio)) * 2.f;
		const float scaleX = 1.0f / Tan(horizontalFoV / 2.0f);
		const float scaleY = 1.0f / Tan(verticalFoVRad * 0.5f);

		Matrix4x4f matrix;
		// One based indexing
		matrix(1, 1) = scaleX;
		matrix(2, 2) = scaleY;
		matrix(3, 3) = Q;
		matrix(3, 4) = 1.0f / Q;
		matrix(4, 3) = -Q * nearPlane;
		matrix(4, 4) = 0.0f;

		return matrix;
	}

	constexpr Matrix4x4f Camera::CreatePerspectiveProjectionMatrix(const Radiansf horizontalFoV, const float nearPlane, const float farPlane, const Vector2ui resolution)
	{
		return CreatePerspectiveProjectionMatrix(horizontalFoV, nearPlane, farPlane, static_cast<float>(resolution.x) / static_cast<float>(resolution.y));
	}

	constexpr Matrix4x4f Camera::CreateOrthographicProjectionMatrix(const float halfSize, const float nearPlane, const float farPlane)
	{
		const float left = -halfSize;
		const float right = halfSize;
		const float top = -halfSize;
		const float bottom = halfSize;

		Matrix4x4f matrix = Matrix4x4f::Identity();

		matrix(1, 1) = 2.0f / (right - left);
		matrix(2, 2) = 2.0f / (bottom - top);
		matrix(3, 3) = 1.0f / (farPlane - nearPlane);

		matrix(4, 1) = -(right + left) / (right - left);
		matrix(4, 2) = -(bottom + top) / (bottom - top);
		matrix(4, 3) = -(nearPlane) / (farPlane - nearPlane);

		return matrix;
	}

	constexpr void Camera::SetOrtographicProjection(const float halfSize, const float nearPlane, const float farPlane)
	{
		mProjectionMatrix = CreateOrthographicProjectionMatrix(halfSize, nearPlane, farPlane);
	}

	constexpr void Camera::SetPosition(const Point3f& position)
	{
		mTransform.SetPosition(position);
	}

	constexpr void Camera::SetRotation(const Rotatorf& rotation)
	{
		mTransform.SetRotation(rotation);
	}

	constexpr void Camera::SetRotation(const Matrix4x4f& matrix)
	{
		mTransform.SetRotation(matrix);
	}

	constexpr void Camera::UpdateProjection(const Vector2ui& resolution)
	{
		switch (mCameraType)
		{
		case eCameraType::Perspective:
			UpdateProjection(static_cast<float>(resolution.x) / static_cast<float>(resolution.y));
			break;
		case eCameraType::Orthographic:
			break;
		}
	}

	constexpr void Camera::UpdateProjection(const float aspectRatio)
	{
		mProjectionMatrix = CreatePerspectiveProjectionMatrix(mHorizontalFoVRad, mNearPlane, mFarPlane, aspectRatio);
	}

	constexpr void Camera::SetCameraType(const eCameraType cameraType, const Vector2ui& resolution)
	{
		mCameraType = cameraType;
		UpdateProjection(resolution);
	}

	constexpr void Camera::SetNearPlane(const float nearPlane, const Vector2ui& resolution)
	{
		mNearPlane = nearPlane;

		if (mCameraType == eCameraType::Perspective)
		{
			UpdateProjection(resolution);
		}
	}

	constexpr void Camera::SetFarPlane(const float farPlane, const Vector2ui& resolution)
	{
		mFarPlane = farPlane;

		if (mCameraType == eCameraType::Perspective)
		{
			UpdateProjection(resolution);
		}
	}

	constexpr void Camera::SetHorizontalFoV(const Radiansf horizontalFoVRad, const Vector2ui& resolution)
	{
		mHorizontalFoVRad = horizontalFoVRad;

		if (mCameraType == eCameraType::Perspective)
		{
			UpdateProjection(resolution);
		}
	}

	constexpr void Camera::SetResolution(const Vector2ui& resolution)
	{
		if (mCameraType == eCameraType::Perspective)
		{
			UpdateProjection(resolution);
		}
	}
}