#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Camera/CameraMovement.hpp"

using namespace Simple;

TEST_CASE("FreeFlyCameraUpdate moves camera forward", "[CameraMovement]")
{
	{

		Camera camera;

		const Point3f initialPosition = camera.GetPosition();

		CameraControls controls;
		controls.forwardKey = eInputKey::W;
		FreeFlyCameraUpdate(camera, 0.1f, 5.f, 45.f, true, false, false, false, false, false, {});

		const Point3f actualPosition = camera.GetPosition();
		REQUIRE(actualPosition.x == 0.f);
		REQUIRE(actualPosition.y == 0.f);
		REQUIRE(actualPosition.z > 0.f);

	}

}