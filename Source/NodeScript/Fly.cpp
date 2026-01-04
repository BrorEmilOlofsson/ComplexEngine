#include "Fly.hpp"
#include "FlyClass.hpp"
#include "Node/FlyNodeTypeManager.hpp"
#include "Pin/FlyPinTypeManager.hpp"
#include "Node/FlyNodeTypeRegistry.hpp"
#include "FlyInternal.hpp"
#include "Command/FlyCommandTracker.hpp"
#include "FlyFoundation.hpp"
#include "Serialization/FlySerializer.hpp"
#include "DataType/FlyPtrValueVariant.hpp"

namespace FLY_NAMESPACE
{
	void InitializeFoundation()
	{
		Foundation::GetInstance().Initialize();
	}

	void DestroyFoundation()
	{
		Foundation::GetInstance().Destroy();
	}

	void LoadAllFlyFiles(const std::filesystem::path& filePath)
	{
		Internal::LoadAllFlyFiles(filePath);
	}

	void SaveCustomEvents(const std::filesystem::path& filePath)
	{
		Internal::SaveCustomEvents(filePath);
	}

	GenericDataTypeProxy CreateStruct(std::string name, const std::filesystem::path& savePath)
	{
		const DataTypeID dataTypeID = Internal::CreateStruct(std::move(name));
		Internal::SaveDataType(*Internal::GetDataTypeByID(dataTypeID), savePath);
		return GenericDataTypeProxy(dataTypeID);
	}

	ClassProxy CreateClass(const GenericDataTypeProxy targetProxy, std::string name, const std::filesystem::path& savePath)
	{
		const ClassID createdClassID = Internal::CreateClass(targetProxy.GetID(), std::move(name));
		Internal::SaveClass(Internal::GetClassByID(createdClassID), savePath);
		return ClassProxy(createdClassID);
	}

	ClassProxy CreateClassWithoutTarget(std::string name, const std::filesystem::path& savePath)
	{
		return CreateClass(GenericDataTypeProxy(GetDataTypeID<None*>()), std::move(name), savePath);
	}

	GenericDataTypeProxy FindDataTypeByName(const std::string_view name)
	{
		return GenericDataTypeProxy{ Internal::GetDataTypeManager().GetDataTypeIDByName(name) };
	}

	ClassProxy FindClassByName(const std::string_view name)
	{
		auto& classes = Internal::GetDataTypeManager().GetClasses();

		for (size_t i = 0; i < classes.size(); ++i)
		{
			if (classes[i]->mName == name)
			{
				return ClassProxy(ClassID{ i });
			}
		}

		return ClassProxy();
	}



	void SetDefaultDataTypeColor(const Color& color)
	{
		Internal::GetDataTypeManager().SetDefaultColor(color);
	}

	void SetEditorTextFunction(const std::function<void(const std::string&)>& textFunction)
	{
		Internal::GetDataTypeManager().SetEditorTextFunction(textFunction);
	}

