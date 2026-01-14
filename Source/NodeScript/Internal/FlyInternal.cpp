#include "FlyInternal.hpp"
#include "FlyClass.hpp"
#include "CustomEvent/FlyCustomEvent.hpp"
#include "Node/FlyNodeTypeManager.hpp"
#include "Pin/FlyPinTypeManager.hpp"
#include "DataType/FlyDataTypeManager.hpp"
#include "Command/FlyCommandTracker.hpp"
#include "Execution/FlyNodeExecutor.hpp"
#include "FlyFlow.hpp"
#include "FlyWildcard.hpp"
#include "FlyFoundation.hpp"
#include "Node/FlyNodeTypeRegistry.hpp"
#include "../Trait/FlyTraitObject.hpp"

namespace FLY_NAMESPACE
{

	namespace Internal
	{


		Foundation& GetFoundation()
		{
			return Foundation::GetInstance();
		}

		NodeExecutor& GetNodeExecutor()
		{
			return GetFoundation().GetNodeExecutor();
		}

		DataTypeManager& GetDataTypeManager()
		{
			return GetFoundation().GetDataTypeManager();
		}

		NodeTypeManager& GetNodeTypeManager()
		{
			return GetFoundation().GetNodeTypeManager();
		}

		PinTypeManager& GetPinTypeManager()
		{
			return GetFoundation().GetPinTypeManager();
		}

		TraitManager& GetTraitManager()
		{
			return GetFoundation().GetTraitManager();
		}

		bool& IsDebugging()
		{
			return GetFoundation().IsDebugging();
		}

		MemoryArena<1024>& GetFrameMemoryArena()
		{
			return GetFoundation().GetFrameMemoryArena();
		}

		MemoryArena<10000>& GetEditMemoryArena()
		{
			return GetFoundation().GetEditMemoryArena();
		}

		MemoryPool& GetMemoryPool()
		{
			return GetFoundation().GetMemoryPool();
		}

		EventGraph& GetNodeGraphCopy()
		{
			return GetFoundation().GetNodeGraphCopy();
		}
	}

	struct PinRef
	{
		NodeGraph* mNodeGraph = nullptr;
		PinID mPinID;

		friend bool operator<(const PinRef& a, const PinRef& b)
		{
			if (a.mNodeGraph == b.mNodeGraph)
			{
				return a.mPinID < b.mPinID;
			}
			return a.mNodeGraph < b.mNodeGraph;
		}

		friend bool operator==(const PinRef& a, const PinRef& b)
		{
			if (a.mNodeGraph == b.mNodeGraph)
			{
				return a.mPinID == b.mPinID;
			}
			return a.mNodeGraph == b.mNodeGraph;
		}

		explicit operator bool() const
		{
			return mPinID != InvalidID<PinID>();
		}
	};


	struct PinRefHasher final
	{
		size_t operator()(const PinRef& aPinRef) const
		{
			return reinterpret_cast<size_t>(&aPinRef.mNodeGraph) + static_cast<size_t>(aPinRef.mPinID);
		}
	};

	PinRef gActivePinLastFrame;
	PinRef gActivePinThisFrame;
	Command gChangePinValueCommand;

	namespace Internal
	{

		NodeGraph& GetNodeGraph(const NodeGraphVariantHandle& nodeGraphVariant)
		{
			return std::visit(Visitor{
				[](EventGraph* aEventGraph) -> NodeGraph&
				{
					return aEventGraph->GetNodeGraph();
				},
				[](FunctionIDWrapper functionIDWrapper) -> NodeGraph&
				{
					return GetNodeTypeManager().GetFunction(functionIDWrapper.mID).GetNodeGraph();
				}
				}, nodeGraphVariant);
		}

		const Pin& GetPin(const PinID pinID, const NodeGraph& nodeGraph)
		{
			return nodeGraph.GetPin(pinID);
		}

		Pin& GetPin(const PinID pinID, NodeGraph& nodeGraph)
		{
			return nodeGraph.GetPin(pinID);
		}

		const PinType& GetPinType(const PinID pinID, const NodeGraph& nodeGraph)
		{
			const Pin& pin = GetPin(pinID, nodeGraph);
			return GetPinTypeManager().GetPinType(pin.GetTypeID());
		}

		const PinType& GetPinType(const Pin& pin)
		{
			return GetPinTypeManager().GetPinType(pin.GetTypeID());
		}

		const PinType& GetPinType(const PinTypeID pinTypeID)
		{
			return GetPinTypeManager().GetPinType(pinTypeID);
		}

		Node& GetNode(const NodeID nodeID, NodeGraph& nodeGraph)
		{
			return nodeGraph.GetNode(nodeID);
		}

		const Node& GetNode(const NodeID nodeID, const NodeGraph& nodeGraph)
		{
			return nodeGraph.GetNode(nodeID);
		}

		const NodeType& GetNodeType(const Node& node)
		{
			return GetNodeTypeManager().GetNodeType(node.GetTypeID());
		}

		const NodeType& GetNodeType(const NodeID nodeID, const NodeGraph& nodeGraph)
		{
			const Node& node = GetNode(nodeID, nodeGraph);
			return GetNodeTypeManager().GetNodeType(node.GetTypeID());
		}

		NodeType& GetNodeType(const NodeTypeID nodeTypeID)
		{
			return GetNodeTypeManager().GetNodeType(nodeTypeID);
		}

		const DataType* GetDataTypeByID(const DataTypeID dataTypeID)
		{
			return GetDataTypeManager().Find(dataTypeID);
		}

		const DataType* GetDataTypeByID(const GenericDataTypeID dataTypeID)
		{
			return GetDataTypeManager().Find(dataTypeID);
		}

		Class& GetClassByID(const ClassID flyClassID)
		{
			return GetDataTypeManager().GetClass(flyClassID);
		}

		Trait& GetTraitByID(const TraitID traitID)
		{
			return GetTraitManager().GetTrait(traitID);
		}

		[[nodiscard]] NodeCreationContext CreateNodeCreationContext()
		{
			return NodeCreationContext
			{
				.nodeTypeManager = GetNodeTypeManager(),
				.pinTypeManager = GetPinTypeManager(),
				.dataTypeManager = GetDataTypeManager()
			};
        }

		static void InitializeSubPinsRecursively(const eIODirection ioDirection, const std::vector<PinTypeID>& pinTypeIDs);

		static void InitializeSubPinsForPin(const PinTypeID pinTypeID)
		{
			PinTypeManager& pinTypeManager = GetPinTypeManager();
			const DataTypeManager& dataTypeManager = GetDataTypeManager();
			const eIODirection ioDirection = pinTypeManager.GetPinType(pinTypeID).GetIODirection();
			if (const DataType* dataType = dataTypeManager.Find(pinTypeManager.GetPinType(pinTypeID).GetDataTypeID()))
			{
				for (const Variable& variable : dataType->GetVariableContainer())
				{
					if (const DataType* dataType2 = dataTypeManager.Find(variable.GetDataTypeID()))
					{
						const PinTypeID newSubPinTypeID = pinTypeManager.CreatePinType(variable.Name(), ioDirection, variable.GetDataTypeID(), dataTypeManager.GetSetPinValueFunction(variable.GetDataTypeID(), ioDirection), dataTypeManager.GetSetPinValueFromPinFunction(variable.GetDataTypeID(), ioDirection));

						pinTypeManager.GetPinType(pinTypeID).AddSplitPinTypeID(newSubPinTypeID);
					}
				}
				InitializeSubPinsRecursively(ioDirection, pinTypeManager.GetPinType(pinTypeID).GetSplitPinTypeIDs());
			}

		}

		static void InitializeSubPinsRecursively(const eIODirection ioDirection, const std::vector<PinTypeID>& pinTypeIDs)
		{
			PinTypeManager& pinTypeManager = GetPinTypeManager();
			const DataTypeManager& dataTypeManager = GetDataTypeManager();

			for (const PinTypeID pinTypeID : pinTypeIDs)
			{
				if (const DataType* dataType = dataTypeManager.Find(pinTypeManager.GetPinType(pinTypeID).GetDataTypeID()))
				{
					for (const Variable& variable : dataType->GetVariableContainer())
					{
						if (const DataType* dataType2 = dataTypeManager.Find(variable.GetDataTypeID()))
						{
							const PinTypeID newSubPinTypeID = pinTypeManager.CreatePinType(variable.Name(), ioDirection, variable.GetDataTypeID(), dataTypeManager.GetSetPinValueFunction(variable.GetDataTypeID(), ioDirection), dataTypeManager.GetSetPinValueFromPinFunction(variable.GetDataTypeID(), ioDirection));

							pinTypeManager.GetPinType(pinTypeID).AddSplitPinTypeID(newSubPinTypeID);
						}
					}
					InitializeSubPinsRecursively(ioDirection, pinTypeManager.GetPinType(pinTypeID).GetSplitPinTypeIDs());
				}

			}
		}

		void InitializeSubPins()
		{
			const std::vector<NodeType>& nodeTypes = GetNodeTypeManager().GetNodeTypes();

			for (const NodeType& nodeType : nodeTypes)
			{
				InitializeSubPinsRecursively(eIODirection::Input, nodeType.GetInputPinTypeIDs());
				InitializeSubPinsRecursively(eIODirection::Output, nodeType.GetOutputPinTypeIDs());
			}
		}

		PinTypeID CreatePinType(std::string name, eIODirection ioDirection, GenericDataTypeID dataTypeID)
		{
			const DataTypeManager& dataTypeManager = GetDataTypeManager();
			PinTypeManager& pinTypeManager = GetPinTypeManager();

			const PinTypeID newPinTypeID = pinTypeManager.CreatePinType(name, ioDirection, dataTypeID,
				dataTypeManager.GetSetPinValueFunction(dataTypeID, ioDirection),
				dataTypeManager.GetSetPinValueFromPinFunction(dataTypeID, ioDirection));

			InitializeSubPinsForPin(newPinTypeID);

			return newPinTypeID;
		}

		void BindNodeToEvent(const NodeID nodeID, EventGraph& eventGraph, CommandTracker* const commandTracker)
		{
			struct BindData
			{
				NodeID mNodeID = InvalidID<NodeID>();
				EventGraph* mEventGraph = nullptr;
			} data;

			data.mNodeID = nodeID;
			data.mEventGraph = &eventGraph;

			auto doCommandFunction = [](const BindData& data) -> void
				{
					data.mEventGraph->BindNodeToEvent(data.mNodeID);
				};

			auto undoCommandFunction = [](const BindData& data) -> void
				{
					data.mEventGraph->UnbindNodeFromEvent(data.mNodeID);
				};

			if (!commandTracker)
			{
				doCommandFunction(data);
			}
			else
			{
				commandTracker->DoCommand(Command(std::move(data), doCommandFunction, undoCommandFunction, "Bind Node To Event"));
			}
		}

