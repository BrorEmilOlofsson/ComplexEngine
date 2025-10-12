#pragma once
#include "Engine/Collision/CollisionShape.hpp"
#include "Engine/Collision/CollisionLayer.hpp"
#include "Utility/Color.hpp"
#include "Engine/ECS/EntityID.hpp"
#include <variant>
#include <functional>
#include <vector>

#undef max

namespace Simple
{
	using CollisionCallback = std::function<void(EntityID entityID)>;

	struct ColliderComponent final
	{
		CollisionShape myShape;

		CollisionCallback myOnEnterCallback;
		CollisionCallback myOnOverlapCallback;
		CollisionCallback myOnExitCallback;

		std::vector<EntityID> myPreviousFrameCollisions;
		std::vector<EntityID> myCurrentFrameCollisions;

		EntityID myEntityID;

		Color myColor;

		eCollisionLayer myOwnCollisionLayer = eCollisionLayer::None;
		eCollisionLayer myCollidesWithLayer = eCollisionLayer::None;
	};

	void RegisterColliderComponent();

	class ColliderComponentFunctions
	{
	public:
		static bool DoesCollideWith(const ColliderComponent& aColliderComponent1, const CollisionShape& aShape);
		static bool DoesCollideWith(const ColliderComponent& aColliderComponent1, const ColliderComponent& aColliderComponent2);
		static bool DoesCollideWith(const CollisionShape& aShape1, const CollisionShape& aShape2);

		template<typename ShapeType>
		decltype(auto) GetShape(ColliderComponent& aColliderComponent)
		{
			return std::get<ShapeType>(aColliderComponent.myShape);
		}

		static void SetOnEnterCallback(ColliderComponent& aColliderComponent, CollisionCallback aCallback)
		{
			aColliderComponent.myOnEnterCallback = std::move(aCallback);
		}

		static void SetOnOverlapCallback(ColliderComponent& aColliderComponent, CollisionCallback aCallback)
		{
			aColliderComponent.myOnOverlapCallback = std::move(aCallback);
		}

		static void SetOnExitCallback(ColliderComponent& aColliderComponent, CollisionCallback aCallback)
		{
			aColliderComponent.myOnExitCallback = std::move(aCallback);
		}

		static void SetColor(ColliderComponent& aColliderComponent, const Simple::Color& aColor)
		{
			aColliderComponent.myColor = aColor;
		}

		static void Render(const ColliderComponent& aColliderComponent, class RenderList& renderList);

		static void UpdateFrameCollisions(ColliderComponent& aColliderComponent)
		{
			aColliderComponent.myPreviousFrameCollisions = aColliderComponent.myCurrentFrameCollisions;
			aColliderComponent.myCurrentFrameCollisions.clear();
		}

		static bool DidCollidePreviousFrame(ColliderComponent& aColliderComponent, const EntityID entityID)
		{
			return std::find(begin(aColliderComponent.myPreviousFrameCollisions), end(aColliderComponent.myPreviousFrameCollisions), entityID) != end(aColliderComponent.myPreviousFrameCollisions);
		}

		static bool DidCollideCurrentFrame(const ColliderComponent& aColliderComponent, const EntityID entityID)
		{
			return std::find(begin(aColliderComponent.myCurrentFrameCollisions), end(aColliderComponent.myCurrentFrameCollisions), entityID) != end(aColliderComponent.myCurrentFrameCollisions);
		}
	};

}