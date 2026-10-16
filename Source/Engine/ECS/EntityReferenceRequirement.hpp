#pragma once
#include "Engine/Utility/Predicate/Predicate.hpp"
#include "Engine/ECS/ECS.hpp"

namespace CLX
{

    using EntityReferenceRequirement = Predicate<EntityID, const ECS&>;
    
    template<typename T>
    [[nodiscard]] inline EntityReferenceRequirement CreateHasComponentRequirement()
    {
        return EntityReferenceRequirement([](EntityID entityID, const ECS& ecs)
            {
                return ecs.HasComponent<T>(entityID);
            });
    }

    [[nodiscard]] inline EntityReferenceRequirement CreateHasComponentRequirement(const std::type_index componentType)
    {
        return EntityReferenceRequirement([componentType](EntityID entityID, const ECS& ecs)
            {
                return ecs.HasComponent(entityID, componentType);
            });
    }
}