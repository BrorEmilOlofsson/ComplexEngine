#include "Node/FlyNodeTypeManager.hpp"
#include <array>

namespace FLY_NAMESPACE
{

	NodeTypeManager::NodeTypeManager()
	{
	}

	NodeTypeManager::~NodeTypeManager()
	{

	}

	NodeTypeID NodeTypeManager::Register(NodeType aNodeType)
	{
		NodeTypeID id{ mNodeTypes.size() };
		mNodeTypes.emplace_back(std::move(aNodeType));
		Assert();

		return id;
	}

	void NodeTypeManager::SetGetterNodeTypeID(const DataTypeID aDataTypeID, const NodeTypeID nodeTypeID)
	{
		mGetterNodeTypeIDs.emplace(aDataTypeID, nodeTypeID);
	}

	void NodeTypeManager::SetSetterNodeTypeID(const DataTypeID aDataTypeID, const NodeTypeID nodeTypeID)
	{
		mSetterNodeTypeIDs.emplace(aDataTypeID, nodeTypeID);
	}

	void NodeTypeManager::SetOperatorNodeTypeID(const DataTypeID aDataTypeID, const eNodeOperatorType operatorType, const NodeTypeID nodeTypeID)
	{
		mTemplateNodeTypeIDMap[operatorType].emplace(aDataTypeID, nodeTypeID);
	}

	void NodeTypeManager::MapNodeTypeIDToTrait(TraitID aTraitID, DataTypeID aDataTypeID, NodeTypeID nodeTypeID)
	{
		mTraitToNodeTypeIDMap[aTraitID][aDataTypeID] = nodeTypeID;
	}

	Node NodeTypeManager::CreateGetterNode(NodeGraph& nodeGraph, const NodeID nodeID, const DataTypeID aDataTypeID, const NodeCreationContext& creationContext)
	{
		const NodeTypeID typeID = mGetterNodeTypeIDs.at(aDataTypeID);
		return CreateNode(nodeGraph, nodeID, typeID, creationContext);
	}

	Node NodeTypeManager::CreateSetterNode(NodeGraph& nodeGraph, const NodeID nodeID, const DataTypeID dataTypeID, const NodeCreationContext& creationContext)
	{
		const NodeTypeID typeID = mSetterNodeTypeIDs.at(dataTypeID);
		return CreateNode(nodeGraph, nodeID, typeID, creationContext);
	}

	Node NodeTypeManager::CreateOperatorNode(NodeGraph& nodeGraph, const NodeID nodeID, const eNodeOperatorType operatorType, const DataTypeID dataTypeID, const NodeCreationContext& creationContext)
	{
		const std::unordered_map<DataTypeID, NodeTypeID>& operatorNodes = mTemplateNodeTypeIDMap.at(operatorType);
		const NodeTypeID typeID = operatorNodes.at(dataTypeID);
		return CreateNode(nodeGraph, nodeID, typeID, creationContext);
	}

	Node NodeTypeManager::CreateTraitNode(NodeGraph& nodeGraph, const NodeID nodeID, const TraitID traitID, const DataTypeID dataTypeID, const NodeCreationContext& creationContext)
	{
		const std::unordered_map<DataTypeID, NodeTypeID>& traitNodes = mTraitToNodeTypeIDMap.at(traitID);
		const NodeTypeID typeID = traitNodes.at(dataTypeID);
        return CreateNode(nodeGraph, nodeID, typeID, creationContext);
	}

	Node NodeTypeManager::CreateNode(NodeGraph& nodeGraph, const NodeID nodeID, const NodeTypeID nodeTypeID, const NodeCreationContext& creationContext)
	{
		return GetNodeType(nodeTypeID).GetCreateFunction().Invoke(nodeID, nodeTypeID, nodeGraph, creationContext);
	}

	bool NodeTypeManager::CanCreateOperatorNode(const eNodeOperatorType operatorType, const DataTypeID aDataTypeID)
	{
		/*const DataTypeID* dataTypeID = std::get_if<DataTypeID>(&aDataTypeID.mID);
		if (!dataTypeID)
		{
			return false;
		}*/
		if (mTemplateNodeTypeIDMap.contains(operatorType))
		{
			return mTemplateNodeTypeIDMap.at(operatorType).contains(aDataTypeID);
		}
		return false;
	}

	bool NodeTypeManager::CanCreateNodeByTrait(const TraitID traitID, const DataTypeID dataTypeID)
	{
		auto it = mTraitToNodeTypeIDMap.find(traitID);
		if (it == end(mTraitToNodeTypeIDMap))
		{
			return false;
		}
		return it->second.contains(dataTypeID);
	}

