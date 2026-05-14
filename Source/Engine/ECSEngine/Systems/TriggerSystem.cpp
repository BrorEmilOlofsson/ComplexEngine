#include "Engine/Precompiled/EnginePch.hpp"
#include "TriggerSystem.hpp"
#include "Engine/ECSEngine/Components/TriggerComponent.hpp"
#include "Engine/Utility/CollisionFunctions.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"
#include "Engine/Math/ShapeMath.hpp"

namespace CLX
{

    struct ShapeWorldTransformer
    {
        const Transform& entityWorldTransform;

        Shape operator()(const Spheref& sphere)
        {
            Spheref transformedSphere = sphere;
            const Transform worldTransform = ToTransform(sphere).ToWorld(entityWorldTransform);
            transformedSphere.SetCenter(worldTransform.GetPosition());
            transformedSphere.SetRadius(sphere.GetRadius() * MaxComponent(worldTransform.GetScale()));
            return transformedSphere;
        }

        Shape operator()(const AABB3f& aabb)
        {
            AABB3f transformedAABB = aabb;
            const Transform worldTransform = ToTransform(aabb).ToWorld(entityWorldTransform);
            transformedAABB.SetCenter(worldTransform.GetPosition());
            transformedAABB.SetExtent(aabb.GetExtent() * worldTransform.GetScale());
            return transformedAABB;
        }
    };

    Transform GetShapeWorldTransform(const ECS& ecs, EntityID entityID, const Shape& shape)
    {
        const Transform shapeTransform = std::visit([](const auto& s) { return ToTransform(s); }, shape);
        return shapeTransform.ToWorld(GetEntityWorldTransform(ecs, entityID));
    }

    void UpdateTriggerStates(std::span<const TriggerData> previousFrameTriggers, std::vector<TriggerData>& newTriggers)
    {
        for (TriggerData& triggerData : newTriggers)
        {
            const auto it = std::ranges::find_if(previousFrameTriggers, [&triggerData](const TriggerData& previousTrigger)
                {
                    return previousTrigger.entityID == triggerData.entityID;
                });
            
            if (it != end(previousFrameTriggers))
            {
                triggerData.state = eTriggerState::Stay;
            }
            else
            {
                triggerData.state = eTriggerState::Enter;
            }
        }

        for (const TriggerData& previousTrigger : previousFrameTriggers)
        {
            const auto it = std::ranges::find_if(newTriggers, [&previousTrigger](const TriggerData& triggerData)
                {
                    return triggerData.entityID == previousTrigger.entityID;
                });
            if (it == end(newTriggers))
            {
                newTriggers.push_back({ previousTrigger.entityID, eTriggerState::Exit });
            }
        }
    }

    void CheckTriggerCollisions(ECS& ecs)
    {
        ecs.ForEach([&ecs](const EntityID entityID, TriggerComponent& trigger)
            {
                const auto& shape = trigger.shape;

                const Transform entityWorldTransform = GetEntityWorldTransform(ecs, entityID);

                const auto transformedShape = std::visit(ShapeWorldTransformer{ entityWorldTransform }, shape);

                std::vector<TriggerData> triggers;

                ecs.ForEach([&ecs, entityID, &transformedShape, &triggers](const EntityID otherEntity, const TriggerComponent& otherTrigger)
                    {
                        if (entityID == otherEntity)
                        {
                            return;
                        }
                        const auto& otherShape = otherTrigger.shape;
                        const Transform otherEntityWorldTransform = GetEntityWorldTransform(ecs, otherEntity);
                        const auto transformedOtherShape = std::visit(ShapeWorldTransformer{ otherEntityWorldTransform }, otherShape);
                        if (std::visit([&transformedShape, &transformedOtherShape](const auto& s, const auto& o)
                            {
                                return DetectCollision(s, o);
                            }, transformedShape, transformedOtherShape))
                        {
                            triggers.push_back({ otherEntity });
                        }
                    });

                UpdateTriggerStates(trigger.triggers, triggers);

                if (trigger.triggers.empty() && !triggers.empty())
                {
                    // Trigger entered
                    trigger.savedDebugColor = trigger.debugColor;
                    trigger.debugColor = Colors::Red;
                }
                else if (!trigger.triggers.empty() && triggers.empty())
                {
                    // Trigger exited
                    trigger.debugColor = trigger.savedDebugColor;
                }

                trigger.triggers = std::move(triggers);
            });
    }

    void RenderTriggers(const ECS& ecs, RenderList& renderList)
    {

        ecs.ForEach([&ecs, &renderList](const EntityID entityID, const TriggerComponent& trigger)
            {
                Transform worldTransform = GetEntityWorldTransform(ecs, entityID);
                const auto transformedShape = std::visit(ShapeWorldTransformer{ worldTransform }, trigger.shape);
                std::visit([&renderList, &trigger](const auto& s)
                    {
                        using ShapeType = std::decay_t<decltype(s)>;
                        if constexpr (std::same_as<ShapeType, Spheref>)
                        {
                            renderList.AddSphere(DrawSphere
                                {
                                    .sphere = s,
                                    .color = Color(trigger.debugColor)
                                }, true);
                        }
                        else if constexpr (std::same_as<ShapeType, AABB3f>)
                        {
                            renderList.AddBoundingBox(DrawBoundingBox
                                {
                                    .boundingBox = s,
                                    .color = trigger.debugColor
                                }, true);
                        }
                    }, transformedShape);
            });
    }

    void TriggerSystem::Update(ECS& ecs, const float, const Blackboard&)
    {
        CheckTriggerCollisions(ecs);
    }

    void TriggerSystem::Render(const ECS& ecs, const Blackboard& blackboard)
    {
        RenderTriggers(ecs, blackboard.Get<Key_CurrentRenderState>().GetRenderList());
    }

    void TriggerSystem::EditorUpdate(ECS& ecs, const Blackboard&)
    {
        CheckTriggerCollisions(ecs);
    }
}