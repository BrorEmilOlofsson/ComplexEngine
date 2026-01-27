#pragma once
#include <string>

#include "../FlyDefines.hpp"
#include "../Graph/FlyNodeGraph.hpp"
#include "../DataType/FlyVariableContainer.hpp"

namespace FLY_NAMESPACE
{

	class Function final
	{
	public:

		explicit Function(std::string name);

		[[nodiscard]] const std::string& GetName() const;
		[[nodiscard]] constexpr NodeTypeID GetCallerNodeTypeID() const;
		[[nodiscard]] constexpr NodeTypeID GetInputNodeTypeID() const;
		[[nodiscard]] constexpr NodeTypeID GetOutputNodeTypeID() const;
		[[nodiscard]] constexpr NodeGraph& GetNodeGraph();
		[[nodiscard]] constexpr const NodeGraph& GetNodeGraph() const;
		[[nodiscard]] constexpr VariableContainer& GetVariableContainer();
		[[nodiscard]] constexpr const VariableContainer& GetVariableContainer() const;

		void SetName(std::string name);


		static constexpr NodeID INPUT_NODE_ID = NodeID{ 0 };
		static constexpr NodeID OUTPUT_NODE_ID = NodeID{ 1 };

	private:

		std::string mName;

		NodeTypeID mCallerNodeTypeID = InvalidID<NodeTypeID>();
		NodeTypeID mInputNodeTypeID = InvalidID<NodeTypeID>();
		NodeTypeID mOutputNodeTypeID = InvalidID<NodeTypeID>();

		NodeGraph mNodeGraph;

		VariableContainer mVariableContainer;
	};

	constexpr NodeTypeID Function::GetCallerNodeTypeID() const
	{
		return mCallerNodeTypeID;
	}
	
	constexpr NodeTypeID Function::GetInputNodeTypeID() const
	{
		return mInputNodeTypeID;
	}

	constexpr NodeTypeID Function::GetOutputNodeTypeID() const
	{
		return mOutputNodeTypeID;
	}

	constexpr NodeGraph& Function::GetNodeGraph()
	{
		return mNodeGraph;
	}

	constexpr const NodeGraph& Function::GetNodeGraph() const
	{
		return mNodeGraph;
	}

	constexpr VariableContainer& Function::GetVariableContainer()
	{
		return mVariableContainer;
    }

	constexpr const VariableContainer& Function::GetVariableContainer() const
	{
        return mVariableContainer;
	}

}