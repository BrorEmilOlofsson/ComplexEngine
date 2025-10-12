#include "Engine/Precompiled/EnginePch.hpp"
#include "ColliderComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"
#include "Engine/Utility/DebugShapes.hpp"
#include "Engine/Collision/CollisionFunctions.hpp"

namespace Simple
{

	void RegisterColliderComponent()
	{
	}

	COMPONENT(ColliderComponent);

	REGISTER_DATATYPE(Spheref);

	REGISTER_MEMBER(Spheref::mCenter);
	REGISTER_MEMBER(Spheref::mRadius);

	REGISTER_DATATYPE(AABB3f);

	REGISTER_DATATYPE(Ray3f);

	REGISTER_DATATYPE(CollisionShape);
	REGISTER_MEMBER(ColliderComponent::myShape);
	
	struct ShapeRenderVisitor
	{
		void operator()(const Spheref& aSphere) const
		{
			RenderSphere(aSphere, myColor, myRenderList);
		}

		void operator()(const AABB3f& aAABB) const
		{
			RenderAABB3(aAABB, myColor, myRenderList);
		}

		void operator()(const Ray3f& aRay) const
		{
			RenderRay(aRay, myColor, myRenderList);
		}

		Color myColor;
		RenderList& myRenderList;
	};

	bool ColliderComponentFunctions::DoesCollideWith(const ColliderComponent& aColliderComponent1, const ColliderComponent& aColliderComponent2)
	{
		return DoesCollideWith(aColliderComponent1.myShape, aColliderComponent2.myShape);
	}

	bool ColliderComponentFunctions::DoesCollideWith(const ColliderComponent& aColliderComponent, const CollisionShape& aShape)
	{
		return DoesCollideWith(aColliderComponent.myShape, aShape);
	}

	bool ColliderComponentFunctions::DoesCollideWith(const CollisionShape& aShape1, const CollisionShape& aShape2)
	{
		return std::visit([](const auto& aShape1, const auto& aShape2) -> bool
			{
				return DetectCollision(aShape1, aShape2);
			}, aShape1, aShape2);
	}

	void ColliderComponentFunctions::Render(const ColliderComponent& aColliderComponent, RenderList& renderList)
	{
		std::visit(ShapeRenderVisitor{ aColliderComponent.myColor, renderList }, aColliderComponent.myShape);
	}

}