	void CreateCopyBuffer(const std::span<NodeID> nodeIDs, NodeGraphProxy copiedFromNodeGraphProxy)
	{
		if (nodeIDs.empty())
		{
			return;
		}
		const NodeGraph& copiedFromNodeGraph = copiedFromNodeGraphProxy.GetNodeGraph();
		EventGraph& eventGraphCopy = Internal::GetNodeGraphCopy();
		eventGraphCopy = EventGraph{};

		NodeGraph& nodeGraphCopy = eventGraphCopy.GetNodeGraph();

		std::vector<NodeID> createdNodeIDs;
		std::unordered_map<NodeID, NodeID> nodeConverter;
		createdNodeIDs.reserve(nodeIDs.size());

		Vec2 avgPos;
		for (const NodeID nodeID : nodeIDs)
		{
			const Node& node = copiedFromNodeGraph.GetNode(nodeID);

			const NodeID createdNodeID = Internal::CreateNode(&eventGraphCopy, node.GetTypeID(), node.GetPosition(), nullptr);
			avgPos += node.GetPosition();

			createdNodeIDs.push_back(createdNodeID);
			nodeConverter.emplace(nodeID, createdNodeID);
		}

		avgPos /= static_cast<float>(nodeIDs.size());

		for (NodeID nodeID{ 0 }; nodeID < nodeGraphCopy.GetNodeCount(); ++nodeID)
		{
			Node& node = nodeGraphCopy.GetNode(nodeID);
			node.SetPosition(node.GetPosition() - avgPos);
		}

		// Create links
		for (const NodeID copiedNodeID : nodeIDs)
		{
			const Node& copiedNode = copiedFromNodeGraph.GetNode(copiedNodeID);
			for (const PinID copiedInputPinID : copiedNode.GetInputPins())
			{
				const Pin& copiedInputPin = Internal::GetPin(copiedInputPinID, copiedFromNodeGraph);
				const PinType& copiedInputPinType = Internal::GetPinTypeManager().GetPinType(copiedInputPin.GetTypeID());

				const NodeID createdNodeID = nodeConverter.at(copiedNodeID);
				const PinID createdInputPinID = Internal::GetOpposingPinID(copiedFromNodeGraph, copiedInputPinID, nodeGraphCopy, createdNodeID);
				assert(createdInputPinID != InvalidID<PinID>());

				const std::vector<LinkID> connectedLinks = Internal::GetLinkIDsByPin(copiedFromNodeGraph, copiedInputPinID);

				for (const LinkID connectedLinkID : connectedLinks)
				{
					const Link& connectedLink = copiedFromNodeGraph.GetLink(connectedLinkID);

					const PinID connectedOutputPinID = connectedLink.GetOutputPinID();
					const Pin& connectedOutputPin = Internal::GetPin(connectedOutputPinID, copiedFromNodeGraph);

					auto it = nodeConverter.find(connectedOutputPin.GetNodeID());
					if (it == end(nodeConverter))
					{
						continue;
					}
					const NodeID newConnectedNodeID = it->second;
					const PinID createdOutputPinID = Internal::GetOpposingPinID(copiedFromNodeGraph, connectedOutputPinID, nodeGraphCopy, newConnectedNodeID);

					Internal::TryCreateLink(nodeGraphCopy, createdInputPinID, createdOutputPinID, nullptr);
				}

				Pin& createdInputPin = Internal::GetPin(createdInputPinID, nodeGraphCopy);

				Internal::GetDataTypeManager().CopyData(copiedInputPinType.GetDataTypeID(), createdInputPin.GetDataPtr(), copiedInputPin.GetDataPtr());
			}
		}
	}

	void PasteCopyBuffer(NodeGraphProxy targetNodeGraphProxy, const Vec2 position, CommandTracker* const commandTracker)
	{
		const NodeGraph& nodeGraphCopy = Internal::GetNodeGraphCopy().GetNodeGraph();

		if (commandTracker)
		{
			commandTracker->BeginComposite("Paste Nodes");
		}

		NodeGraph& targetNodeGraph = targetNodeGraphProxy.GetNodeGraph();

		std::unordered_map<NodeID, NodeID> nodeConverter;

		for (NodeID sourceNodeID{ 0 }; sourceNodeID < nodeGraphCopy.GetNodeCount(); sourceNodeID++)
		{
			const Node& node = nodeGraphCopy.GetNode(sourceNodeID);
			const NodeID createdNodeID = Internal::CreateNode(targetNodeGraphProxy.GetVariant(), node.GetTypeID(), position + node.GetPosition(), commandTracker);
			nodeConverter.emplace(sourceNodeID, createdNodeID);

			const Node& createdNode = Internal::GetNode(createdNodeID, targetNodeGraph);
			DataTypeManager& dataTypeManager = Internal::GetDataTypeManager();
			for (const PinID createdInputPinID : createdNode.GetInputPins())
			{
				Pin& createdInputPin = Internal::GetPin(createdInputPinID, targetNodeGraph);
				const PinType& createdInputPinType = Internal::GetPinTypeManager().GetPinType(createdInputPin.GetTypeID());

				const PinID sourcePinID = Internal::GetOpposingPinID(targetNodeGraph, createdInputPinID, nodeGraphCopy, sourceNodeID);
				const Pin& sourcePin = nodeGraphCopy.GetPin(sourcePinID);
				dataTypeManager.CopyData(createdInputPinType.GetDataTypeID(), createdInputPin.GetDataPtr(), sourcePin.GetDataPtr());
			}
		}

		for (const Link& link : nodeGraphCopy.GetLinks())
		{
			const Pin& inputPin = nodeGraphCopy.GetPin(link.GetInputPinID());
			const Pin& outputPin = nodeGraphCopy.GetPin(link.GetOutputPinID());
			const PinID createdInputPinID = Internal::GetOpposingPinID(nodeGraphCopy, link.GetInputPinID(), targetNodeGraph, nodeConverter.at(inputPin.GetNodeID()));
			const PinID createdOutputPinID = Internal::GetOpposingPinID(nodeGraphCopy, link.GetOutputPinID(), targetNodeGraph, nodeConverter.at(outputPin.GetNodeID()));
			const LinkID createdLinkID = Internal::TryCreateLink(targetNodeGraph, createdInputPinID, createdOutputPinID, commandTracker);

			assert(createdLinkID != InvalidID<LinkID>());
		}

		if (commandTracker)
		{
			commandTracker->EndComposite();
		}
	}

