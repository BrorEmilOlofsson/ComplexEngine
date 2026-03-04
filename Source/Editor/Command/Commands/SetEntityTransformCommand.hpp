#pragma once
#include "Editor/Utility/EditorUtilities.hpp"
#include "Engine/Math/Transform3.hpp"
#include "Engine/ECS/EntityID.hpp"

namespace CLX
{

	class SetEntityTransformCommand final
	{
	public:

		void Execute() const;
		void Undo() const;

	public:
		EntityID entityID;
		class ECS* ecs = nullptr;
		Transform oldTransform;
		Transform newTransform;
	};

}