		void BreakNode()
		{

		}

		DataTypeID CreateStruct(std::string name)
		{
			const DataTypeID dataTypeID = GetDataTypeManager().CreateStruct(std::move(name));
			DataType& dataType = *GetDataTypeManager().Find(dataTypeID);

			const NodeTypeID breakNodeTypeID = RegisterSystemNodeType(BreakNode, NodeCreationData{ .mName = "Default/Break " + dataType.Name() });
			AddPinTypeToNodeType(breakNodeTypeID, GenericDataTypeID{ dataTypeID }, eIODirection::Input, dataType.Name());
			dataType.SetBreakerNodeTypeID(breakNodeTypeID);

			return dataTypeID;
		}

		/*void SetStructName(const StructID aStructID, std::string_view name, CommandTracker* const commandTracker)
		{
			struct SetStructNameData final
			{
				StructID mStructID;
				std::string mNewName;
				std::string mOldName;
			} data;

			data.mStructID = aStructID;
			data.mNewName = name;
			data.mOldName = GetStructByID(aStructID).mName;

			auto doCommandFunction = [](const SetStructNameData& data) -> void
				{
					GetStructByID(data.mStructID).mName = data.mNewName;
				};

			auto undoCommandFunction = [](const SetStructNameData& data) -> void
				{
					GetStructByID(data.mStructID).mName = data.mOldName;
				};

			if (commandTracker)
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Set Class Name"));
			}
			else
			{
				doCommandFunction(data);
			}
		}*/

		ClassID CreateClass(const GenericDataTypeID targetID, std::string name)
		{
			const DataTypeID* dataTypeID = std::get_if<DataTypeID>(&targetID.mID);
			return GetDataTypeManager().CreateClass(*dataTypeID, std::move(name));
		}

		void SetClassName(const ClassID classID, std::string name, CommandTracker* const commandTracker)
		{
			struct SetClassNameData final
			{
				ClassID mClassID;
				std::string mNewName;
				std::string mOldName;
			} data;

			data.mClassID = classID;
			data.mNewName = std::move(name);
			data.mOldName = GetClassByID(classID).mName;

			auto doCommandFunction = [](const SetClassNameData& data) -> void
				{
					GetClassByID(data.mClassID).mName = data.mNewName;
				};

			auto undoCommandFunction = [](const SetClassNameData& data) -> void
				{
					GetClassByID(data.mClassID).mName = data.mOldName;
				};

			if (commandTracker)
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Set Class Name"));
			}
			else
			{
				doCommandFunction(data);
			}
		}

		ClassInstance& CreateClassInstance(const ClassID classID)
		{
			Class& c = GetClassByID(classID);

			return *c.mClassInstances.emplace_back(HeapObject<ClassInstance>(classID));
		}

		void DestroyClassInstance(ClassInstance& classInstance)
		{
			Class& c = GetClassByID(classInstance.mClassID);

			std::erase_if(c.mClassInstances,
				[eraseClassInstance = classInstance](const auto& classInstance) { return &eraseClassInstance == classInstance.Get(); });
		}

		TraitID CreateTrait(std::string name)
		{
			return GetTraitManager().CreateTrait(name);
		}

		Flow InputTraitNode()
		{
			return Flow(true);
		}

		void OutputTraitNode(Flow)
		{

		}

		void CreateTraitImplementation(const DataTypeID dataTypeID, const TraitID traitID)
		{
			TraitManager& traitManager = GetTraitManager();
			traitManager.CreateTraitImplementation(dataTypeID, traitID);
			TraitImplementation* traitImplementaion = traitManager.GetTraitImplementation(dataTypeID, traitID);
			const Trait& trait = traitManager.GetTrait(traitID);
			traitImplementaion;
			trait;

		}

		CustomEventID CreateCustomEvent(std::string name)
		{
			return GetNodeTypeManager().CreateCustomEvent(name);
		}

		FunctionID CreateFunction(std::string name)
		{
			const FunctionID id = GetNodeTypeManager().CreateFunction(name);

			Function& createdFunction = GetNodeTypeManager().GetFunction(id);

			[[maybe_unused]] const NodeID inputNodeID = CreateNode(FunctionIDWrapper(id), createdFunction.GetInputNodeTypeID());
			[[maybe_unused]] const NodeID outputNodeID = CreateNode(FunctionIDWrapper(id), createdFunction.GetOutputNodeTypeID());

			return id;
		}

		NodeID CreateNode(const NodeGraphVariantHandle& nodeGraphVariant, const NodeTypeID nodeTypeID, const Vec2 position, CommandTracker* const commandTracker)
		{
			if (commandTracker)
			{
				commandTracker->BeginComposite("Create Node + Set Position");
			}

			NodeGraph& nodeGraph = GetNodeGraph(nodeGraphVariant);
			const NodeID nodeID = nodeGraph.GetCurrentNodeID();
			AddNode(nodeGraph, GetNodeTypeManager().CreateNode(nodeGraph, nodeID, nodeTypeID, CreateNodeCreationContext()), nodeID, commandTracker);

			SetNodePosition(nodeID, position, nodeGraph, commandTracker);

			std::visit(
				[&](auto&& type) -> void
				{
					using Type = std::decay_t<decltype(type)>;
					if constexpr (std::same_as<Type, EventGraph*>)
					{
						BindNodeToEvent(nodeID, *type, commandTracker);
					}

				}, nodeGraphVariant
			);

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
			return nodeID;
		}


		std::optional<NodeID> CreateNode(const NodeGraphVariantHandle& nodeGraphVariant, std::string name, const Vec2 position, const bool createIfNameNotFound, CommandTracker* const commandTracker)
		{
			const NodeTypeID typeID = GetNodeTypeManager().GetTypeID(name);
			if (typeID != NodeTypeID{ 0 } && !createIfNameNotFound)
			{
				return std::nullopt;
			}
			return CreateNode(nodeGraphVariant, typeID, position, commandTracker);
		}

		NodeID CreateNodeAutoLink(const NodeGraphVariantHandle& nodeGraphVariant, const NodeTypeID nodeTypeID, const PinID connectingPinID, const Vec2 position, CommandTracker* const commandTracker)
		{
			if (commandTracker)
			{
				commandTracker->BeginComposite("Create Node + Auto Link");
			}

			const NodeID createdNodeID = Internal::CreateNode(nodeGraphVariant, nodeTypeID, position, commandTracker);

			const Pin& createdFromPin = GetNodeGraph(nodeGraphVariant).GetPin(connectingPinID);
			const PinType& pinType = GetPinTypeManager().GetPinType(createdFromPin.GetTypeID());
			const Node& createdNode = GetNodeGraph(nodeGraphVariant).GetNode(createdNodeID);

			const std::vector<PinID>& pinIDs = SelectByIODirection(pinType.GetIODirection(), createdNode.GetOutputPins(), createdNode.GetInputPins());

			for (const PinID pinID : pinIDs)
			{
				if (Internal::TryCreateLink(GetNodeGraph(nodeGraphVariant), pinID, connectingPinID, commandTracker) != InvalidID<LinkID>())
				{
					break;
				}
			}

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}