	CustomEventProxy CreateCustomEvent(std::string name)
	{
		return CustomEventProxy(Internal::CreateCustomEvent(std::move(name)));
	}

	FunctionProxy CreateGlobalFunction(std::string name)
	{
		return FunctionProxy(Internal::CreateFunction(std::move(name)));
	}

	void BeginFrame(CommandTracker* const commandTracker)
	{
		Internal::BeginFrame(commandTracker);
	}

	template<IsPredicate<const Pin&> Predicate>
	std::vector<PinProxy> GetPinProxysFiltered(Predicate&& predicate, const NodeGraphProxy& nodeGraphProxy)
	{
		std::vector<PinProxy> pinProxys;
		const NodeGraph& nodeGraph = nodeGraphProxy.GetNodeGraph();
		pinProxys.reserve(nodeGraph.mPins.size());

		for (PinID pinID = 0; pinID < nodeGraph.mPins.size(); ++pinID)
		{
			const Pin& pin = nodeGraph.mPins[pinID];
			if (predicate(pin))
			{
				pinProxys.push_back(PinProxy(pinID, nodeGraphProxy));
			}
		}

		return pinProxys;
	}

	bool& IsDebugging()
	{
		return Internal::IsDebugging();
	}

	std::vector<DataTypeProxy> GetDataTypes()
	{
		const auto& dataTypes = Internal::GetDataTypeManager().GetDataTypes();
		std::vector<DataTypeProxy> dataTypeProxys;
		dataTypeProxys.reserve(dataTypes.size());

		for (const auto& [dataTypeID, dataType] : dataTypes)
		{
			dataTypeProxys.push_back(DataTypeProxy(dataTypeID));
		}

		return dataTypeProxys;
	}

	std::vector<GenericDataTypeProxy> GetGenericDataTypes()
	{
		const auto& dataTypes = Internal::GetDataTypeManager().GetDataTypes();
		std::vector<GenericDataTypeProxy> dataTypeProxys;
		dataTypeProxys.reserve(dataTypes.size());

		for (const auto& [dataTypeID, dataType] : dataTypes)
		{
			dataTypeProxys.push_back(GenericDataTypeProxy(dataTypeID));
		}

		return dataTypeProxys;
	}

	DataTypeProxy GetDataTypeProxyByName(std::string_view name)
	{
		return DataTypeProxy(Internal::GetDataTypeManager().GetDataTypeIDByName(std::string(name)));
	}

	std::vector<NodeTypeProxy> GetNodeTypes()
	{
		const auto& nodeTypes = Internal::GetNodeTypeManager().GetNodeTypes();
		std::vector<NodeTypeProxy> views;
		views.reserve(nodeTypes.size());

		for (NodeTypeID nodeTypeID{ 0 }; nodeTypeID < nodeTypes.size(); ++nodeTypeID)
		{
			views.push_back(NodeTypeProxy(nodeTypeID));
		}

		return views;
	}

	std::vector<FunctionProxy> GetFunctions()
	{
		const auto& functions = Internal::GetNodeTypeManager().GetFunctions();
		std::vector<FunctionProxy> views;
		views.reserve(functions.size());

		for (FunctionID functionID{ 0 }; functionID < functions.size(); ++functionID)
		{
			views.push_back(FunctionProxy(functionID));
		}

		return views;
	}

	std::vector<CustomEventProxy> GetCustomEvents()
	{
		const auto& customEvents = Internal::GetNodeTypeManager().GetCustomEvents();
		std::vector<CustomEventProxy> customEventProxys;
		customEventProxys.reserve(customEvents.size());

		for (CustomEventID customEventID{ 0 }; customEventID < customEvents.size(); ++customEventID)
		{
			customEventProxys.push_back(CustomEventProxy(customEventID));
		}

		return customEventProxys;
	}

	std::vector<LinkProxy> GetTraversedLinks()
	{
		std::vector<LinkProxy> linkProxys;
		const std::vector<LinkRef> linkRefs = Internal::GetNodeExecutor().GetDebugger().GetTraversedLinks();
		linkProxys.reserve(linkRefs.size());

		for (auto& linkRef : linkRefs)
		{
			linkProxys.push_back(LinkProxy(linkRef.linkID, NodeGraphProxy(linkRef.nodeGraphVariantHandle)));
		}

		return linkProxys;
	}



