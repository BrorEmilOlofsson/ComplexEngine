#include "Engine/Precompiled/EnginePch.hpp"
#include "DebugShapeSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/ECSEngine/Components/CylinderComponent.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/Math/ShapeMath.hpp"

namespace CLX
{

    void DebugShapeSystem::Render(const ECS& ecs, const Blackboard& blackboard)
    {
        RenderState& renderState = blackboard.Get<Key_CurrentRenderState>();
        RenderList& renderList = renderState.GetRenderList();
        ecs.ForEach([&renderList](const CylinderComponent& cylinderComponent, const TransformComponent& transformComponent)
            {

                Transform localTransfom;
                localTransfom.SetPosition(cylinderComponent.cylinder.GetCenter());
                localTransfom.SetRotation(RotationMatrix3f::FromY(cylinderComponent.cylinder.GetAxis()));

                const Transform worldTransform = localTransfom.ToWorld(transformComponent.transform);

                Cylinderf cylinder = cylinderComponent.cylinder;
                cylinder.SetAxis(worldTransform.GetUpVector());
                cylinder.SetCenter(worldTransform.GetPosition());

                renderList.AddCylinder(cylinder, cylinderComponent.color);
                
                const auto aabb = GetBounds(cylinder);
                renderList.AddBoundingBox(aabb, Colors::LightGreen, true);
            });
    }
}