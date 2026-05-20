#include "Engine/Precompiled/EnginePch.hpp"
#include "DebugShapeSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/ECSEngine/Components/CylinderComponent.hpp"
#include "Engine/Math/ShapeMath.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"
#include "Engine/Math/RotationMath.hpp"

namespace CLX
{

    void DebugShapeSystem::Render(const ECS& ecs, const Blackboard& blackboard)
    {
        RenderState& renderState = blackboard.Get<Key_CurrentRenderState>();
        RenderList& renderList = renderState.GetRenderList();
        ecs.ForEach([&renderList, &ecs](const EntityID entityID, const CylinderComponent& cylinderComponent)
            {

                Transform localTransform;
                localTransform.SetPosition(cylinderComponent.cylinder.GetCenter());
                localTransform.SetRotation(RotationMatrix3f::FromY(cylinderComponent.cylinder.GetAxis()));

                const Transform worldTransform = ToWorldSpace(localTransform, GetEntityWorldTransform(ecs, entityID));

                Cylinderf cylinder = cylinderComponent.cylinder;
                cylinder.SetAxis(worldTransform.GetUpVector());
                cylinder.SetCenter(worldTransform.GetPosition());

                renderList.AddCylinder(cylinder, cylinderComponent.color);
                
                const auto aabb = GetBounds(cylinder);
                renderList.AddBoundingBox(aabb, Colors::LightGreen, true);
            });
    }
}