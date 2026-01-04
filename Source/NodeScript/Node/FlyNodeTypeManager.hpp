#pragma once
#include "../FlyDefines.hpp"
#include "FlyNodeType.hpp"
#include "../CustomEvent/FlyCustomEvent.hpp"
#include "../Function/FlyFunction.hpp"
#include "../Utilities/FlyMeta.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <optional>

namespace FLY_NAMESPACE
{
	class Class;
	class Foundation;

	class NodeTypeManager
	{
	public:

		NodeTypeManager();
		~NodeTypeManager();

		NodeTypeID Register(NodeType nodeType);

		void SetGetterNodeTypeID(DataTypeID dataTypeID, NodeTypeID nodeType);
		void SetSetterNodeTypeID(DataTypeID dataTypeID, NodeTypeID nodeType);

		void SetOperatorNodeTypeID(DataTypeID dataTypeID, eNodeOperatorType operatorType, NodeTypeID nodeTypeID);
		void MapNodeTypeIDToTrait(TraitID traitID, DataTypeID dataTypeID, NodeTypeID nodeTypeID);

		Node CreateGetterNode(NodeGraph& nodeGraph, NodeID nodeID, DataTypeID dataTypeID, const NodeCreationContext& creationContext);
		Node CreateSetterNode(NodeGraph& nodeGraph, NodeID nodeID, DataTypeID dataTypeID, const  NodeCreationContext& creationContext);
		Node CreateTraitNode(NodeGraph& nodeGraph, NodeID nodeID, TraitID aTraitID, DataTypeID dataTypeID, const NodeCreationContext& creationContext);
		Node CreateOperatorNode(NodeGraph& nodeGraph, NodeID nodeID, eNodeOperatorType operatorType, DataTypeID dataTypeID, const NodeCreationContext& creationContext);

		Node CreateNode(NodeGraph& nodeGraph, NodeID nodeID, NodeTypeID nodeTypeID, const NodeCreationContext& creationContext);

		[[nodiscard]] bool CanCreateOperatorNode(eNodeOperatorType operatorType, DataTypeID dataTypeID);
		[[nodiscard]] bool CanCreateNodeByTrait(TraitID traitID, DataTypeID dataTypeID);

		[[nodiscard]] constexpr NodeType& GetNodeType(const NodeTypeID nodeTypeID)
		{
			return mNodeTypes[nodeTypeID];
		}

		[[nodiscard]] constexpr const NodeType& GetNodeType(const NodeTypeID nodeTypeID) const
		{
			return mNodeTypes[nodeTypeID];
		}

		[[nodiscard]] const std::vector<NodeType>& GetNodeTypes();

		[[nodiscard]] CustomEvent& GetCustomEvent(CustomEventID id);
		[[nodiscard]] const CustomEvent& GetCustomEvent(CustomEventID id) const;
		[[nodiscard]] std::vector<CustomEvent>& GetCustomEvents();
		[[nodiscard]] const std::vector<CustomEvent>& GetCustomEvents() const;

		[[nodiscard]] CustomEventID GetCustomEventID(NodeTypeID nodeTypeID) const;

		[[nodiscard]] Function& GetFunction(FunctionID functionID);
		[[nodiscard]] const Function& GetFunction(FunctionID functionID) const;
		[[nodiscard]] const std::vector<HeapObject<Function>>& GetFunctions();
		[[nodiscard]] FunctionID GetFunctionID(NodeTypeID nodeTypeID) const;

		[[nodiscard]] NodeTypeID GetTypeID(std::string_view name);

		[[nodiscard]] const std::string& GetFullName(NodeTypeID nodeTypeID) const;
		[[nodiscard]] std::string GetShortName(NodeTypeID nodeTypeID) const;
		[[nodiscard]] std::string GetNameDirectory(NodeTypeID nodeTypeID) const;

		CustomEventID CreateCustomEvent(std::string name);
		FunctionID CreateFunction(std::string name);

		[[nodiscard]] const std::unordered_map<DataTypeID, NodeTypeID>& GetTemplateMapByOperator(eNodeOperatorType operatorType) const;
		[[nodiscard]] const std::unordered_map<DataTypeID, NodeTypeID>& GetMapByTrait(TraitID traitID) const;
		[[nodiscard]] NodeTypeID GetNodeTypeIDByTraitAndDataType(TraitID traitID, DataTypeID dataTypeID) const;

		void Assert();

	private:

		static NodeType CreateInvalidNodeType();

	private:
		std::vector<NodeType> mNodeTypes;
		std::vector<CustomEvent> mCustomEvents;
		std::vector<HeapObject<Function>> mFunctions;

		std::unordered_multimap<NodeTypeID, CustomEventID> mToCustomEventID;
		std::unordered_multimap<NodeTypeID, FunctionID> mToFunctionID;
		std::unordered_map<DataTypeID, NodeTypeID> mGetterNodeTypeIDs;
		std::unordered_map<DataTypeID, NodeTypeID> mSetterNodeTypeIDs;
		std::unordered_map<eNodeOperatorType, std::unordered_map<DataTypeID, NodeTypeID>> mTemplateNodeTypeIDMap;
		std::unordered_map<TraitID, std::unordered_map<DataTypeID, NodeTypeID>> mTraitToNodeTypeIDMap;
	};
}