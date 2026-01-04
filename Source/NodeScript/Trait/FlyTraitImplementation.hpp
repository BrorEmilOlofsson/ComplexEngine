#pragma once
#include "../FlyDefines.hpp"
#include "../Graph/FlyNodeGraph.hpp"
#include "../Graph/FlyEventGraph.hpp"

namespace FLY_NAMESPACE
{

	struct CPPTraitImplementation final
	{
		NodeTypeID mNodeTypeID;
	};

	struct FlyTraitImplementation final
	{
		EventGraph mEventGraph;
		NodeTypeID mInputNodeTypeID;
		NodeTypeID mOutputNodeTypeID;

		static constexpr NodeID _InputNodeID{ 0 };
		static constexpr NodeID _OutputNodeID{ 1 };
	};

	using TraitImplementation = std::variant<CPPTraitImplementation, FlyTraitImplementation>;
}