#include "Game/Precompiled/GamePch.hpp"
#include "DuckMovementSystem.hpp"

#include "Engine/ECS/ECS.hpp"
#include "Game/ECS/Components/DuckComponent.hpp"
#include "Game/ECS/Components/DucklingComponent.hpp"

constexpr CLX::Vector3f MaxLength(const CLX::Vector3f& vector, const float maxLength)
{
    float lengthSquared = CLX::LengthSquared(vector);
    if (lengthSquared > CLX::Square(maxLength))
    {
        return vector * (maxLength / CLX::Sqrt(lengthSquared));
    }
    return vector;
}

//void UpdateMovement(CLX::ECS& ecs, const float deltaTime)
//{
//    std::vector<std::tuple<CLX::EntityID, CLX::Transform, DuckComponent&>> duckEntities;
//    duckEntities.reserve(100);
//    ecs.ForEach([&duckEntities, &ecs](const CLX::EntityID entityID, DuckComponent& duckComponent)
//        {
//            duckEntities.push_back({ entityID, GetWorldTransform(ecs, entityID), duckComponent });
//        });
//
//    for (const auto& [entityID1, transform1, duckComponent] : duckEntities)
//    {
//        CLX::Vector3f cohesionMovement;
//        float cohesionFactor = 1.f;
//        CLX::Vector3f repulsionMovement;
//        for (const auto& [entityID2, transform2, _] : duckEntities)
//        {
//            float distance = CLX::Distance(transform1.GetPosition(), transform2.GetPosition());
//            if (distance <= 5.f)
//            {
//                // Move towards other duck
//                const CLX::Vector3f directionTowardsOtherDuck = transform2.GetPosition() - transform1.GetPosition();
//                cohesionMovement += directionTowardsOtherDuck;
//
//                // Repulsion if too close
//                const CLX::Vector3f repulsionDirection = transform1.GetPosition() - transform2.GetPosition();
//                if (distance <= 1.f)
//                {
//                    repulsionMovement += repulsionDirection * 5.f; // Strong repulsion when very close
//                }
//            }
//        }
//
//        CLX::Normalize(cohesionMovement);
//        CLX::Normalize(repulsionMovement);
//        cohesionMovement *= cohesionFactor;
//
//        const CLX::Vector3f frameMovement = duckComponent.speed * deltaTime * (cohesionMovement + repulsionMovement);
//
//        /* if (Length(frameMovement) < 0.01f)
//         {
//             continue;
//         }*/
//        CLX::Transform newTransform = transform1;
//        newTransform.MovePosition(frameMovement);
//        ecs.GetComponent<CLX::TransformComponent>(entityID1)->transform = CLX::GetLocalTransform(ecs, entityID1, newTransform);
//    }
//}


struct LessDistanceSorter
{
    CLX::Point3f targetPoint;
    CLX::ECS& ecs;

    bool operator()(CLX::EntityID lhs, CLX::EntityID rhs) const
    {
        const float distance1 = CLX::DistanceSquared(GetEntityWorldTransform(ecs, lhs).GetPosition(), targetPoint);
        const float distance2 = CLX::DistanceSquared(GetEntityWorldTransform(ecs, rhs).GetPosition(), targetPoint);
        return distance1 < distance2;
    }
};

static void UpdateFollowMovement(CLX::ECS& ecs, const float deltaTime)
{
    ecs.ForEach([&ecs, deltaTime](CLX::EntityID entityID, DuckComponent& duckComponent, const CLX::TransformHierarchyComponent& transformHierarchy)
        {
            auto& children = transformHierarchy.children;

            if (children.empty())
            {
                return;
            }

            auto ducklingChildren = children | std::views::filter([&ecs](const CLX::EntityID childID)
                {
                    return ecs.HasComponent<DucklingComponent>(childID);
                })
                | std::ranges::to<std::vector>();

            const CLX::Transform parentTransform = GetEntityWorldTransform(ecs, entityID);
            std::ranges::sort(ducklingChildren, LessDistanceSorter{ parentTransform.GetPosition(), ecs});

            auto moveTowardsTarget = [&ecs](const CLX::EntityID ducklingID, const CLX::Point3f& target, const float speed, const float deltaTime)
                {
                    const CLX::Transform ducklingTransform = GetEntityWorldTransform(ecs, ducklingID);
                    const CLX::Point3f currentPosition = ducklingTransform.GetPosition();
                    constexpr float minDistance = 0.5f;
                    if (CLX::IsInRange(currentPosition, target, minDistance))
                    {
                        return;
                    }

                    const CLX::Point3f newPosition = CLX::InterpPoint(currentPosition, target, speed, deltaTime);

                    CLX::Transform newTransform = ducklingTransform;
                    newTransform.SetPosition(newPosition);

                    ecs.GetComponent<CLX::TransformComponent>(ducklingID)->transform = GetEntityLocalTransform(ecs, ducklingID, newTransform);
                };
            {
                const CLX::Point3f target = parentTransform.GetPosition();

                moveTowardsTarget(ducklingChildren.front(), target, duckComponent.speed, deltaTime);
            }

            for (std::size_t i = 1; i < ducklingChildren.size(); i++)
            {
                const CLX::EntityID ducklingID = ducklingChildren[i];
                const CLX::EntityID previousEntityID = ducklingChildren[i - 1];

                const CLX::Transform previousTransform = GetEntityWorldTransform(ecs, previousEntityID);

                const CLX::Point3f target = CLX::Lerp(previousTransform.GetPosition(), parentTransform.GetPosition(), 0.2f);

                moveTowardsTarget(ducklingID, target, duckComponent.speed, deltaTime);
            }
        });
}

void DuckMovementSystem::Update(CLX::ECS& ecs, const float deltaTime, const CLX::Blackboard&)
{
    UpdateFollowMovement(ecs, deltaTime);
}


void DuckMovementSystem::EditorUpdate(CLX::ECS& ecs, const CLX::Blackboard& blackboard)
{
    ecs;
    blackboard;
    //const float deltaTime = blackboard.Get<CLX::Key_DeltaTime>();
    //UpdateFollowMovement(ecs, deltaTime);
}

