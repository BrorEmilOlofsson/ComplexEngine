#pragma once
#include "Engine/Math/Transform3.hpp"
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/UnitVector3.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Angle.hpp"

namespace CLX
{
	enum class eCameraType
	{
		Perspective,
		Orthographic
	};

	constexpr Radiansf CalculateHorizontalFOV(Radiansf verticalFOV, float aspectRatio)
	{
		return 2.0f * ATan(Tan(verticalFOV * 0.5f) * aspectRatio);
	}

	constexpr Radiansf CalculateVerticalFOV(Radiansf horizontalFOV, float aspectRatio)
	{
		return 2.0f * ATan(Tan(horizontalFOV / 2.0f) / aspectRatio);
	}


	class Camera final
	{
	public:

		constexpr Camera();
		
		constexpr void UpdateProjection(const float aspectRatio);

		constexpr void SetOrthographicProjection(const float halfSize, const float nearPlane, const float farPlane);

		constexpr void SetCameraType(const eCameraType cameraType);

		constexpr void SetPosition(const Point3f& position);
		constexpr void SetRotation(const Rotatorf& rotationInDegree);
		constexpr void SetRotation(const RotationMatrix3f& rotationMatrix);
        constexpr void SetTransform(const Transform& transform);
		constexpr void SetNearPlane(const float nearPlane);
		constexpr void SetFarPlane(const float farPlane);
		constexpr void SetHorizontalFOV(const Radiansf horizontalFoVRad);
		constexpr void SetAspectRatio(const float aspectRatio);
        constexpr void SetResolution(const Vector2ui resolution);

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
		[[nodiscard]] constexpr Radiansf GetHorizontalFOV() const noexcept;
		[[nodiscard]] constexpr Radiansf GetVerticalFOV() const noexcept;
		[[nodiscard]] constexpr float GetAspectRatio() const noexcept;

		[[nodiscard]] constexpr static Matrix4x4f CreatePerspectiveProjectionMatrix(const Radiansf verticalFOV, const float nearPlane, const float farPlane, const float aspectRatio);
		[[nodiscard]] constexpr static Matrix4x4f CreateOrthographicProjectionMatrix(const float halfSize, const float nearPlane, const float farPlane);

	private:

		Radiansf mVerticalFOV = DefaultVerticalFoV;
		float mAspectRatio = DefaultAspectRatio;
		float mNearPlane = 0.1f;
		float mFarPlane = 30000.f;

		eCameraType mCameraType = eCameraType::Perspective;
		Transform mTransform;
		Matrix4x4f mProjectionMatrix;

		static constexpr float DefaultAspectRatio = 16.f / 9.f;
		static constexpr Radiansf DefaultVerticalFoV = ToRadians(Degreesf(58.7f));
	};

	constexpr Camera::Camera()
		: mProjectionMatrix(CreatePerspectiveProjectionMatrix(mVerticalFOV, mNearPlane, mFarPlane, DefaultAspectRatio))
		, mCameraType(eCameraType::Perspective)
		, mAspectRatio(DefaultAspectRatio)
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

	constexpr Radiansf Camera::GetHorizontalFOV() const noexcept
	{
		return CalculateHorizontalFOV(mVerticalFOV, mAspectRatio);
	}

	constexpr Radiansf Camera::GetVerticalFOV() const noexcept
	{
		return mVerticalFOV;
	}

	constexpr float Camera::GetAspectRatio() const noexcept
	{
		return mAspectRatio;
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

	constexpr Matrix4x4f Camera::CreatePerspectiveProjectionMatrix(const Radiansf verticalFOV, const float nearPlane, const float farPlane, const float aspectRatio)
	{
		const float Q = farPlane / (farPlane - nearPlane);

		float scaleY = 1.0f / Tan(verticalFOV * 0.5f);
		float scaleX = scaleY / aspectRatio;


		Matrix4x4f matrix;

		matrix(0, 0) = scaleX;
		matrix(1, 1) = scaleY;
		matrix(2, 2) = Q;
		matrix(2, 3) = 1.0f / Q;
		matrix(3, 2) = -Q * nearPlane;
		matrix(3, 3) = 0.0f;

		return matrix;
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

	constexpr void Camera::SetOrthographicProjection(const float halfSize, const float nearPlane, const float farPlane)
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

	constexpr void Camera::SetRotation(const RotationMatrix3f& rotationMatrix)
	{
		mTransform.SetRotation(rotationMatrix);
	}

	constexpr void Camera::SetTransform(const Transform& transform)
	{
		mTransform = transform;
    }

	constexpr void Camera::UpdateProjection(const float aspectRatio)
	{
		mAspectRatio = aspectRatio;
		switch (mCameraType)
		{
		case eCameraType::Perspective:
			mProjectionMatrix = CreatePerspectiveProjectionMatrix(mVerticalFOV, mNearPlane, mFarPlane, aspectRatio);
			break;
		case eCameraType::Orthographic:
			break;
		}
	}

	constexpr void Camera::SetCameraType(const eCameraType cameraType)
	{
		mCameraType = cameraType;
		UpdateProjection(mAspectRatio);
	}

	constexpr void Camera::SetNearPlane(const float nearPlane)
	{
		mNearPlane = nearPlane;

		if (mCameraType == eCameraType::Perspective)
		{
			UpdateProjection(mAspectRatio);
		}
	}

	constexpr void Camera::SetFarPlane(const float farPlane)
	{
		mFarPlane = farPlane;

		if (mCameraType == eCameraType::Perspective)
		{
			UpdateProjection(mAspectRatio);
		}
	}

	constexpr void Camera::SetHorizontalFOV(const Radiansf horizontalFoVRad)
	{
		mVerticalFOV = CalculateVerticalFOV(horizontalFoVRad, mAspectRatio);

		if (mCameraType == eCameraType::Perspective)
		{
			UpdateProjection(mAspectRatio);
		}
	}

	constexpr void Camera::SetAspectRatio(const float aspectRatio)
	{
		if (aspectRatio == 0)
		{
            ASSERT(false && "Aspect ratio cannot be zero");
		}
        mAspectRatio = aspectRatio;
		if (mCameraType == eCameraType::Perspective)
		{
			UpdateProjection(mAspectRatio);
		}
	}

	constexpr void Camera::SetResolution(const Vector2ui resolution)
	{
		SetAspectRatio(ToAspectRatio(resolution));
    }
}