	template<typename FilterFunction>
	std::vector<NodeTypeProxy> GetNodeTypesFiltered(FilterFunction&& aFilter)
	{
		std::vector<NodeTypeProxy> facades;
		const std::vector<NodeType>& nodeTypes = Internal::GetNodeTypeManager().GetNodeTypes();
		facades.reserve(nodeTypes.size());
		for (NodeTypeID id{ 0 }; id < nodeTypes.size(); id++)
		{
			if (aFilter(nodeTypes[id]))
			{
				facades.push_back(NodeTypeProxy(id));
			}
		}
		return facades;
	}

	std::vector<NodeTypeProxy> GetNodeTypesFilteredByDataTypeAndFlowType(const GenericDataTypeID aDataTypeID, const eIODirection aIODirection)
	{
		return GetNodeTypesFiltered([aDataTypeID, aIODirection](const NodeType& aNodeType) -> bool
			{
				const std::vector<PinTypeID>& pinTypeIDs = SelectByIODirection(aIODirection, aNodeType.GetInputPinTypeIDs(), aNodeType.GetOutputPinTypeIDs());
				for (const PinTypeID pinTypeID : pinTypeIDs)
				{
					const PinType& pinType = Internal::GetPinTypeManager().GetPinType(pinTypeID);
					if (pinType.GetDataTypeID() == aDataTypeID)
					{
						return true;
					}
				}
				return false;
			}
		);
	}

	NodeTypeProxyIteratorService IterateNodeTypes()
	{
		return NodeTypeProxyIteratorService(NodeTypeID{ Internal::GetNodeTypeManager().GetNodeTypes().size() });
	}

	NodeTypeProxyIteratorService IterateNodeTypes(Predicate<NodeTypeProxy> aFilterPredicate)
	{
		return NodeTypeProxyIteratorService(NodeTypeID{ Internal::GetNodeTypeManager().GetNodeTypes().size() }, aFilterPredicate);
	}

	std::vector<NodeTypeProxy> GetNodeTypesFilteredByRelatedDataTypesAndFlowTypeAndTrait(const GenericDataTypeID aDataTypeID, const eIODirection aIODirection, const eNodeTrait aNodeTrait, bool(*aBitOperation)(eNodeTrait, eNodeTrait))
	{
		return GetNodeTypesFiltered([aDataTypeID, aIODirection, aNodeTrait, aBitOperation](const NodeType& aNodeType) -> bool
			{
				if (!aBitOperation(aNodeTrait, aNodeType.GetTraits()))
				{
					return false;
				}
				const std::vector<PinTypeID>& pinTypeIDs = SelectByIODirection(aIODirection, aNodeType.GetInputPinTypeIDs(), aNodeType.GetOutputPinTypeIDs());
				for (const PinTypeID pinTypeID : pinTypeIDs)
				{
					const PinType& pinType = Internal::GetPinTypeManager().GetPinType(pinTypeID);
					const GenericDataTypeID inputDataTypeID = SelectByIODirection(aIODirection, pinType.GetDataTypeID(), aDataTypeID);
					const GenericDataTypeID outputDataTypeID = SelectByIODirection(aIODirection, aDataTypeID, pinType.GetDataTypeID());
					if (Internal::AreDataTypesLinkable(inputDataTypeID, outputDataTypeID))
					{
						return true;
					}
				}
				return false;
			}
		);
	}

	std::vector<NodeTypeProxy> GetNodeTypesFilteredByTrait(const eNodeTrait aNodeTrait, bool(*aBitOperation)(eNodeTrait, eNodeTrait))
	{
		return GetNodeTypesFiltered([aNodeTrait, aBitOperation](const NodeType& aNodeType) -> bool
			{
				return aBitOperation(aNodeTrait, aNodeType.GetTraits());
			}
		);
	}

	std::unordered_map<DataTypeProxy, std::vector<ClassProxy>> GetClasses()
	{
		auto& classes = Internal::GetDataTypeManager().GetClasses();

		std::unordered_map<DataTypeProxy, std::vector<ClassProxy>> views;

		for (size_t i = 0; i < classes.size(); i++)
		{
			views[DataTypeProxy(classes[i]->mTargetID)].push_back(ClassProxy(ClassID{ i }));
		}

		return views;
	}

	std::vector<ClassProxy> GetClassesByTargetDataType(const DataTypeProxy aDataTypeProxy)
	{
		auto& classes = Internal::GetDataTypeManager().GetClasses();

		std::vector<ClassProxy> views;

		for (size_t i = 0; i < classes.size(); ++i)
		{
			if (classes[i]->mTargetID == aDataTypeProxy.GetID())
			{
				views.push_back(ClassProxy(ClassID{ i }));
			}
		}

		return views;
	}
}

