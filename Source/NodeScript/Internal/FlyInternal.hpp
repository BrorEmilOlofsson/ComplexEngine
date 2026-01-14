#pragma once
#include "../FlyDefines.hpp"
#include "../Pin/FlyPin.hpp"
#include "../Pin/FlyLink.hpp"
#include "../Pin/FlyPinType.hpp"
#include "../Node/FlyNodeTrait.hpp"
#include "../SystemTypes/FlyVec2.hpp"
#include "../Node/FlyNodeRef.hpp"
#include "../DataType/FlyVariableRef.hpp"
#include "../Graph/FlyNodeGraphVariant.hpp"
#include "../Node/FlyNodeDragData.hpp"
#include <string>
#include <variant>
#include <unordered_map>
#include <optional>
#include <span>

namespace FLY_NAMESPACE
{

	class Node;
	class Struct;
	class Class;
	class CommandTracker;
	class EventGraph;
	class NodeType;
	class StructInstance;
	class ClassInstance;
	class VariableContainer;
	class Foundation;
	class NodeExecutor;
	class DataTypeManager;
	class NodeTypeManager;
	class PinTypeManager;
	class TraitManager;
	class DataType;
	class Trait;
	struct NodeCreationContext;
	
	template<size_t> class MemoryArena;
	class MemoryPool;

	namespace Internal
	{


		[[nodiscard]] Foundation& GetFoundation();
		[[nodiscard]] NodeExecutor& GetNodeExecutor();
		[[nodiscard]] DataTypeManager& GetDataTypeManager();
		[[nodiscard]] NodeTypeManager& GetNodeTypeManager();
		[[nodiscard]] PinTypeManager& GetPinTypeManager();
		[[nodiscard]] TraitManager& GetTraitManager();

		[[nodiscard]] bool& IsDebugging();

		[[nodiscard]] MemoryArena<1024>& GetFrameMemoryArena();
		[[nodiscard]] MemoryArena<10000>& GetEditMemoryArena();
		[[nodiscard]] MemoryPool& GetMemoryPool();
		[[nodiscard]] EventGraph& GetNodeGraphCopy();

		[[nodiscard]] NodeGraph& GetNodeGraph(const NodeGraphVariantHandle& nodeGraphVariantHandle);
		[[nodiscard]] const Pin& GetPin(PinID pinID, const NodeGraph& nodeGraph);
		[[nodiscard]] Pin& GetPin(PinID pinID, NodeGraph& nodeGraph);
		[[nodiscard]] const PinType& GetPinType(PinID pinID, const NodeGraph& nodeGraph);
		[[nodiscard]] const PinType& GetPinType(const Pin& pin);
		[[nodiscard]] const PinType& GetPinType(PinTypeID pinTypeID);
		[[nodiscard]] Node& GetNode(NodeID nodeID, NodeGraph& nodeGraph);
		[[nodiscard]] const Node& GetNode(NodeID nodeID, const NodeGraph& nodeGraph);
		[[nodiscard]] const NodeType& GetNodeType(const Node& node);
		[[nodiscard]] const NodeType& GetNodeType(NodeID nodeID, const NodeGraph& nodeGraph);
		[[nodiscard]] NodeType& GetNodeType(NodeTypeID nodeTypeID);
		[[nodiscard]] const DataType* GetDataTypeByID(DataTypeID dataTypeID);
		[[nodiscard]] const DataType* GetDataTypeByID(GenericDataTypeID dataTypeID);
		[[nodiscard]] Class& GetClassByID(ClassID flyClassID);
		[[nodiscard]] Trait& GetTraitByID(TraitID traitID);

		void InitializeSubPins();

		[[nodiscard]] DataTypeID CreateStruct(std::string name);
		//void SetStructName(StructID aStructID, std::string_view name, CommandTracker* commandTracker);

		[[nodiscard]] ClassID CreateClass(GenericDataTypeID targetID, std::string name);
		void SetClassName(ClassID flyClassID, std::string name, CommandTracker* commandTracker);
		[[nodiscard]] ClassInstance& CreateClassInstance(ClassID flyClassID);
		void DestroyClassInstance(ClassInstance& flyClassInstance);

		[[nodiscard]] TraitID CreateTrait(std::string name);
		void CreateTraitImplementation(DataTypeID dataTypeID, TraitID traitID);