			return createdNodeID;
		}

		NodeID CreateGetterNode(NodeGraph& nodeGraph, const VarID varID, Class& flyClass, const DataTypeID dataTypeID, Vec2 position, CommandTracker* const commandTracker)
		{
			if (commandTracker)
			{
				commandTracker->BeginComposite("Create Getter Node");
			}

			const NodeID nodeID = nodeGraph.GetCurrentNodeID();
			AddNode(nodeGraph, GetNodeTypeManager().CreateGetterNode(nodeGraph, nodeID, dataTypeID, CreateNodeCreationContext()), nodeID, commandTracker);

			Internal::SetNodePosition(nodeID, position, nodeGraph, commandTracker);
			Internal::BindVariable(flyClass, CreateContextualNodeRef(nodeID, nodeGraph), varID, commandTracker);

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}

			return nodeID;
		}

		NodeID CreateSetterNode(NodeGraph& nodeGraph, const VarID varID, Class& flyClass, const DataTypeID dataTypeID, Vec2 position, CommandTracker* const commandTracker)
		{
			if (commandTracker)
			{
				commandTracker->BeginComposite("Create Setter Node");
			}

			const NodeID nodeID = nodeGraph.GetCurrentNodeID();
			AddNode(nodeGraph, GetNodeTypeManager().CreateSetterNode(nodeGraph, nodeID, dataTypeID, CreateNodeCreationContext()), nodeID, commandTracker);

			Internal::SetNodePosition(nodeID, position, nodeGraph, commandTracker);
			Internal::BindVariable(flyClass, CreateContextualNodeRef(nodeID, nodeGraph), varID, commandTracker);

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}

			return nodeID;
		}

		NodeID CreateTraitNode(NodeGraph& nodeGraph, const TraitID traitID, const DataTypeID dataTypeID, CommandTracker* const commandTracker)
		{
			const NodeID nodeID = nodeGraph.GetCurrentNodeID();
			AddNode(nodeGraph, GetNodeTypeManager().CreateTraitNode(nodeGraph, nodeID, traitID, dataTypeID, CreateNodeCreationContext()), nodeID, commandTracker);
			return nodeID;
		}

		NodeID CreateOperatorNode(NodeGraph& nodeGraph, const eNodeOperatorType operatorType, const DataTypeID dataTypeID, CommandTracker* const commandTracker)
		{
			const NodeID nodeID = nodeGraph.GetCurrentNodeID();
			AddNode(nodeGraph, GetNodeTypeManager().CreateOperatorNode(nodeGraph, nodeID, operatorType, dataTypeID, CreateNodeCreationContext()), nodeID, commandTracker);
			return nodeID;
		}

		void AddNode(NodeGraph& nodeGraph, Node&& node, const NodeID nodeID, CommandTracker* const commandTracker)
		{
			nodeGraph.AddNode(node);

			Node& createdNode = nodeGraph.GetNode(nodeID);

			NodeType& nodeType = GetNodeTypeManager().GetNodeType(createdNode.GetTypeID());
			nodeType.GetNodeRefs().push_back(CreateContextualNodeRef(nodeID, nodeGraph));

			struct CreateNodeData
			{
				NodeID mNodeID;
				NodeGraph* mNodeGraph = nullptr;
			} data;

			data.mNodeID = nodeID;
			data.mNodeGraph = &nodeGraph;

			auto doCommandFunction = [](const auto& data) -> void
				{
					GetNode(data.mNodeID, *data.mNodeGraph).SetIsDestroyed(false);
				};

			auto undoCommandFunction = [](const auto& data) -> void
				{
					GetNode(data.mNodeID, *data.mNodeGraph).SetIsDestroyed(true);
				};

			if (!commandTracker)
			{
				doCommandFunction(data);
			}
			else
			{
				commandTracker->DoCommand(Command(std::move(data), doCommandFunction, undoCommandFunction, "Create Node"));
			}
		}

		void DestroyNode(NodeGraph& nodeGraph, const NodeID nodeID, CommandTracker* commandTracker)
		{
			if (commandTracker)
			{
				commandTracker->BeginComposite("Destroy Node + Conncected Links");

			}

			struct DestroyNodeData
			{
				NodeID mNodeID = InvalidID<NodeID>();
				NodeGraph* mNodeGraph = nullptr;
			} data;

			data.mNodeID = nodeID;
			data.mNodeGraph = &nodeGraph;


			auto doCommandFunction = [](const auto& data) -> void
				{
					GetNode(data.mNodeID, *data.mNodeGraph).SetIsDestroyed(true);
				};

			auto undoCommandFunction = [](const auto& data) -> void
				{
					GetNode(data.mNodeID, *data.mNodeGraph).SetIsDestroyed(false);
				};

			if (!commandTracker)
			{
				doCommandFunction(data);
			}
			else
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Destroy Node"));
			}

			for (const LinkID linkID : GetLinkIDsByNode(nodeGraph, nodeID))
			{
				DestroyLink(nodeGraph, linkID, commandTracker);
			}


			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
		}

		void DestroyNodes(const std::span<GlobalNodeRef> nodeRefs, CommandTracker* commandTracker)
		{
			if (commandTracker)
			{
				commandTracker->BeginComposite("Destroy Nodes");
			}

			for (const GlobalNodeRef& nodeRef : nodeRefs)
			{
				DestroyNode(nodeRef.GetNodeGraph(), nodeRef.GetNodeID(), commandTracker);
			}

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
		}

		void DestroyNodes(const std::span<NodeRef> nodeRefs, CommandTracker* const commandTracker)
		{
			if (commandTracker)
			{
				commandTracker->BeginComposite("Destroy Nodes");
			}

			for (const NodeRef& nodeRef : nodeRefs)
			{
				DestroyNode(nodeRef.GetNodeGraph(), nodeRef.GetNodeID(), commandTracker);
			}

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
		}

		void DestroyNodes(const std::span<NodeID> nodeIDs, NodeGraph& nodeGraph, CommandTracker* const commandTracker)
		{
			if (commandTracker)
			{
				commandTracker->BeginComposite("Destroy Nodes");
			}

			for (const NodeID nodeID : nodeIDs)
			{
				DestroyNode(nodeGraph, nodeID, commandTracker);
			}

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
		}

		void DestroyLinks(const std::span<LinkID> linkIDs, NodeGraph& nodeGraph, CommandTracker* const commandTracker)
		{
			if (commandTracker)
			{
				commandTracker->BeginComposite("Destroy Links");
			}

			for (const LinkID linkID : linkIDs)
			{
				DestroyLink(nodeGraph, linkID, commandTracker);
			}

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
		}

		void DestroySelection(const std::span<NodeID> nodeIDs, const std::span<LinkID> linkIDs, NodeGraph& nodeGraph, CommandTracker* const commandTracker)
		{
			if (commandTracker)
			{
				commandTracker->BeginComposite("Destroy Selection");
			}

			DestroyLinks(linkIDs, nodeGraph, commandTracker);
			DestroyNodes(nodeIDs, nodeGraph, commandTracker);

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
		}

		void SetNodePosition(const NodeID nodeID, const Vec2 position, NodeGraph& nodeGraph, CommandTracker* const commandTracker)
		{
			const Vec2 oldPos = nodeGraph.GetNode(nodeID).GetPosition();
			SetNodePosition(nodeID, position, oldPos, nodeGraph, commandTracker);
		}

		void SetNodePosition(const NodeID nodeID, const Vec2 position, const Vec2 oldPosition, NodeGraph& nodeGraph, CommandTracker* const commandTracker)
		{

			struct SetNodePositionData final
			{
				NodeID nodeID;
				Vec2 oldPos;
				Vec2 newPos;
				NodeGraph* nodeGraph = nullptr;
			} data;

			if (position == oldPosition)
			{
				return;
			}


			data.nodeID = nodeID;
			data.oldPos = oldPosition;
			data.newPos = position;
			data.nodeGraph = &nodeGraph;

			auto doCommandFunction = [](const SetNodePositionData& data) -> void
				{
					Node& node = GetNode(data.nodeID, *data.nodeGraph);
					node.SetPosition(data.newPos);
				};

			auto undoCommandFunction = [](const SetNodePositionData& data) -> void
				{
					Node& node = GetNode(data.nodeID, *data.nodeGraph);
					node.SetPosition(data.oldPos);
				};

			if (!commandTracker)
			{
				doCommandFunction(data);
			}
			else
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Set Node Position"));
			}
		}

		void CommitNodeDrag(const std::unordered_map<NodeID, NodeDragData>& nodeDragData, NodeGraph& nodeGraph, CommandTracker* const commandTracker)
		{
			if (nodeDragData.empty())
			{
				return;
			}

			if (commandTracker)
			{
				commandTracker->BeginComposite("Drag nodes");
			}

			for (const auto& [nodeID, dragData] : nodeDragData)
			{
				Internal::SetNodePosition(nodeID, dragData.endPos, dragData.startPos, nodeGraph, commandTracker);
			}

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
		}

		static std::vector<PinID> CreatePins(NodeGraph& nodeGraph, const NodeID nodeID, const NodeTypeID nodeTypeID, const eIODirection ioDirection, const std::size_t startIndex, const NodeCreationContext& creationContext)
		{
			const NodeType& nodeType = GetNodeTypeManager().GetNodeType(nodeTypeID);
			const std::vector<PinTypeID>& pinTypeIDs = SelectByIODirection(ioDirection, nodeType.GetInputPinTypeIDs(), nodeType.GetOutputPinTypeIDs());
			std::vector<PinID> pinIDs;
			for (size_t i = startIndex; i < pinTypeIDs.size(); i++)
			{
				pinIDs.push_back(CreatePin(nodeGraph, nodeID, pinTypeIDs[i], creationContext));
			}

			return pinIDs;
		}

		std::vector<PinID> CreateInputPins(NodeGraph& nodeGraph, const NodeID nodeID, const NodeTypeID nodeTypeID, const NodeCreationContext& creationContext, const size_t startIndex)
		{
			return CreatePins(nodeGraph, nodeID, nodeTypeID, eIODirection::Input, startIndex, creationContext);
		}

		std::vector<PinID> CreateOutputPins(NodeGraph& nodeGraph, const NodeID nodeID, const NodeTypeID nodeTypeID, const NodeCreationContext& creationContext, const size_t startIndex)
		{
			return CreatePins(nodeGraph, nodeID, nodeTypeID, eIODirection::Output, startIndex, creationContext);
		}

		PinID CreatePin(NodeGraph& nodeGraph, const NodeID nodeID, const PinTypeID pinTypeID, const NodeCreationContext& creationContext)
		{
			const auto dataTypeID = creationContext.pinTypeManager.GetPinType(pinTypeID).GetDataTypeID();

			void* const dataPtr = creationContext.dataTypeManager.AllocateData(dataTypeID, nodeGraph.GetMemoryArena());

			return CreatePin(nodeGraph, nodeID, pinTypeID, dataPtr, creationContext);
		}

		PinID CreatePin(NodeGraph& nodeGraph, const NodeID nodeID, const PinTypeID pinTypeID, void* const dataPtr, const NodeCreationContext& creationContext)
		{
			const PinType& pinType = GetPinType(pinTypeID);

			const PinID createdPinID = nodeGraph.AddPin(Pin(pinTypeID, dataPtr, nodeID));

			if (pinType.GetSplitPinTypeIDs().empty())
			{
				return createdPinID;
			}

			const DataType* dataType = creationContext.dataTypeManager.Find(pinType.GetDataTypeID());
			if (!dataType)
			{
				return createdPinID;
			}

			std::vector<PinID> subPinIDs;
			subPinIDs.reserve(pinType.GetSplitPinTypeIDs().size());

			assert(pinType.GetSplitPinTypeIDs().size() == dataType->GetVariableContainer().GetVariableCount());
			for (size_t i = 0; i < pinType.GetSplitPinTypeIDs().size(); i++)
			{
				void* const varDataPtr = reinterpret_cast<char*>(dataPtr) + dataType->GetVariableContainer().GetVariable(i).GetByteOffset();

				const PinID createdSubPinID = CreatePin(nodeGraph, nodeID, pinType.GetSplitPinTypeIDs()[i], varDataPtr, creationContext);
				subPinIDs.push_back(createdSubPinID);

				nodeGraph.GetPin(createdSubPinID).SetParentPinID(createdPinID);
			}

			Pin& createdPin = nodeGraph.GetPin(createdPinID);
			createdPin.SetSplitPinIDs(std::move(subPinIDs));

			return createdPinID;
		}

		void ViewAndEditPinGeneric(const PinID pinID, NodeGraph& nodeGraph, CommandTracker* const commandTracker)
		{
			Pin& pin = GetPin(pinID, nodeGraph);

			if (pin.IsSplit())
			{
				assert(false);
				return;
			}

			const PinType& pinType = GetPinType(pin);

			const DataTypeManager& dataTypeManager = GetDataTypeManager();

			const void* const copyDataPtr = [commandTracker, &dataTypeManager, &pinType, &pin]() -> const void*
				{
					return commandTracker != nullptr ? dataTypeManager.AllocateData(pinType.GetDataTypeID(), Internal::GetFrameMemoryArena(), pin.GetDataPtr()) : nullptr;
				}();

			const ViewAndEditResult viewAndEditResult = dataTypeManager.ViewAndEditData(pinType.GetDataTypeID(), pin.GetDataPtr());

			if (!viewAndEditResult.mIsItemActive || !commandTracker)
			{
				return;
			}

			const PinRef pinRef{ .mNodeGraph = &nodeGraph, .mPinID = pinID };

			gActivePinThisFrame = pinRef;

			if (gActivePinLastFrame == gActivePinThisFrame)
			{
				return;
			}

			void* const previousDataPtr = GetDataTypeManager().AllocateData(pinType.GetDataTypeID(), Internal::GetEditMemoryArena(), copyDataPtr);

			struct EditPinData
			{
				PinID mPinID;
				void* mPreviousDataPtr = nullptr;
				NodeGraph* mNodeGraph = nullptr;
			} data;

			data.mPinID = pinID;
			data.mPreviousDataPtr = previousDataPtr;
			data.mNodeGraph = &nodeGraph;

			auto doCommandFunction = [](const EditPinData& data) -> void
				{
					Pin& pin = GetPin(data.mPinID, *data.mNodeGraph);
					const PinType& pinType = GetPinType(pin);

					GetDataTypeManager().SwapData(pinType.GetDataTypeID(), pin.GetDataPtr(), data.mPreviousDataPtr);
				};

			auto undoCommandFunction = [](const EditPinData& data) -> void
				{
					Pin& pin = GetPin(data.mPinID, *data.mNodeGraph);
					const PinType& pinType = GetPinType(pin);

					GetDataTypeManager().SwapData(pinType.GetDataTypeID(), pin.GetDataPtr(), data.mPreviousDataPtr);
				};

			gChangePinValueCommand = Command(data, doCommandFunction, undoCommandFunction, "Edit Pin");
		}


		/*void ViewAndEditPin(const PinID pinID, NodeGraph& nodeGraph, CommandTracker* const commandTracker)
		{
			Pin& pin = GetPin(pinID, nodeGraph);

			if (pin.mIsSplit)
			{
				assert(false);
				return;
			}

			const PinType& pinType = GetPinType(pin);

			const DataTypeManager& dataTypeManager = GetDataTypeManager();

			const void* const copyDataPtr = [commandTracker, &dataTypeManager, &pinType, &pin]() -> const void*
				{
					return commandTracker != nullptr ? dataTypeManager.AllocateData(pinType.mDataTypeID, Internal::GetFrameMemoryArena(), pin.mDataPtr) : nullptr;
				}();

			const ViewAndEditResult viewAndEditResult = dataTypeManager.ViewAndEditData(pinType.mDataTypeID, pin.mDataPtr);

			if (!viewAndEditResult.mIsItemActive || !commandTracker)
			{
				return;
			}

			const PinRef pinRef{ .mNodeGraph = &nodeGraph, .mPinID = pinID };

			gActivePinThisFrame = pinRef;

			if (gActivePinLastFrame == gActivePinThisFrame)
			{
				return;
			}

			void* const previousDataPtr = GetDataTypeManager().AllocateData(pinType.mDataTypeID, Internal::GetEditMemoryArena(), copyDataPtr);

			struct EditPinData
			{
				PinID mPinID;
				void* mPreviousDataPtr = nullptr;
				NodeGraph* mNodeGraph = nullptr;
			} data;

			data.mPinID = pinID;
			data.mPreviousDataPtr = previousDataPtr;
			data.mNodeGraph = &nodeGraph;

			auto doCommandFunction = [](const EditPinData& data) -> void
				{
					Pin& pin = GetPin(data.mPinID, *data.mNodeGraph);
					const PinType& pinType = GetPinType(pin);

					GetDataTypeManager().SwapData(pinType.mDataTypeID, pin.mDataPtr, data.mPreviousDataPtr);
				};

			auto undoCommandFunction = [](const EditPinData& data) -> void
				{
					Pin& pin = GetPin(data.mPinID, *data.mNodeGraph);
					const PinType& pinType = GetPinType(pin);

					GetDataTypeManager().SwapData(pinType.mDataTypeID, pin.mDataPtr, data.mPreviousDataPtr);
				};

			gChangePinValueCommand = Command(data, doCommandFunction, undoCommandFunction, "Edit Pin");
		}*/

		void ViewPinGeneric(PinID pinID, const NodeGraph& nodeGraph)
		{
			GetDataTypeManager().ViewData(GetPinType(pinID, nodeGraph).GetDataTypeID(), nodeGraph.GetPin(pinID).GetDataPtr());
		}

		/*void ViewPin(const PinID pinID, const NodeGraph& nodeGraph)
		{
			GetDataTypeManager().ViewData(GetPinType(pinID, nodeGraph).mDataTypeID, GetPin(pinID, nodeGraph).mDataPtr);
		}*/

		static void SplitPinInternal(const PinID pinID, NodeGraph& nodeGraph, const size_t index)
		{
			Pin& pin = GetPin(pinID, nodeGraph);
			pin.SetIsSplit(true);
			Node& node = GetNode(pin.GetNodeID(), nodeGraph);
			const PinType& pinType = GetPinType(pinID, nodeGraph);
			auto& nodePins = SelectByIODirection(pinType.GetIODirection(), node.GetSplitInputPins(), node.GetSplitOutputPins());

			nodePins.erase(begin(nodePins) + index);
			nodePins.insert(begin(nodePins) + index, begin(pin.GetSplitPinIDs()), end(pin.GetSplitPinIDs()));
		}

		static void RecombinePinInternal(const PinID pinID, NodeGraph& nodeGraph, const size_t index)
		{
			Pin& pin = GetPin(pinID, nodeGraph);
			pin.SetIsSplit(false);
			Node& node = GetNode(pin.GetNodeID(), nodeGraph);
			const PinType& pinType = GetPinType(pinID, nodeGraph);
			auto& nodePins = SelectByIODirection(pinType.GetIODirection(), node.GetSplitInputPins(), node.GetSplitOutputPins());

			nodePins.erase(nodePins.begin() + index, begin(nodePins) + index + pin.GetSplitPinIDs().size());
			nodePins.insert(nodePins.begin() + index, pinID);
		}

		void SplitPin(const PinID pinID, NodeGraph& nodeGraph, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			Pin& pin = GetPin(pinID, nodeGraph);

			if (!pin.GetConnectedPinIDs().empty())
			{
				assert(false);
				return;
			}

			Node& node = GetNode(pin.GetNodeID(), nodeGraph);

			const PinType& pinType = GetPinType(pinID, nodeGraph);

			auto& nodePins = SelectByIODirection(pinType.GetIODirection(), node.GetSplitInputPins(), node.GetSplitOutputPins());

			auto it = std::find(begin(nodePins), end(nodePins), pinID);
			if (it == nodePins.end())
			{
				assert(false);
				return;
			}

			struct SplitPinData final
			{
				size_t mIndex = std::numeric_limits<size_t>::max();
				PinID mPinID = InvalidID<PinID>();
				NodeGraph* mNodeGraph = nullptr;
			} data;

			data.mIndex = std::distance(begin(nodePins), it);
			data.mPinID = pinID;
			data.mNodeGraph = &nodeGraph;

			auto doCommandFunction = [](const SplitPinData& data) -> void
				{
					SplitPinInternal(data.mPinID, *data.mNodeGraph, data.mIndex);
				};

			auto undoCommandFunction = [](const SplitPinData& data) -> void
				{
					RecombinePinInternal(data.mPinID, *data.mNodeGraph, data.mIndex);
				};

			if (commandTracker)
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Split Pin"));
			}
			else
			{
				doCommandFunction(data);
			}
		}

		void RecombinePin(const PinID pinID, NodeGraph& nodeGraph, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			Pin& pin = GetPin(pinID, nodeGraph);

			if (!pin.GetConnectedPinIDs().empty())
			{
				assert(false);
				return;
			}

			Node& node = GetNode(pin.GetNodeID(), nodeGraph);

			const PinType& pinType = GetPinType(pinID, nodeGraph);

			const auto& nodePins = SelectByIODirection(pinType.GetIODirection(), node.GetSplitInputPins(), node.GetSplitOutputPins());

			auto it = std::find(nodePins.begin(), nodePins.end(), pin.GetSplitPinIDs().front());
			if (it == nodePins.end())
			{
				assert(false);
				return;
			}

			struct RecombinePinData final
			{
				size_t mIndex = std::numeric_limits<size_t>::max();
				PinID mPinID;
				NodeGraph* mNodeGraph = nullptr;
			} data;

			data.mIndex = std::distance(nodePins.begin(), it);
			data.mPinID = pinID;
			data.mNodeGraph = &nodeGraph;

			auto doCommandFunction = [](const RecombinePinData& data) -> void
				{
					RecombinePinInternal(data.mPinID, *data.mNodeGraph, data.mIndex);
				};

			auto undoCommandFunction = [](const RecombinePinData& data) -> void
				{
					SplitPinInternal(data.mPinID, *data.mNodeGraph, data.mIndex);
				};

			if (commandTracker)
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Recombine Pin"));
			}
			else
			{
				doCommandFunction(data);
			}
		}

		void BeginFrame(CommandTracker* const commandTracker)
		{
			if (!commandTracker)
			{
				return;
			}

			if (gActivePinLastFrame && gActivePinLastFrame != gActivePinThisFrame)
			{
				commandTracker->RegisterCommand(Command(gChangePinValueCommand));
			}

			gActivePinLastFrame = gActivePinThisFrame;
			gActivePinThisFrame = PinRef{};


			Internal::GetFrameMemoryArena().Clear();
			GetNodeExecutor().GetDebugger().ClearTraversedLinks();
		}

		bool IsNodeReplacable(NodeGraph& nodeGraph, NodeID nodeID)
		{
			const NodeType& nodeType = GetNodeType(nodeID, nodeGraph);
			return nodeType.GetTraitID() != InvalidID<TraitID>();
			//return nodeType.mNodeRecipe.mOperatorTrait != eNodeOperatorTrait::None;
		}

		void ActivateLink(NodeGraph& nodeGraph, const LinkID linkID)
		{
			Link& link = nodeGraph.GetLink(linkID);

			{
				Pin& inputPin = nodeGraph.GetPin(link.GetInputPinID());
				auto it = std::find(begin(inputPin.GetConnectedPinIDs()), end(inputPin.GetConnectedPinIDs()), link.GetOutputPinID());
				if (it == end(inputPin.GetConnectedPinIDs()))
				{
					inputPin.AddConnectedPin(link.GetOutputPinID());
				}
			}
			{
				Pin& outputPin = nodeGraph.GetPin(link.GetOutputPinID());
				auto it = std::find(begin(outputPin.GetConnectedPinIDs()), end(outputPin.GetConnectedPinIDs()), link.GetInputPinID());
				if (it == end(outputPin.GetConnectedPinIDs()))
				{
					outputPin.AddConnectedPin(link.GetInputPinID());
				}
			}

			link.SetIsDestroyed(false);
		}

		void DeactivateLink(NodeGraph& nodeGraph, const LinkID linkID)
		{
			Link& link = nodeGraph.GetLink(linkID);

			Pin& inputPin = nodeGraph.GetPin(link.GetInputPinID());
			Pin& outputPin = nodeGraph.GetPin(link.GetOutputPinID());

			inputPin.RemoveConnectedPin(link.GetOutputPinID());
			outputPin.RemoveConnectedPin(link.GetInputPinID());

			link.SetIsDestroyed(true);
		}

		LinkID TryCreateLink(NodeGraph& nodeGraph, const PinID pinID1, const PinID pinID2, CommandTracker* const commandTracker)
		{
			const Link createdLink = ArePinsLinkable(nodeGraph, pinID1, pinID2);
			if (!createdLink)
			{
				// Check if we can replace node with overloaded operator node
				const Pin& pin1 = nodeGraph.GetPin(pinID1);
				const Pin& pin2 = nodeGraph.GetPin(pinID2);

				const PinType& pinType1 = GetPinTypeManager().GetPinType(pin1.GetTypeID());
				const PinType& pinType2 = GetPinTypeManager().GetPinType(pin2.GetTypeID());

				if (pinType1.GetDataTypeID() == GenericDataTypeID{ GetDataTypeID<Wildcard>() })
				{
					ReplaceTemplateNodeWithLink(nodeGraph, pinID1, pinID2, commandTracker);
				}
				else if (pinType2.GetDataTypeID() == GenericDataTypeID{ GetDataTypeID<Wildcard>() })
				{
					ReplaceTemplateNodeWithLink(nodeGraph, pinID2, pinID1, commandTracker);
				}

				return InvalidID<LinkID>();
			}

			return CreateLink(nodeGraph, createdLink.GetInputPinID(), createdLink.GetOutputPinID(), commandTracker);
		}

		LinkID CreateLinkGeneric(NodeGraph& nodeGraph, const PinID inputPinID, const PinID outputPinID, CommandTracker* const commandTracker)
		{
			assert(inputPinID != InvalidID<PinID>());
			assert(outputPinID != InvalidID<PinID>());

			struct CreateLinkData final
			{
				LinkID mCreatedLinkID;
				LinkID mPreviousLinkID;
				NodeGraph* mNodeGraph = nullptr;
			} data;

			const Pin& inputPin = nodeGraph.GetPin(inputPinID);
			const Pin& outputPin = nodeGraph.GetPin(outputPinID);
			const PinType& inputPinType = GetPinTypeManager().GetPinType(inputPin.GetTypeID());
			[[maybe_unused]] const PinType& outputPinType = GetPinTypeManager().GetPinType(outputPin.GetTypeID());
			assert(inputPinType.GetIODirection() == eIODirection::Input);
			assert(outputPinType.GetIODirection() == eIODirection::Output);
			assert(GetDataTypeManager().AreDataTypesRelated(inputPinType.GetDataTypeID(), outputPinType.GetDataTypeID()));

			if (inputPinType.GetDataTypeID() != GenericDataTypeID{ GetDataTypeID<Flow>() })
			{
				const std::vector<LinkID> inputLinkIDs = GetLinkIDsByPin(nodeGraph, inputPinID);
				if (!inputLinkIDs.empty())
				{
					assert(inputLinkIDs.size() == 1);
					data.mPreviousLinkID = inputLinkIDs.front();
				}
			}
			else
			{
				const std::vector<LinkID> outputLinkIDs = GetLinkIDsByPin(nodeGraph, outputPinID);
				if (!outputLinkIDs.empty())
				{
					assert(outputLinkIDs.size() == 1);
					data.mPreviousLinkID = outputLinkIDs.front();
				}
			}

			data.mNodeGraph = &nodeGraph;
			data.mCreatedLinkID = nodeGraph.AddLink(Link{ inputPinID, outputPinID });

			auto doCommandFunction = [](const CreateLinkData& data) -> void
				{
					if (data.mPreviousLinkID != InvalidID<LinkID>())
					{
						DeactivateLink(*data.mNodeGraph, data.mPreviousLinkID);
					}

					ActivateLink(*data.mNodeGraph, data.mCreatedLinkID);
				};

			auto undoCommandFunction = [](const CreateLinkData& data) -> void
				{
					DeactivateLink(*data.mNodeGraph, data.mCreatedLinkID);
					if (data.mPreviousLinkID != InvalidID<LinkID>())
					{
						ActivateLink(*data.mNodeGraph, data.mPreviousLinkID);
					}
				};

			if (!commandTracker)
			{
				doCommandFunction(data);
			}
			else
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Create Link"));
			}

			return data.mCreatedLinkID;
		}

		LinkID CreateLink(NodeGraph& nodeGraph, const PinID inputPinID, const PinID outputPinID, CommandTracker* const commandTracker)
		{
			assert(inputPinID != InvalidID<PinID>());
			assert(outputPinID != InvalidID<PinID>());

			struct CreateLinkData final
			{
				LinkID mCreatedLinkID;
				LinkID mPreviousLinkID;
				NodeGraph* mNodeGraph = nullptr;
			} data;

			const Pin& inputPin = nodeGraph.GetPin(inputPinID);
			const Pin& outputPin = nodeGraph.GetPin(outputPinID);
			const PinType& inputPinType = GetPinTypeManager().GetPinType(inputPin.GetTypeID());
			[[maybe_unused]] const PinType& outputPinType = GetPinTypeManager().GetPinType(outputPin.GetTypeID());
			assert(inputPinType.GetIODirection() == eIODirection::Input);
			assert(outputPinType.GetIODirection() == eIODirection::Output);
			assert(GetDataTypeManager().AreDataTypesRelated(inputPinType.GetDataTypeID(), outputPinType.GetDataTypeID()));

			if (inputPinType.GetDataTypeID() != GenericDataTypeID{ GetDataTypeID<Flow>() })
			{
				const std::vector<LinkID> inputLinkIDs = GetLinkIDsByPin(nodeGraph, inputPinID);
				if (!inputLinkIDs.empty())
				{
					assert(inputLinkIDs.size() == 1);
					data.mPreviousLinkID = inputLinkIDs.front();
				}
			}
			else
			{
				const std::vector<LinkID> outputLinkIDs = GetLinkIDsByPin(nodeGraph, outputPinID);
				if (!outputLinkIDs.empty())
				{
					assert(outputLinkIDs.size() == 1);
					data.mPreviousLinkID = outputLinkIDs.front();
				}
			}

			data.mNodeGraph = &nodeGraph;
			data.mCreatedLinkID = nodeGraph.AddLink(Link{ inputPinID, outputPinID });


			auto doCommandFunction = [](const CreateLinkData& data) -> void
				{
					if (data.mPreviousLinkID != InvalidID<LinkID>())
					{
						DeactivateLink(*data.mNodeGraph, data.mPreviousLinkID);
					}

					ActivateLink(*data.mNodeGraph, data.mCreatedLinkID);
				};

			auto undoCommandFunction = [](const CreateLinkData& data) -> void
				{
					DeactivateLink(*data.mNodeGraph, data.mCreatedLinkID);
					if (data.mPreviousLinkID != InvalidID<LinkID>())
					{
						ActivateLink(*data.mNodeGraph, data.mPreviousLinkID);
					}
				};

			if (!commandTracker)
			{
				doCommandFunction(data);
			}
			else
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Create Link"));
			}

			return data.mCreatedLinkID;
		}

		void DestroyLink(NodeGraph& nodeGraph, const LinkID linkID, CommandTracker* const commandTracker)
		{
			assert(linkID != InvalidID<LinkID>());

			struct DestroyLinkData final
			{
				LinkID mDestroyedLinkID;
				NodeGraph* mNodeGraph = nullptr;
			} data;

			data.mDestroyedLinkID = linkID;
			data.mNodeGraph = &nodeGraph;

			auto doCommandFunction = [](const DestroyLinkData& data) -> void
				{
					DeactivateLink(*data.mNodeGraph, data.mDestroyedLinkID);
				};

			auto undoCommandFunction = [](const DestroyLinkData& data) -> void
				{
					ActivateLink(*data.mNodeGraph, data.mDestroyedLinkID);
				};

			if (!commandTracker)
			{
				doCommandFunction(data);
			}
			else
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Destroy Link"));
			}
		}

		void DestroyLinksByPin(NodeGraph& nodeGraph, const PinID pinID, CommandTracker* const commandTracker)
		{
			const Pin& pin = nodeGraph.GetPin(pinID);

			if (commandTracker)
			{
				commandTracker->BeginComposite("Destroy Links By Pin");
			}


			for (const PinID connectedPinID : pin.GetConnectedPinIDs())
			{
				const LinkID linkID = GetLinkIDByPinIDs(nodeGraph, pinID, connectedPinID);
				DestroyLink(nodeGraph, linkID, commandTracker);
			}


			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
		}

		void UpdateStructBreakNode(GenericDataTypeID dataTypeID)
		{
			const DataType* dataType = GetDataTypeByID(dataTypeID);
			if (!dataType)
			{
				assert(false);
				return;
			}
			NodeType& nodeType = GetNodeType(dataType->GetBreakerNodeTypeID());
			nodeType.GetOutputPinTypeIDs().clear();

			for (const Variable& variable : dataType->GetVariableContainer())
			{
				AddPinTypeToNodeType(dataType->GetBreakerNodeTypeID(), dataTypeID, eIODirection::Output, variable.Name());
			}
		}

		VarID CreateVariable(const GenericDataTypeID parentGenericDataTypeID, const GenericDataTypeID dataTypeID, std::string name, CommandTracker* const commandTracker)
		{
			const DataTypeID* parentDataTypeID = std::get_if<DataTypeID>(&parentGenericDataTypeID.mID);
			if (!parentDataTypeID)
			{
				assert(false);
				return VarID{};
			}
			VariableContainer& variableContainer = GetDataTypeManager().Find(*parentDataTypeID)->GetVariableContainer();
			const VarID varID{ variableContainer.GetVariableCount() };

			variableContainer.AddVariable(Variable{});

			if (commandTracker)
			{
				commandTracker->BeginComposite("Create Variable");
			}

			SetVariableName(varID, variableContainer, std::move(name), commandTracker);
			SetVariableDataType(varID, variableContainer, dataTypeID, commandTracker);
			UpdateStructBreakNode(parentGenericDataTypeID);

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}

			/*for (auto& classInstance : flyClass.mClassInstances)
			{
				classInstance->mVariableContainerInstance.Mirror();
			}*/



			return varID;
		}

		VarID CreateVariable(VariableContainer& variableContainer, const GenericDataTypeID dataTypeID, std::string name, CommandTracker* const commandTracker)
		{
			const VarID varID{ variableContainer.GetVariableCount() };

			variableContainer.AddVariable(Variable{});

			if (commandTracker)
			{
				commandTracker->BeginComposite("Create Variable");
			}

			SetVariableName(varID, variableContainer, std::move(name), commandTracker);
			SetVariableDataType(varID, variableContainer, dataTypeID, commandTracker);

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}

			/*for (auto& classInstance : flyClass.mClassInstances)
			{
				classInstance->mVariableContainerInstance.Mirror();
			}*/



			return varID;
		}

		void SetVariableDataType(const VarID varID, VariableContainer& variableContainer, const GenericDataTypeID dataTypeID, CommandTracker* const commandTracker)
		{
			Variable& variable = variableContainer.GetVariable(varID);

			void* const defaultValueDataPtr = GetDataTypeManager().AllocateData(dataTypeID, variableContainer.GetMemoryArena());

			variable.SetDataTypeID(dataTypeID);
			variable.SetDefaultValueDataPtr(defaultValueDataPtr);

			commandTracker;

			//DestroyVariableNodes(varID, flyClass, commandTracker);
		}

		void SetVariableName(const VarID varID, VariableContainer& variableContainer, std::string name, CommandTracker* const commandTracker)
		{
			struct SetVariableNameData
			{
				std::string mNewName;
				std::string mOldName;
				VarID mVarID = InvalidID<VarID>();
				VariableContainer* mVariableContainer = nullptr;
			} data;

			data.mOldName = variableContainer.GetVariable(varID).Name();
			data.mNewName = std::move(name);
			data.mVarID = varID;
			data.mVariableContainer = &variableContainer;

			auto doCommandFunction = [](const SetVariableNameData& data) -> void
				{
					data.mVariableContainer->GetVariable(data.mVarID).Name() = data.mNewName;
				};

			auto undoCommandFunction = [](const SetVariableNameData& data) -> void
				{
					data.mVariableContainer->GetVariable(data.mVarID).Name() = data.mOldName;
				};


			if (commandTracker)
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Set Variable Name"));
			}
			else
			{
				doCommandFunction(data);
			}
		}

		void DestroyVariable(const VarID varID, VariableContainer& variableContainer, CommandTracker* const commandTracker)
		{
			if (commandTracker)
			{
				commandTracker->BeginComposite("Destroy Variable + Connected Nodes");
			}

			struct DestroyVariableData
			{
				VarID mVarID = InvalidID<VarID>();
				VariableContainer* mVariableContainer = nullptr;
			} data;

			data.mVarID = varID;
			data.mVariableContainer = &variableContainer;

			auto doCommandFunction = [](const DestroyVariableData& data) -> void
				{
					data.mVariableContainer->GetVariable(data.mVarID).SetIsDestroyed(true);
				};


			auto undoCommandFunction = [](const DestroyVariableData& data) -> void
				{
					data.mVariableContainer->GetVariable(data.mVarID).SetIsDestroyed(false);
				};

			if (!commandTracker)
			{
				doCommandFunction(data);
			}
			else
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Destroy Variable"));
			}

			//DestroyNodes(GetNodeRefsByVariableRef(VariableRef(varID, flyClass)), commandTracker);

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
		}

		/*void DestroyVariableNodes(const VarID varID, VariableContainer& variableContainer, CommandTracker* const commandTracker)
		{
			DestroyNodes(GetNodeRefsByVariableRef(VariableRef(varID, flyClass)), commandTracker);
		}*/

		void ViewAndEditVariableDefaultValue(const VarID varID, VariableContainer& variableContainer, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			varID; variableContainer;
			Variable& variable = variableContainer.GetVariable(varID);

			ViewAndEditResult r = GetDataTypeManager().ViewAndEditData(variable.GetDataTypeID(), variable.GetDefaultValueDataPtr());
			r;
		}

		void ViewAndEditClassInstanceVariableDefaultValue(ClassInstance& flyClassInstance, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			flyClassInstance;
			/*const std::vector<Variable>& variables = flyClassInstance.mVariableContainerInstance.mVariableContainer->mVariables;
			std::vector<VariableInstance>& variableInstances = flyClassInstance.mVariableContainerInstance.mVariableInstances;

			assert(variables.size() == variableInstances.size());

			for (size_t i = 0; i < variableInstances.size(); ++i)
			{
				const Variable& variable = variables[i];
				VariableInstance& variableInstance = variableInstances[i];
				GetDataTypeManager().ViewAndEditData(variable.mDataTypeID, variableInstance.mDefaultValueDataPtr);
			}*/
		}

		void BindVariable(Class& flyClass, const NodeRef& nodeRef, const VarID varID, CommandTracker* const commandTracker)
		{
			const GlobalNodeRef gNodeRef = CreateGlobalNodeRef(nodeRef, flyClass);
			struct BindVarData
			{
				GlobalNodeRef mNodeRef;
				VariableRef mVarRef;
			} data;

			data.mNodeRef = gNodeRef;
			data.mVarRef = VariableRef(varID, flyClass);

			auto doCommandFunction = [](const BindVarData& data) -> void
				{
					GetFoundation().mNodeRefToVarRef[data.mNodeRef] = data.mVarRef;
				};

			auto undoCommandFunction = [](const BindVarData& data) -> void
				{
					GetFoundation().mNodeRefToVarRef.erase(data.mNodeRef);
				};

			if (!commandTracker)
			{
				doCommandFunction(data);
			}
			else
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Bind Node To Variable"));
			}
		}

		void UnbindVariable(Class& flyClass, const NodeRef& nodeRef, CommandTracker* commandTracker)
		{
			const GlobalNodeRef gNodeRef = CreateGlobalNodeRef(nodeRef, flyClass);
			if (!GetFoundation().mNodeRefToVarRef.contains(gNodeRef))
			{
				return;
			}

			struct UnbindVarData
			{
				GlobalNodeRef mNodeRef;
				VariableRef mVarRef;
			} data;

			data.mNodeRef = gNodeRef;
			data.mVarRef = GetVariableRefByNodeRef(gNodeRef);

			auto doCommandFunction = [](const UnbindVarData& data) -> void
				{
					GetFoundation().mNodeRefToVarRef.erase(data.mNodeRef);
				};

			auto undoCommandFunction = [](const UnbindVarData& data) -> void
				{
					GetFoundation().mNodeRefToVarRef[data.mNodeRef] = data.mVarRef;
				};

			if (!commandTracker)
			{
				doCommandFunction(data);
			}
			else
			{
				commandTracker->DoCommand(Command(data, doCommandFunction, undoCommandFunction, "Unbind Variable"));
			}
		}

		void SetPinTypeName(const PinTypeID pinTypeID, std::string name)
		{
			PinType& pinType = GetPinTypeManager().GetPinType(pinTypeID);

			pinType.SetName(std::move(name));
		}

		PinTypeID AddPinTypeToNodeType(const NodeTypeID nodeTypeID, const GenericDataTypeID dataTypeID, const eIODirection ioDirection, std::string pinName)
		{
			NodeType& nodeType = GetNodeType(nodeTypeID);

			const PinTypeID createdPinTypeID = CreatePinType(pinName, ioDirection, dataTypeID);

			std::vector<PinTypeID>& pinTypeIDs = SelectByIODirection(ioDirection, nodeType.GetInputPinTypeIDs(), nodeType.GetOutputPinTypeIDs());
			pinTypeIDs.push_back(createdPinTypeID);

			for (const NodeRef& nodeRef : nodeType.GetNodeRefs())
			{
				Node& node = GetNode(nodeRef.GetNodeID(), nodeRef.GetNodeGraph());

				std::vector<PinID>& pinIDs = SelectByIODirection(ioDirection, node.GetInputPins(), node.GetOutputPins());
				const PinID createdPinID = CreatePin(nodeRef.GetNodeGraph(), nodeRef.GetNodeID(), createdPinTypeID, CreateNodeCreationContext());

				pinIDs.push_back(createdPinID);
			}



			return createdPinTypeID;
		}


		static void SetPinAtIndexNodeType(const NodeTypeID nodeTypeID, const std::size_t index, const GenericDataTypeID dataTypeID, const eIODirection ioDirection)
		{
			NodeTypeManager& nodeTypeManager = GetNodeTypeManager();
			PinTypeManager& pinTypeManager = GetPinTypeManager();

			NodeType& nodeType = nodeTypeManager.GetNodeType(nodeTypeID);
			std::vector<PinTypeID>& pinTypeIDs = SelectByIODirection(ioDirection, nodeType.GetInputPinTypeIDs(), nodeType.GetOutputPinTypeIDs());
			const PinTypeID oldPinTypeID = pinTypeIDs[index];
			const PinType& oldPinType = pinTypeManager.GetPinType(oldPinTypeID);

			const PinTypeID newPinTypeID = CreatePinType(oldPinType.GetName(), ioDirection, dataTypeID);

			pinTypeIDs[index] = newPinTypeID;

			const std::vector<NodeRef>& nodeRefs = nodeType.GetNodeRefs();
			for (const NodeRef& nodeRef : nodeRefs)
			{
				Node& node = nodeRef.GetNodeGraph().GetNode(nodeRef.GetNodeID());

				std::vector<PinID>& pinIDs = SelectByIODirection(ioDirection, node.GetInputPins(), node.GetOutputPins());
				const PinID createdPinID = CreatePin(nodeRef.GetNodeGraph(), nodeRef.GetNodeID(), newPinTypeID, CreateNodeCreationContext());
				pinIDs[index] = createdPinID;
			}
		}

		static void DeletePinAtIndexNodeType(const NodeTypeID nodeTypeID, const std::size_t index, const eIODirection ioDirection)
		{
			NodeType& nodeType = GetNodeTypeManager().GetNodeType(nodeTypeID);

			std::vector<PinTypeID>& pinTypeIDs = SelectByIODirection(ioDirection, nodeType.GetInputPinTypeIDs(), nodeType.GetOutputPinTypeIDs());

			assert(index < pinTypeIDs.size());

			pinTypeIDs.erase(pinTypeIDs.begin() + index);

			for (const NodeRef& nodeRef : nodeType.GetNodeRefs())
			{
				Node& node = nodeRef.GetNodeGraph().GetNode(nodeRef.GetNodeID());

				std::vector<PinID>& pinIDs = SelectByIODirection(ioDirection, node.GetInputPins(), node.GetOutputPins());
				DestroyLinksByPin(nodeRef.GetNodeGraph(), pinIDs.at(index), nullptr);
				pinIDs.erase(pinIDs.begin() + index);
			}
		}

		void SetCustomEventName(const CustomEventID customEventID, std::string name, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			const CustomEvent& customEvent = GetNodeTypeManager().GetCustomEvent(customEventID);
			NodeType& executorNodeType = GetNodeTypeManager().GetNodeType(customEvent.GetExecutorTypeID());
			NodeType& callerNodeType = GetNodeTypeManager().GetNodeType(customEvent.GetCallerTypeID());

			const std::string nameDirectory = GetNodeTypeManager().GetNameDirectory(customEvent.GetExecutorTypeID());
			executorNodeType.SetName(nameDirectory + name);
			callerNodeType.SetName(nameDirectory + "Call " + name);
		}

		void AddPinTypeToCustomEvent(const CustomEventID customEventID, const GenericDataTypeID dataTypeID, const std::string pinName, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			const CustomEvent& customEvent = GetNodeTypeManager().GetCustomEvent(customEventID);

			AddPinTypeToNodeType(customEvent.GetExecutorTypeID(), dataTypeID, eIODirection::Output, pinName);
			AddPinTypeToNodeType(customEvent.GetCallerTypeID(), dataTypeID, eIODirection::Input, pinName);
		}

		void SetPinDataTypeAtIndexCustomEvent(const CustomEventID customEventID, const GenericDataTypeID dataTypeID, const size_t index, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			if (index == 0)
			{
				return;
			}
			const CustomEvent& customEvent = GetNodeTypeManager().GetCustomEvent(customEventID);

			SetPinAtIndexNodeType(customEvent.GetExecutorTypeID(), index, dataTypeID, eIODirection::Output);
			SetPinAtIndexNodeType(customEvent.GetCallerTypeID(), index, dataTypeID, eIODirection::Input);
		}

		void SetPinNameAtIndexCustomEvent(const CustomEventID customEventID, std::string name, const size_t index, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			if (index == 0)
			{
				return;
			}
			const NodeTypeManager& nodeTypeManager = GetNodeTypeManager();
			const CustomEvent& customEvent = nodeTypeManager.GetCustomEvent(customEventID);

			{
				const NodeType& callerNodeType = nodeTypeManager.GetNodeType(customEvent.GetCallerTypeID());

				SetPinTypeName(callerNodeType.GetInputPinTypeIDs()[index], name);
			}

			{
				const NodeType& executorNodeType = nodeTypeManager.GetNodeType(customEvent.GetExecutorTypeID());

				SetPinTypeName(executorNodeType.GetOutputPinTypeIDs()[index], name);
			}
		}

		void DeletePinAtIndexCustomEvent(const CustomEventID customEventID, const std::size_t index, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			if (index == 0)
			{
				return;
			}

			const CustomEvent& customEvent = GetNodeTypeManager().GetCustomEvent(customEventID);

			DeletePinAtIndexNodeType(customEvent.GetCallerTypeID(), index, eIODirection::Input);
			DeletePinAtIndexNodeType(customEvent.GetExecutorTypeID(), index, eIODirection::Output);
		}

		void AddPinTypeToFunction(const FunctionID functionID, const GenericDataTypeID dataTypeID, const eIODirection ioDirection, std::string pinName, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			const NodeTypeManager& nodeTypeManager = GetNodeTypeManager();
			const Function& function = nodeTypeManager.GetFunction(functionID);


			AddPinTypeToNodeType(function.GetCallerNodeTypeID(), dataTypeID, ioDirection, pinName);

			const NodeTypeID inputOutputNodeTypeID = SelectByIODirection(ioDirection, function.GetInputNodeTypeID(), function.GetOutputNodeTypeID());
			AddPinTypeToNodeType(inputOutputNodeTypeID, dataTypeID, InvertIODirection(ioDirection), pinName);
		}

		void SetPinDataTypeAtIndexFunction(const FunctionID functionID, const GenericDataTypeID dataTypeID, const size_t index, const eIODirection ioDirection, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			const Function& function = GetNodeTypeManager().GetFunction(functionID);

			SetPinAtIndexNodeType(function.GetCallerNodeTypeID(), index, dataTypeID, ioDirection);

			const NodeTypeID inputOutputNodeTypeID = SelectByIODirection(ioDirection, function.GetInputNodeTypeID(), function.GetOutputNodeTypeID());
			SetPinAtIndexNodeType(inputOutputNodeTypeID, index, dataTypeID, InvertIODirection(ioDirection));
		}

		void SetPinNameAtIndexFunction(const FunctionID functionID, std::string name, const size_t index, const eIODirection ioDirection, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			if (index == 0)
			{
				return;
			}
			const NodeTypeManager& nodeTypeManager = GetNodeTypeManager();
			const Function& function = nodeTypeManager.GetFunction(functionID);

			{
				const NodeType& callerNodeType = nodeTypeManager.GetNodeType(function.GetCallerNodeTypeID());

				const std::vector<PinTypeID>& callerPinTypeIDs = SelectByIODirection(ioDirection, callerNodeType.GetInputPinTypeIDs(), callerNodeType.GetOutputPinTypeIDs());
				SetPinTypeName(callerPinTypeIDs.at(index), name);
			}

			{
				const NodeType& inputNodeType = nodeTypeManager.GetNodeType(function.GetInputNodeTypeID());
				const NodeType& outputNodeType = nodeTypeManager.GetNodeType(function.GetOutputNodeTypeID());

				const std::vector<PinTypeID>& inputOutputPinTypeIDs = SelectByIODirection(ioDirection, inputNodeType.GetOutputPinTypeIDs(), outputNodeType.GetInputPinTypeIDs());
				SetPinTypeName(inputOutputPinTypeIDs.at(index), name);
			}
		}

		void DeletePinAtIndexFunction(const FunctionID functionID, const std::size_t index, const eIODirection ioDirection, [[maybe_unused]] CommandTracker* const commandTracker)
		{
			const Function& function = GetNodeTypeManager().GetFunction(functionID);

			DeletePinAtIndexNodeType(function.GetCallerNodeTypeID(), index, ioDirection);

			const NodeTypeID inputOutputNodeTypeID = SelectByIODirection(ioDirection, function.GetInputNodeTypeID(), function.GetOutputNodeTypeID());
			DeletePinAtIndexNodeType(inputOutputNodeTypeID, index, InvertIODirection(ioDirection));
		}

		void ReplaceNode(NodeGraph& nodeGraph, const NodeID replacedNodeID, const DataTypeID dataTypeID, CommandTracker* const commandTracker)
		{
			const NodeType& replacedNodeType = GetNodeType(replacedNodeID, nodeGraph);

			if (replacedNodeType.GetTraitID() == InvalidID<TraitID>())
			{
				return;
			}

			//const DataTypeID* dataTypeID = std::get_if<DataTypeID>(&dataTypeID.mID);

			/*if (!dataTypeID)
			{
				return;
			}*/

			const bool canReplace = GetNodeTypeManager().CanCreateNodeByTrait(replacedNodeType.GetTraitID(), dataTypeID);
			if (!canReplace)
			{
				return;
			}

			if (commandTracker)
			{
				commandTracker->BeginComposite("Replace node composite");
			}

			const NodeID createdNodeID = CreateTraitNode(nodeGraph, replacedNodeType.GetTraitID(), dataTypeID, commandTracker);
			const Node& replacedNode = GetNode(replacedNodeID, nodeGraph);
			SetNodePosition(createdNodeID, replacedNode.GetPosition(), nodeGraph, commandTracker);
			DestroyNode(nodeGraph, replacedNodeID, commandTracker);

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
		}

		void ReplaceTemplateNodeWithLink(NodeGraph& nodeGraph, const PinID wildcardPinID, const PinID connectedPinID, CommandTracker* const commandTracker)
		{
			const Pin& wildcardPin = GetPin(wildcardPinID, nodeGraph);
			const Pin& connectedPin = GetPin(connectedPinID, nodeGraph);

			const NodeID wildcardNodeID = wildcardPin.GetNodeID();

			const NodeType& wildcardNodeType = GetNodeTypeManager().GetNodeType(nodeGraph.GetNode(wildcardNodeID).GetTypeID());

			if (wildcardNodeType.GetOperatorType() == eNodeOperatorType::None)
			{
				return;
			}


			const PinType& wildcardPinType = GetPinType(wildcardPin.GetTypeID());
			const PinType& connectedPinType = GetPinType(connectedPin.GetTypeID());
			const GenericDataTypeID dtID = connectedPinType.GetDataTypeID();
			const DataTypeID* dataTypeID = std::get_if<DataTypeID>(&dtID.mID);

			if (!dataTypeID)
			{
				return;
			}

			const bool canReplace = GetNodeTypeManager().CanCreateOperatorNode(wildcardNodeType.GetOperatorType(), *dataTypeID);
			if (!canReplace)
			{
				return;
			}

			if (commandTracker)
			{
				commandTracker->BeginComposite("Replace node composite");
			}

			const NodeID createdNodeID = CreateOperatorNode(nodeGraph, wildcardNodeType.GetOperatorType(), *dataTypeID, commandTracker);

			DestroyNode(nodeGraph, wildcardNodeID, commandTracker);

			Node& createdNode = GetNode(createdNodeID, nodeGraph);
			Node& replacedNode = GetNode(wildcardNodeID, nodeGraph);
			SetNodePosition(createdNodeID, replacedNode.GetPosition(), nodeGraph, nullptr);

			{ // Link new pin
				const size_t pinIndex = GetPinIndex(nodeGraph, wildcardPinID);

				const PinID createdPinConnectedID = SelectByIODirection(wildcardPinType.GetIODirection(), createdNode.GetInputPins()[pinIndex], createdNode.GetOutputPins()[pinIndex]);

				TryCreateLink(nodeGraph, connectedPinID, createdPinConnectedID, commandTracker);
			}

			{ // Link previously linked pins


				for (size_t pinIndex = 0; pinIndex < replacedNode.GetInputPins().size(); ++pinIndex)
				{
					const Pin& destroyedInputPin = nodeGraph.GetPin(replacedNode.GetInputPins()[pinIndex]);

					if (!destroyedInputPin.GetConnectedPinIDs().empty())
					{
						TryCreateLink(nodeGraph, destroyedInputPin.GetConnectedPinIDs()[0], GetPinID(nodeGraph, createdNodeID, pinIndex, eIODirection::Input), commandTracker);
					}

				}

				for (size_t pinIndex = 0; pinIndex < replacedNode.GetOutputPins().size(); ++pinIndex)
				{
					const Pin& destroyedOutputPin = nodeGraph.GetPin(replacedNode.GetOutputPins()[pinIndex]);

					for (const PinID connectedInputPinID : destroyedOutputPin.GetConnectedPinIDs())
					{
						if (connectedInputPinID != InvalidID<PinID>())
						{
							TryCreateLink(nodeGraph, connectedInputPinID, GetPinID(nodeGraph, createdNodeID, pinIndex, eIODirection::Output), commandTracker);
						}
					}
				}
			}

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}

		}

		void ReplaceTemplateNode(NodeGraph& nodeGraph, const NodeID replacedNodeID, const DataTypeID dataTypeID, CommandTracker* const commandTracker)
		{
			const NodeType& replacedNodeType = GetNodeTypeManager().GetNodeType(nodeGraph.GetNode(replacedNodeID).GetTypeID());

			if (replacedNodeType.GetOperatorType() == eNodeOperatorType::None)
			{
				return;
			}

			//const DataTypeID* dataTypeID = std::get_if<DataTypeID>(&dataTypeID.mID);

			/*if (!dataTypeID)
			{
				return;
			}*/

			const bool canReplace = GetNodeTypeManager().CanCreateOperatorNode(replacedNodeType.GetOperatorType(), dataTypeID);
			if (!canReplace)
			{
				return;
			}

			if (commandTracker)
			{
				commandTracker->BeginComposite("Replace node composite");
			}

			const NodeID createdNodeID = CreateOperatorNode(nodeGraph, replacedNodeType.GetOperatorType(), dataTypeID, commandTracker);
			const Node& replacedNode = GetNode(replacedNodeID, nodeGraph);
			SetNodePosition(createdNodeID, replacedNode.GetPosition(), nodeGraph, commandTracker);
			DestroyNode(nodeGraph, replacedNodeID, commandTracker);

			if (commandTracker)
			{
				commandTracker->EndComposite();
			}
		}

		static std::vector<PinID> GetInputPins(const NodeGraph& nodeGraph, const bool includeDestroyed, const PinTypeManager& pinTypeManager)
		{
			std::vector<PinID> pinIDs;
			pinIDs.reserve(nodeGraph.GetPinCount());

			for (PinID i{ 0 }; i < nodeGraph.GetPinCount(); i++)
			{
				const Pin& pin = nodeGraph.GetPin(i);
				if (pinTypeManager.GetPinType(pin.GetTypeID()).GetIODirection() != eIODirection::Input)
				{
					continue;
				}

				if (!includeDestroyed && nodeGraph.GetNode(pin.GetNodeID()).IsDestroyed())
				{
					continue;
				}

				pinIDs.push_back(i);
			}
			return pinIDs;
		}

		std::vector<PinID> GetInputPins(const NodeGraph& nodeGraph, const bool includeDestroyed)
		{
            return GetInputPins(nodeGraph, includeDestroyed, GetPinTypeManager());
		}

		static std::vector<PinID> GetOutputPins(const NodeGraph& nodeGraph, const bool includeDestroyed, const PinTypeManager& pinTypeManager)
		{
			std::vector<PinID> pinIDs;
			pinIDs.reserve(nodeGraph.GetPinCount());

			for (PinID i{ 0 }; i < nodeGraph.GetPinCount(); i++)
			{
				const Pin& pin = nodeGraph.GetPin(i);
				if (pinTypeManager.GetPinType(pin.GetTypeID()).GetIODirection() != eIODirection::Output)
				{
					continue;
				}

				if (!includeDestroyed && nodeGraph.GetNode(pin.GetNodeID()).IsDestroyed())
				{
					continue;
				}

				pinIDs.push_back(i);
			}
			return pinIDs;
        }

		std::vector<PinID> GetOutputPins(const NodeGraph& nodeGraph, const bool includeDestroyed)
		{
            return GetOutputPins(nodeGraph, includeDestroyed, GetPinTypeManager());
		}

		VariableRef GetVariableRefByNodeRef(const GlobalNodeRef& nodeRef)
		{
			return GetFoundation().GetVariableRefByNodeRef(nodeRef);
		}

		std::vector<GlobalNodeRef> GetNodeRefsByVariableRef(const VariableRef& aVarRef)
		{
			return GetFoundation().GetNodeRefsByVariableRef(aVarRef);
		}

		PinID GetPinID(const NodeGraph& nodeGraph, const NodeID nodeID, const std::size_t pinIndex, const eIODirection direction)
		{
			const Node& node = nodeGraph.GetNode(nodeID);
			const auto& pins = SelectByIODirection(direction, node.GetInputPins(), node.GetOutputPins());

			return pins[pinIndex];
		}

		std::size_t GetPinIndex(const NodeGraph& nodeGraph, const PinID pinID)
		{
			const Pin& pin = nodeGraph.GetPin(pinID);
			const PinType& pinType = GetPinTypeManager().GetPinType(pin.GetTypeID());
			const Node& node = nodeGraph.GetNode(pin.GetNodeID());

			const std::vector<PinID>& pinIDs = SelectByIODirection(pinType.GetIODirection(), node.GetInputPins(), node.GetOutputPins());

			auto it = std::find(begin(pinIDs), end(pinIDs), pinID);
			if (it != end(pinIDs))
			{
				return std::distance(begin(pinIDs), it);
			}

			assert(false);
			return InvalidID<size_t>();
		}

		PinID GetOpposingPinID(const NodeGraph& previousNodeGraph, const PinID previousPinID, const NodeGraph& newNodeGraph, const NodeID nodeID)
		{
			const std::size_t pinIndex = GetPinIndex(previousNodeGraph, previousPinID);
			const PinType& pinType = GetPinType(previousPinID, previousNodeGraph);
			return GetPinID(newNodeGraph, nodeID, pinIndex, pinType.GetIODirection());
		}

		bool AreDataTypesLinkable(const GenericDataTypeID inputDataTypeID, const GenericDataTypeID outputDataTypeID)
		{
			const eDataTypeRelation dataTypeRelation = GetDataTypeManager().GetDataTypeRelation(inputDataTypeID, outputDataTypeID);

			switch (dataTypeRelation)
			{
			case eDataTypeRelation::None:
				return false;
			case eDataTypeRelation::Same:
				return true;
			case eDataTypeRelation::Pointer_Value:
				return true;
			case eDataTypeRelation::Value_Pointer:
				return true;
				break;
			default:
				break;
			}

			return false;
		}

		bool AreDataTypesLinkable(const DataTypeID inputDataTypeID, const DataTypeID aOutputDataTypeID)
		{
			const eDataTypeRelation dataTypeRelation = GetDataTypeManager().GetDataTypeRelation(inputDataTypeID, aOutputDataTypeID);

			switch (dataTypeRelation)
			{
			case eDataTypeRelation::None:
				return false;
			case eDataTypeRelation::Same:
				return true;
			case eDataTypeRelation::Pointer_Value:
				return true;
			case eDataTypeRelation::Value_Pointer:
				return true;
				break;
			default:
				break;
			}

			return false;
		}

		bool ArePinTypesLinkableByDataType(const PinTypeID inputPinTypeID, const PinTypeID aOutputPinTypeID)
		{
			const PinType& inputPinType = GetPinTypeManager().GetPinType(inputPinTypeID);
			const PinType& outputPinType = GetPinTypeManager().GetPinType(aOutputPinTypeID);

			return AreDataTypesLinkable(inputPinType.GetDataTypeID(), outputPinType.GetDataTypeID());
		}

		static bool ArePinsLinkableByDataType(const NodeGraph& nodeGraph, const PinID inputPinID, const PinID outputPinID)
		{
			return ArePinTypesLinkableByDataType(GetPin(inputPinID, nodeGraph).GetTypeID(), GetPin(outputPinID, nodeGraph).GetTypeID());
		}

		Link ArePinsLinkable(const NodeGraph& nodeGraph, const PinID pinID1, const PinID pinID2)
		{
			const Pin& pin1 = nodeGraph.GetPin(pinID1);
			const Pin& pin2 = nodeGraph.GetPin(pinID2);
			const PinType& pinType1 = GetPinTypeManager().GetPinType(pin1.GetTypeID());
			const PinType& pinType2 = GetPinTypeManager().GetPinType(pin2.GetTypeID());

			PinID inputPinID;
			PinID outputPinID;

			switch (pinType1.GetIODirection())
			{
			case eIODirection::Input:
				if (pinType2.GetIODirection() == eIODirection::Output)
				{
					if (ArePinsLinkableByDataType(nodeGraph, pinID1, pinID2))
					{
						inputPinID = pinID1;
						outputPinID = pinID2;
					}
				}
				break;
			case eIODirection::Output:
				if (pinType2.GetIODirection() == eIODirection::Input)
				{
					if (ArePinsLinkableByDataType(nodeGraph, pinID2, pinID1))
					{
						inputPinID = pinID2;
						outputPinID = pinID1;
					}
				}
				break;
			default:
				break;
			}
			return Link(inputPinID, outputPinID);
		}

		LinkID GetLinkIDByPinIDs(const NodeGraph& nodeGraph, const PinID pinID1, const PinID pinID2, bool aIncludeDestroyed)
		{
			for (LinkID id{ 0 }; id < nodeGraph.GetLinkCount(); ++id)
			{
				const Link& link = nodeGraph.GetLink(id);
				if (!aIncludeDestroyed && link.IsDestroyed())
				{
					continue;
				}
				if (link == Link{ pinID1, pinID2 } || link == Link{ pinID2, pinID1 })
				{
					return id;
				}
			}
			return InvalidID<LinkID>();
		}

		std::vector<LinkID> GetLinkIDsByPin(const NodeGraph& nodeGraph, const PinID pinID, bool aIncludeDestroyed)
		{
			std::vector<LinkID> linkIDs;
			const Pin& pin = nodeGraph.GetPin(pinID);

			for (const PinID connectedPinID : pin.GetConnectedPinIDs())
			{
				const LinkID linkID = GetLinkIDByPinIDs(nodeGraph, pinID, connectedPinID, aIncludeDestroyed);
				assert(linkID != InvalidID<LinkID>());

				linkIDs.push_back(linkID);
			}

			return linkIDs;
		}

		std::vector<LinkID> GetLinkIDsByNode(const NodeGraph& nodeGraph, const NodeID nodeID)
		{
			std::vector<LinkID> linkIDs;
			const Node& node = nodeGraph.GetNode(nodeID);

			for (const PinID inputPinID : node.GetInputPins())
			{
				std::vector<LinkID> inputLinks = GetLinkIDsByPin(nodeGraph, inputPinID);
				linkIDs.insert(linkIDs.end(), inputLinks.begin(), inputLinks.end());
			}
			for (const PinID mOutputPinID : node.GetOutputPins())
			{
				std::vector<LinkID> outputLinks = GetLinkIDsByPin(nodeGraph, mOutputPinID);
				linkIDs.insert(linkIDs.end(), outputLinks.begin(), outputLinks.end());
			}


			return linkIDs;
		}
	}
}
