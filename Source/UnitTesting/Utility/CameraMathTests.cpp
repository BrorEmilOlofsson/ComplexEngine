#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Utility/CameraMath.hpp"
#include "Engine/Utility/Win/WinAdapterFunctions.hpp"
//#include <Windows.h>
#include <DirectXMath.h>

using namespace CLX;

Point2f GetScreenPositionDX(const Point3f& position, const Camera& camera, const Vector2ui& windowSize)
{
	// Assume you have a world position
	DirectX::XMFLOAT3 worldPos = ToXMFloat3(position);

	// Matrices
	DirectX::XMMATRIX view = ToXMMATRIX(camera.GetViewMatrix());
	DirectX::XMMATRIX proj = ToXMMATRIX(camera.GetProjectionMatrix());

	DirectX::XMVECTOR pos3D = XMLoadFloat3(&worldPos);
	DirectX::XMVECTOR projected = XMVector3TransformCoord(pos3D, view * proj);

	// Convert to screen space ([-1,1] to [0, width] and [0, height])
	DirectX::XMFLOAT2 screenPos{};
	XMStoreFloat3(&worldPos, projected);  // Get X, Y, Z

	screenPos.x = (worldPos.x + 1.0f) * 0.5f * windowSize.x;
	screenPos.y = (1.0f - worldPos.y) * 0.5f * windowSize.y; // Y is flipped

	return ToPoint(screenPos);
}

TEST_CASE("GetScreenPosition", "[CameraMath]")
{
	{
		const Camera camera;
		constexpr Point3f worldPosition(0.0f, 0.0f, 10.0f);
		constexpr Dimension2u windowSize(1600, 900);
		const std::optional<Point2f> screenPosition = GetScreenPosition(worldPosition, camera, windowSize);

		REQUIRE(screenPosition == Point2f(800, 450));
	}

	{
		Camera camera;
		camera.GetTransform().RotateOnAxis(UnitVector3f::Right(), Degreesf(32.f));
		camera.GetTransform().RotateOnAxis(UnitVector3f::Up(), Degreesf(21.45f));
		auto viewMatrixXM = ToXMMATRIX(camera.GetViewMatrix());
		auto projectionMatrixXM = ToXMMATRIX(camera.GetProjectionMatrix());
		auto clipMatrixXM = viewMatrixXM * projectionMatrixXM;
		auto viewMatrix = camera.GetViewMatrix();
		auto& projectionMatrix = camera.GetProjectionMatrix();
		auto clipMatrix = viewMatrix * projectionMatrix;
		REQUIRE(ToMatrix4x4f(clipMatrixXM) == clipMatrix);
	}
}