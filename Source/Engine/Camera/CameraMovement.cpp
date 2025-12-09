#include "Engine/Precompiled/EnginePch.hpp"
#include "CameraMovement.hpp"
#include "Utility/Math/RotationMath.hpp"
#include "Utility/Input/InputState.hpp"

namespace Simple
{

    void FreeFlyCameraUpdate(Camera& camera, const float deltaTime, const float moveSpeed, const float rotationSpeed,
        bool forwardMovement, bool backwardMovement, bool leftMovement, bool rightMovement, bool upMovement, bool invertUp, Vector2f mouseDelta)
    {
        const Point3f currentPosition = camera.GetPosition();

        const UnitVector3f cameraForward = camera.GetForward();
        const UnitVector3f cameraRight = camera.GetRight();
        const UnitVector3f cameraUp = camera.GetUp();

        Vector3f moveDirection;

        if (forwardMovement)
        {
            moveDirection += cameraForward * moveSpeed * deltaTime;
        }

        if (backwardMovement)
        {
            moveDirection += -cameraForward * moveSpeed * deltaTime;
        }

        if (leftMovement)
        {
            moveDirection += -cameraRight * moveSpeed * deltaTime;
        }

        if (rightMovement)
        {
            moveDirection += cameraRight * moveSpeed * deltaTime;
        }

        if (upMovement)
        {
            const float direction = BoolToSign<float>(!invertUp);
            const float upDir = BoolToSign<float>(cameraUp.Y() >= 0);
            moveDirection.y += direction * upDir * moveSpeed * deltaTime;
        }

        const Point3f targetPosition = currentPosition + ToNormalized(moveDirection) * (moveSpeed * deltaTime);
        mouseDelta = Vector2f(mouseDelta) * (rotationSpeed * 0.003f);
        if (mouseDelta == Vector2f::Zero())
        {
            camera.SetPosition(targetPosition);
            return;
        }

        {
            RotationMatrix3f rotationMatrix = camera.GetTransform().GetMatrix().GetRotationMatrix();
            RotateMatrixAroundAxis(rotationMatrix, UnitVector3f::Up(), Degreesf(-mouseDelta.x));
            rotationMatrix = RotationMatrix3f::FromXY(RemoveY(rotationMatrix.GetRight()), rotationMatrix.GetUp());
            RotateMatrixAroundAxis(rotationMatrix, cameraRight, Degreesf(mouseDelta.y));
            camera.GetTransform().SetRotation(rotationMatrix);
            camera.SetPosition(targetPosition);
        }

       /* {
            Matrix4x4f matrix = camera.GetTransform().GetMatrix();

            RotateMatrixAroundAxis(matrix, UnitVector3f::Up(), Degreesf(-mouseDelta.x));
            matrix = CreateMatrixFromXY(RemoveY(matrix.GetRight()), matrix.GetUp());
            RotateMatrixAroundAxis(matrix, cameraRight, Degreesf(mouseDelta.y));
            camera.GetTransform().SetMatrix(matrix);
            camera.SetPosition(targetPosition);
        }*/
    }

    void FreeFlyCameraUpdate(Camera& camera, const float deltaTime, const float moveSpeed, const float rotationSpeed, const InputState& input, const CameraControls& controls)
    {
        const bool forwardMovement = input.IsKeyHeld(controls.forwardKey);
        const bool backwardMovement = input.IsKeyHeld(controls.backKey);
        const bool leftMovement = input.IsKeyHeld(controls.leftKey);
        const bool rightMovement = input.IsKeyHeld(controls.rightKey);
        const bool upMovement = input.IsKeyHeld(controls.upKey);
        const bool invertUp = input.IsKeyHeld(controls.upKeyInverter);
        const Vector2f mouseDelta = Vector2f(input.GetMousePositionDelta());
        FreeFlyCameraUpdate(camera, deltaTime, moveSpeed, rotationSpeed, forwardMovement, backwardMovement, leftMovement, rightMovement, upMovement, invertUp, mouseDelta);
    }
}