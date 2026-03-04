#include <Catch2/catch_amalgamated.hpp>

#include "Engine/Math/Quaternion.hpp"
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/RotationMath.hpp"

using namespace Simple;

TEST_CASE("Quaternion - Default constructor is identity")
{
    Quaternionf q;
    REQUIRE(q.w == 1.0f);
    REQUIRE(q.x == 0.0f);
    REQUIRE(q.y == 0.0f);
    REQUIRE(q.z == 0.0f);
}

TEST_CASE("Quaternion - Identity function is correct")
{
    Quaternionf q = Quaternionf::Identity();
    REQUIRE(q.w == 1.0f);
    REQUIRE(q.x == 0.0f);
    REQUIRE(q.y == 0.0f);
    REQUIRE(q.z == 0.0f);
}

TEST_CASE("Quaternion - Constructor is correct")
{
    Quaternionf q(4.0f, 2.0f, 3.0f, 1.0f);
    REQUIRE(q.w == 4.0f);
    REQUIRE(q.x == 2.0f);
    REQUIRE(q.y == 3.0f);
    REQUIRE(q.z == 1.0f);
}

TEST_CASE("MatrixToQuaternion - Identity")
{
    constexpr Matrix4x4f m = Matrix4x4f::Identity();
    const Quaternionf q = ToQuaternion(m);

    constexpr Quaternionf expected = Quaternionf::Identity();
    REQUIRE(NearlyEqual(q, expected));
}

TEST_CASE("QuaternionToMatrix")
{
    constexpr Quaternionf identityQuat = Quaternionf::Identity();
	const RotationMatrix3f m = ToRotationMatrix(identityQuat);

	constexpr RotationMatrix3f expectedMatrix = RotationMatrix3f::Identity();
	REQUIRE(m == expectedMatrix);
}

//TEST_CASE("MatrixToQuaternion - 90 degrees around X")
//{
//    Matrix4x4f m = Matrix4x4f::CreateRotationAroundX(Degreesf(90));
//    Quaternionf q = ToQuaternion(m);
//
//    Quaternionf expected = Quaternionf::FromAxisAndAngle(UnitVector3f::Right(), Degreesf(90));
//    expected.Normalize();
//
//    REQUIRE(NearlyEqual(q, expected));
//}

//TEST_CASE("MatrixToQuaternion - 90 degrees around Y")
//{
//    Matrix4x4f m = Matrix4x4f::CreateRotationAroundY(Degreesf(90));
//    Quaternionf q = QuaternionFromMatrix(m);
//
//    Quaternionf expected = Quaternionf::FromAxisAngle(Vector3f(0, 1, 0), Degreesf(90));
//    expected.Normalize();
//
//    REQUIRE(AlmostEqualQuat(q, expected));
//}

//TEST_CASE("MatrixToQuaternion - 90 degrees around Z")
//{
//    Matrix4x4f m = Matrix4x4f::CreateRotationAroundZ(Degreesf(90));
//    Quaternionf q = QuaternionFromMatrix(m);
//
//    Quaternionf expected = Quaternionf::FromAxisAngle(Vector3f(0, 0, 1), Degreesf(90));
//    expected.Normalize();
//
//    REQUIRE(AlmostEqualQuat(q, expected));
//}
//
//TEST_CASE("MatrixToQuaternion - 180 degree rotation around X")
//{
//    Matrix4x4f m = Matrix4x4f::CreateRotationAroundX(Degreesf(180));
//    Quaternionf q = QuaternionFromMatrix(m);
//
//    Quaternionf expected = Quaternionf::FromAxisAngle(Vector3f(1, 0, 0), Degreesf(180));
//    expected.Normalize();
//
//    REQUIRE(AlmostEqualQuat(q, expected));
//}
//
//TEST_CASE("MatrixToQuaternion - Combined rotation XYZ")
//{
//    // Row-major, row-vector: M = Rx * Ry * Rz
//    Matrix4x4f m =
//        Matrix4x4f::CreateRotationAroundX(Degreesf(30)) *
//        Matrix4x4f::CreateRotationAroundY(Degreesf(45)) *
//        Matrix4x4f::CreateRotationAroundZ(Degreesf(60));
//
//    // Build quaternion from same operations
//    Quaternionf expected =
//        Quaternionf::FromAxisAngle(Vector3f(1, 0, 0), Degreesf(30)) *
//        Quaternionf::FromAxisAngle(Vector3f(0, 1, 0), Degreesf(45)) *
//        Quaternionf::FromAxisAngle(Vector3f(0, 0, 1), Degreesf(60));
//
//    expected.Normalize();
//
//    Quaternionf q = QuaternionFromMatrix(m);
//    q.Normalize();
//
//    REQUIRE(AlmostEqualQuat(q, expected));
//}
//
//TEST_CASE("MatrixToQuaternion - Resilience to floating point noise")
//{
//    Matrix4x4f m = Matrix4x4f::CreateRotationAroundY(Degreesf(45));
//
//    // introduce small noise
//    m(0, 1) += 1e-6f;
//    m(2, 0) -= 1e-6f;
//
//    Quaternionf q = QuaternionFromMatrix(m);
//
//    Quaternionf expected = Quaternionf::FromAxisAngle(Vector3f(0, 1, 0), Degreesf(45));
//
//    expected.Normalize();
//    REQUIRE(AlmostEqualQuat(q, expected));
//}
//
//TEST_CASE("MatrixToQuaternion - Round trip Matrix -> Quaternion -> Matrix")
//{
//    Matrix4x4f m =
//        Matrix4x4f::CreateRotationAroundX(Degreesf(25)) *
//        Matrix4x4f::CreateRotationAroundY(Degreesf(-40)) *
//        Matrix4x4f::CreateRotationAroundZ(Degreesf(10));
//
//    Quaternionf q = QuaternionFromMatrix(m);
//    Matrix4x4f m2 = q.ToMatrix4x4(); // your implementation
//
//    // compare only rotation part
//    for (int i = 0; i < 3; ++i)
//        for (int j = 0; j < 3; ++j)
//            REQUIRE(std::abs(m(i, j) - m2(i, j)) < 1e-4f);
//}