	const std::vector<NodeType>& NodeTypeManager::GetNodeTypes()
	{
		return mNodeTypes;
	}

	Function& NodeTypeManager::GetFunction(const FunctionID id)
	{
		return *mFunctions.at(id);
	}

	const Function& NodeTypeManager::GetFunction(const FunctionID id) const
	{
		return *mFunctions.at(id);
	}

	const std::vector<HeapObject<Function>>& NodeTypeManager::GetFunctions()
	{
		return mFunctions;
	}

	FunctionID NodeTypeManager::GetFunctionID(const NodeTypeID nodeTypeID) const
	{
		auto it = mToFunctionID.find(nodeTypeID);
		if (it != mToFunctionID.end())
		{
			return it->second;
		}
		return InvalidID<FunctionID>();
	}

	NodeTypeID NodeTypeManager::GetTypeID(std::string_view name)
	{
		for (NodeTypeID id{ 0 }; id < mNodeTypes.size(); ++id)
		{
			if (GetShortName(id) == name)
			{
				return id;
			}
		}
		return NodeTypeID{};
	}

	const std::string& NodeTypeManager::GetFullName(const NodeTypeID id) const
	{
		return GetNodeType(id).GetName();
	}

	std::string NodeTypeManager::GetShortName(const NodeTypeID id) const
	{
		const std::string& fullName = GetFullName(id);
		if (fullName.find_last_of('/') != std::string::npos)
		{
			return fullName.substr(fullName.find_last_of('/') + 1, fullName.length());

		}
		else
		{
			return fullName;
		}
	}

	std::string NodeTypeManager::GetNameDirectory(const NodeTypeID id) const
	{
		const std::string& fullName = GetFullName(id);
		if (fullName.find_last_of('/') != std::string::npos)
		{
			return fullName.substr(0, fullName.find_last_of('/') + 1);

		}
		else
		{
			return fullName;
		}
	}

	FunctionID NodeTypeManager::CreateFunction(std::string name)
	{
		const FunctionID id{ mFunctions.size() };
		const HeapObject<Function>& function = mFunctions.emplace_back(HeapObject<Function>(std::move(name)));

		mToFunctionID.emplace(function->GetCallerNodeTypeID(), id);
		mToFunctionID.emplace(function->GetInputNodeTypeID(), id);
		mToFunctionID.emplace(function->GetOutputNodeTypeID(), id);

		return id;
	}

	const std::unordered_map<DataTypeID, NodeTypeID>& NodeTypeManager::GetTemplateMapByOperator(const eNodeOperatorType operatorType) const
	{
		return mTemplateNodeTypeIDMap.at(operatorType);
	}

	const std::unordered_map<DataTypeID, NodeTypeID>& NodeTypeManager::GetMapByTrait(const TraitID traitID) const
	{
		return mTraitToNodeTypeIDMap.at(traitID);
	}


	NodeTypeID NodeTypeManager::GetNodeTypeIDByTraitAndDataType(const TraitID traitID, const DataTypeID dataTypeID) const
	{
		return mTraitToNodeTypeIDMap.at(traitID).at(dataTypeID);
	}

	NodeType NodeTypeManager::CreateInvalidNodeType()
	{
		CreateNodeFunction createNodeFunction{ [](const NodeID, const NodeTypeID, NodeGraph&, const NodeCreationContext&) -> Node { return Node(NodeTypeID{ 0 }, std::array<PinID, 0>(), std::array<PinID, 0>(), nullptr); } };
		ExecuteNodeFunction executeNodeFunction{ [](const NodeExecutionData&, InternalExecutionContext&) {} };
		eNodeTrait nodeTrait = eNodeTrait::Invalid | eNodeTrait::CPPGenerated;
		std::string name = "Invalid Node";

		NodeType nodeType(name, createNodeFunction, executeNodeFunction, nullptr, nodeTrait, 
			InvalidID<EventID>(), InvalidID<DataTypeID>(), {}, {}, InvalidID<MemoryPoolID>(), 
			InvalidID<DataTypeID>(), InvalidID<TraitID>(), eNodeOperatorType::None);

		return nodeType;
	}


	void NodeTypeManager::Assert()
	{
		/*std::unordered_set<std::string> shortNames;
		for (NodeTypeID id = 0; id < myNodeTypes.size(); ++id)
		{
			std::string shortName = GetShortName(id);

			if (!shortNames.insert(shortName).second)
			{
				throw std::runtime_error("Cannot have nodes with same name" + shortName);
			}
		};*/
	}
}