		[[nodiscard]] CustomEventID CreateCustomEvent(std::string name);
		[[nodiscard]] FunctionID CreateFunction(std::string name);
		[[nodiscard]] NodeID CreateNode(const NodeGraphVariantHandle& nodeGraphVariant, NodeTypeID nodeTypeID, Vec2 position = Vec2(), CommandTracker* commandTracker = nullptr);
		[[nodiscard]] std::optional<NodeID> CreateNode(const NodeGraphVariantHandle& nodeGraphVariant, std::string name, Vec2 position, bool createIfNameNotFound, CommandTracker* commandTracker);
		[[nodiscard]] NodeID CreateNodeAutoLink(const NodeGraphVariantHandle& nodeGraphVariant, NodeTypeID nodeTypeID, PinID aConnectingPinID, Vec2 position = Vec2(), CommandTracker* commandTracker = nullptr);
		[[nodiscard]] NodeID CreateGetterNode(NodeGraph& nodeGraph, VarID varID, Class& flyClass, DataTypeID dataTypeID, Vec2 position, CommandTracker* commandTracker);
		[[nodiscard]] NodeID CreateSetterNode(NodeGraph& nodeGraph, VarID varID, Class& flyClass, DataTypeID dataTypeID, Vec2 position, CommandTracker* commandTracker);
		[[nodiscard]] NodeID CreateOperatorNode(NodeGraph& nodeGraph, eNodeOperatorType operatorType, DataTypeID dataTypeID, CommandTracker* commandTracker);

		void AddNode(NodeGraph& nodeGraph, Node&& node, NodeID nodeID, CommandTracker* commandTracker);

		void DestroyNode(NodeGraph& nodeGraph, NodeID nodeID, CommandTracker* commandTracker);
		void DestroyNodes(std::span<NodeRef> nodeRefs, CommandTracker* commandTracker);
		void DestroyNodes(std::span<GlobalNodeRef> nodeRefs, CommandTracker* commandTracker);
		void DestroyNodes(std::span<NodeID> nodeIDs, NodeGraph& nodeGraph, CommandTracker* commandTracker);
		void DestroyLinks(std::span<LinkID> linkIDs, NodeGraph& nodeGraph, CommandTracker* commandTracker);
		void DestroySelection(std::span<NodeID> nodeIDs, std::span<LinkID> linkIDs, NodeGraph& nodeGraph, CommandTracker* commandTracker);

		void SetNodePosition(NodeID nodeID, Vec2 position, NodeGraph& nodeGraph, CommandTracker* commandTracker);
		void SetNodePosition(NodeID nodeID, Vec2 position, Vec2 oldPosition, NodeGraph& nodeGraph, CommandTracker* commandTracker);
		void CommitNodeDrag(const std::unordered_map<NodeID, NodeDragData>& nodeDragData, NodeGraph& nodeGraph, CommandTracker* const commandTracker);

		[[nodiscard]] std::vector<PinID> CreateInputPins(NodeGraph& nodeGraph, NodeID nodeID, NodeTypeID nodeTypeID, const NodeCreationContext& creationContext, std::size_t startIndex = 0);
		[[nodiscard]] std::vector<PinID> CreateOutputPins(NodeGraph& nodeGraph, NodeID, NodeTypeID nodeTypeID, const NodeCreationContext& creationContext, std::size_t startIndex);

		[[nodiscard]] PinID CreatePin(NodeGraph& nodeGraph, NodeID nodeID, PinTypeID pinTypeID, const NodeCreationContext& creationContext);
		[[nodiscard]] PinID CreatePin(NodeGraph& nodeGraph, NodeID nodeID, PinTypeID pinTypeID, void* dataPtr, const NodeCreationContext& creationContext);

		void ViewAndEditPinGeneric(PinID pinID, NodeGraph& nodeGraph, CommandTracker* commandTracker);
		//void ViewAndEditPin(PinID pinID, NodeGraph& nodeGraph, CommandTracker* commandTracker);
		void ViewPinGeneric(PinID pinID, const NodeGraph& nodeGraph);
		//void ViewPin(PinID pinID, const NodeGraph& nodeGraph);
		void SplitPin(PinID pinID, NodeGraph& nodeGraph, CommandTracker* commandTracker);
		void RecombinePin(PinID pinID, NodeGraph& nodeGraph, CommandTracker* commandTracker);

		void BeginFrame(CommandTracker* commandTracker);

		bool IsNodeReplacable(NodeGraph& nodeGraph, NodeID nodeID);

		LinkID TryCreateLink(NodeGraph& nodeGraph, PinID pinID1, PinID pinID2, CommandTracker* commandTracker);
		LinkID CreateLinkGeneric(NodeGraph& nodeGraph, PinID inputPinID, PinID outputPinID, CommandTracker* commandTracker);
		LinkID CreateLink(NodeGraph& nodeGraph, PinID inputPinID, PinID outputPinID, CommandTracker* commandTracker);
		void DestroyLink(NodeGraph& nodeGraph, LinkID linkID, CommandTracker* commandTracker);
		void DestroyLinksByPin(NodeGraph& nodeGraph, PinID pinID, CommandTracker* commandTracker);

		VarID CreateVariable(GenericDataTypeID parentDataTypeID, GenericDataTypeID dataTypeID, std::string name, CommandTracker* commandTracker);
		VarID CreateVariable(VariableContainer& variableContainer, GenericDataTypeID dataTypeID, std::string name, CommandTracker* commandTracker);
		void SetVariableDataType(VarID varID, VariableContainer& variableContainer, GenericDataTypeID dataTypeID, CommandTracker* commandTracker);
		void SetVariableName(VarID varID, VariableContainer& variableContainer, std::string name, CommandTracker* commandTracker);
		void DestroyVariable(VarID varID, VariableContainer& variableContainer, CommandTracker* commandTracker);
		//void DestroyVariableNodes(VarID varID, VariableContainer& variableContainer, CommandTracker* commandTracker);
		void ViewAndEditVariableDefaultValue(VarID varID, VariableContainer& variableContainer, CommandTracker* commandTracker);

