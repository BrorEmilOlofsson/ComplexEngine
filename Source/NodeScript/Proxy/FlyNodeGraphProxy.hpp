#pragma once
#include "../FlyDefines.hpp"
#include "FlyNodeProxy.hpp"
#include "FlyPinProxy.hpp"
#include "FlyLinkProxy.hpp"
#include "FlyDataTypeProxy.hpp"
#include "FlyGenericDataTypeProxy.hpp"
#include "../Graph/FlyNodeGraphVariant.hpp"
#include "../Node/FlyNodeDragData.hpp"
#include "ProxyIterators/FlyProxyContextIterator.hpp"
#include "ProxyIterators/FlyNodeProxyIterator.hpp"
#include <span>

namespace FLY_NAMESPACE
{

	using PinProxyIterator = ProxyContextIterator<PinID, PinProxy>;
	using PinProxyIteratorService = ProxyContextIteratorService<PinID, PinProxyIterator, PinProxy>;
	using LinkProxyIterator = ProxyContextIterator<LinkID, LinkProxy>;
	using LinkProxyIteratorService = ProxyContextIteratorService<LinkID, LinkProxyIterator>;
	
	class NodeGraph;
	class EventGraph;
	class FunctionProxy;
	class ClassProxy;
	class NodeTypeProxy;
	class VariableProxy;


	enum class eNodeGraphType
	{
		None,
		EventGraph,
		Function
	};

	class NodeGraphProxy final
	{
	public:

		NodeGraphProxy() = default;
		explicit NodeGraphProxy(const FunctionProxy& aFunctionProxy);
		explicit NodeGraphProxy(EventGraph& aEventGraph);
		explicit NodeGraphProxy(NodeGraphVariantHandle&& aNodeGraphVariant);
		explicit NodeGraphProxy(const NodeGraphVariantHandle& aNodeGraphVariant);
		
		[[nodiscard]] NodeProxyIteratorService IterateNodes(Predicate<NodeProxy> filterPredicate) const;
		[[nodiscard]] NodeProxyIteratorService IterateNodes(bool includeDestroyed = false) const;
		[[nodiscard]] LinkProxyIteratorService IterateLinks(bool includeDestroyed = false) const;

		[[nodiscard]] NodeGraph& GetNodeGraph();
		[[nodiscard]] const NodeGraph& GetNodeGraph() const;

		[[nodiscard]] eNodeGraphType GetType() const;

		[[nodiscard]] std::vector<PinProxy> GetNonConnectedInputPins() const;
		[[nodiscard]] std::vector<PinProxy> GetNonConnectedOutputPins() const;
		[[nodiscard]] std::vector<PinProxy> GetNonConnectedPinsByIODirection(eIODirection ioDirection) const;
		[[nodiscard]] std::vector<PinProxy> GetNonConnectedPinsByIODirectionAndDataType(eIODirection ioDirection, GenericDataTypeProxy dataTypeProxy) const;
		[[nodiscard]] std::vector<PinProxy> GetNonConnectedPinsByIODirectionAndRelatedDataTypes(eIODirection ioDirection, GenericDataTypeProxy dataTypeProxy) const;

		NodeProxy CreateNode(const NodeTypeProxy& nodeType, Vec2 position = Vec2(), CommandTracker* commandTracker = nullptr);
		NodeProxy CreateNode(std::string name, Vec2 position = Vec2(), CommandTracker* commandTracker = nullptr, bool createIfNameNotFound = true);
		NodeProxy CreateNodeAutoLink(NodeTypeProxy nodeTypeProxy, PinID connection, Vec2 position = Vec2(), CommandTracker* commandTracker = nullptr);
		//NodeProxy CreateGetterNode(VariableProxy aVariableProxy, Vec2 position = Vec2(), CommandTracker* commandTracker = nullptr);
		//NodeProxy CreateSetterNode(VariableProxy aVariableProxy, Vec2 position = Vec2(), CommandTracker* commandTracker = nullptr);

		void DestroySelection(std::span<NodeID> nodeIDs, std::span<LinkID> linkIDs, CommandTracker* commandTracker);
		LinkProxy TryCreateLink(PinProxy pin1, PinProxy pin2, CommandTracker* commandTracker);

		void CommitNodeDrag(const std::unordered_map<NodeID, NodeDragData>& nodeDragData, CommandTracker* commandTracker);

		void ReplaceTemplateNode(NodeProxy replaceNode, DataTypeProxy dataType, CommandTracker* commandTracker);
		void ReplaceTemplateNode(PinProxy replacePin, DataTypeProxy dataType, CommandTracker* commandTracker);

		[[nodiscard]] const NodeGraphVariantHandle& GetVariant() const;

		friend bool operator==(const NodeGraphProxy& lhs, const NodeGraphProxy& rhs);

		explicit operator bool() const;

	private:

		NodeGraphVariantHandle mNodeGraphVariant;

	};
}

template<>
struct std::hash<FLY_NAMESPACE::NodeGraphProxy>
{
	size_t operator()(const FLY_NAMESPACE::NodeGraphProxy& aNodeGraphProxy) const
	{
		return reinterpret_cast<size_t>(&aNodeGraphProxy.GetNodeGraph());
	}
};