		void ViewAndEditClassInstanceVariableDefaultValue(ClassInstance& flyClassInstance, CommandTracker* commandTracker);

		void BindVariable(Class& flyClass, const NodeRef& nodeRef, VarID varID, CommandTracker* commandTracker);
		void UnbindVariable(Class& flyClass, const NodeRef& nodeRef, CommandTracker* commandTracker);

		void SetPinTypeName(PinTypeID pinTypeID, std::string name);
		PinTypeID AddPinTypeToNodeType(NodeTypeID nodeTypeID, GenericDataTypeID dataTypeID, eIODirection ioDirection, std::string pinName);

		void SetCustomEventName(CustomEventID customEventID, std::string name, CommandTracker* commandTracker);
		void AddPinTypeToCustomEvent(CustomEventID customEventID, GenericDataTypeID dataTypeID, std::string pinName, CommandTracker* commandTracker);
		void SetPinDataTypeAtIndexCustomEvent(CustomEventID customEventID, GenericDataTypeID dataTypeID, std::size_t index, CommandTracker* commandTracker);
		void SetPinNameAtIndexCustomEvent(CustomEventID customEventID, std::string name, std::size_t index, CommandTracker* commandTracker);
		void DeletePinAtIndexCustomEvent(CustomEventID customEventID, std::size_t index, CommandTracker* commandTracker);

		void AddPinTypeToFunction(FunctionID functionID, GenericDataTypeID dataTypeID, eIODirection ioDirection, std::string pinName, CommandTracker* commandTracker);
		void SetPinDataTypeAtIndexFunction(FunctionID functionID, GenericDataTypeID dataTypeID, std::size_t index, eIODirection ioDirection, CommandTracker* commandTracker);
		void SetPinNameAtIndexFunction(FunctionID functionID, std::string name, std::size_t index, eIODirection ioDirection, CommandTracker* commandTracker);
		void DeletePinAtIndexFunction(FunctionID functionID, std::size_t index, eIODirection ioDirection, CommandTracker* commandTracker);

		void ReplaceNode(NodeGraph& nodeGraph, NodeID nodeID, DataTypeID dataTypeID, CommandTracker* commandTracker);
		void ReplaceTemplateNodeWithLink(NodeGraph& nodeGraph, PinID wildcardPinID, PinID connectedPinID, CommandTracker* commandTracker);
		void ReplaceTemplateNode(NodeGraph& nodeGraph, NodeID nodeID, DataTypeID dataTypeID, CommandTracker* commandTracker);

		[[nodiscard]] std::vector<PinID> GetInputPins(const NodeGraph& nodeGraph, bool includeDestroyed = false);
		[[nodiscard]] std::vector<PinID> GetOutputPins(const NodeGraph& nodeGraph, bool includeDestroyed = false);

		[[nodiscard]] VariableRef GetVariableRefByNodeRef(const GlobalNodeRef& nodeRef);
		[[nodiscard]] std::vector<GlobalNodeRef> GetNodeRefsByVariableRef(const VariableRef& varRef);


		[[nodiscard]] PinID GetPinID(const NodeGraph& nodeGraph, const NodeID nodeID, const std::size_t pinIndex, const eIODirection ioDirection);
		[[nodiscard]] std::size_t GetPinIndex(const NodeGraph& nodeGraph, const PinID pinID);
		[[nodiscard]] PinID GetOpposingPinID(const NodeGraph& previousNodeGraph, const PinID previousPinID, const NodeGraph& newNodeGraph, const NodeID nodeID);

		[[nodiscard]] bool AreDataTypesLinkable(GenericDataTypeID inputDataTypeID, GenericDataTypeID outputDataTypeID);
		[[nodiscard]] bool AreDataTypesLinkable(DataTypeID inputDataTypeID, DataTypeID outputDataTypeID);
		[[nodiscard]] bool ArePinTypesLinkableByDataType(PinTypeID inputPinTypeID, PinTypeID outputPinTypeID);
		[[nodiscard]] Link ArePinsLinkable(const NodeGraph& nodeGraph, PinID pinID1, PinID pinID2);

		[[nodiscard]] LinkID GetLinkIDByPinIDs(const NodeGraph& nodeGraph, const PinID pinID1, const PinID pinID2, bool includeDestroyed = false);
		[[nodiscard]] std::vector<LinkID> GetLinkIDsByPin(const NodeGraph& nodeGraph, const PinID pinID, bool includeDestroyed = false);
		[[nodiscard]] std::vector<LinkID> GetLinkIDsByNode(const NodeGraph& nodeGraph, const NodeID nodeID);

	}



}