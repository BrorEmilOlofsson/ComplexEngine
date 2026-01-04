#pragma once
#include "../FlyDefines.hpp"

namespace FLY_NAMESPACE
{

	class Class;
	class NodeGraph;

	struct NodeGraphRef final
	{
		NodeGraph* mNodeGraph = nullptr;
	};

	[[nodiscard]] constexpr bool operator==(const NodeGraphRef& a, const NodeGraphRef& b)
	{
		return a.mNodeGraph == b.mNodeGraph;
	}

	[[nodiscard]] constexpr bool operator<(const NodeGraphRef& lhs, const NodeGraphRef& rhs)
	{
		return lhs.mNodeGraph < rhs.mNodeGraph;
	}

	struct NodeRef final
	{
		NodeRef() = default;

		NodeRef(NodeID nodeID, NodeGraph& nodeGraph)
			: mNodeGraph(&nodeGraph)
			, mNodeID(nodeID)

		{
		}

		[[nodiscard]] constexpr NodeGraph& GetNodeGraph() const
		{
			return *mNodeGraph;
		}

		[[nodiscard]] constexpr NodeID GetNodeID() const
		{
			return mNodeID;
		}

		friend constexpr bool operator==(const NodeRef& lhs, const NodeRef& rhs);
		friend constexpr bool operator<(const NodeRef& lhs, const NodeRef& rhs);

		[[nodiscard]] constexpr explicit operator bool() const
		{
			return mNodeID != InvalidID<NodeID>();
		}

	private:

		NodeGraph* mNodeGraph = nullptr;
		NodeID mNodeID = InvalidID<NodeID>();
	};


	NodeRef CreateContextualNodeRef(NodeID nodeID, NodeGraph& nodeGraph);

	constexpr bool operator==(const NodeRef& lhs, const NodeRef& rhs)
	{
		return lhs.mNodeGraph == rhs.mNodeGraph && lhs.mNodeID == rhs.mNodeID;
	}

	constexpr bool operator<(const NodeRef& lhs, const NodeRef& rhs)
	{
		if (lhs.mNodeGraph == rhs.mNodeGraph)
		{
			return lhs.mNodeID < rhs.mNodeID;
		}
		return lhs.mNodeGraph < rhs.mNodeGraph;
	}

	struct NodeRefHasher final
	{

#ifdef FLY_DEBUG
		size_t operator()(const NodeRef& nodeRef) const;
#else 
		size_t operator()(const NodeRef& nodeRef) const
		{
			return reinterpret_cast<size_t>(&nodeRef.GetNodeGraph()) + static_cast<size_t>(nodeRef.GetNodeID());
		}
#endif

	};


	class GlobalNodeRef final
	{
	public:

		GlobalNodeRef() = default;
		GlobalNodeRef(NodeID nodeID, NodeGraph& nodeGraph, Class& flyClass)
			: mClass(&flyClass)
			, mNodeGraph(&nodeGraph)
			, mNodeID(nodeID)
		{

		}

		GlobalNodeRef(NodeID nodeID, NodeGraph& nodeGraph)
			: mClass(nullptr)
			, mNodeGraph(&nodeGraph)
			, mNodeID(nodeID)
		{

		}

		constexpr Class& GetClass() const
		{
			return *mClass;
		}

		constexpr NodeGraph& GetNodeGraph() const
		{
			return *mNodeGraph;
		}

		constexpr NodeID GetNodeID() const
		{
			return mNodeID;
		}

	private:
		Class* mClass = nullptr;
		NodeGraph* mNodeGraph = nullptr;
		NodeID mNodeID = InvalidID<NodeID>();
	};

	GlobalNodeRef CreateGlobalNodeRef(const NodeRef& nodeRef, Class& flyClass);
	GlobalNodeRef CreateGlobalNodeRef(NodeID nodeID, NodeGraph& nodeGraph, Class& flyClass);

	[[nodiscard]] constexpr bool operator==(const GlobalNodeRef& lhs, const GlobalNodeRef& rhs)
	{
		return &lhs.GetNodeGraph() == &rhs.GetNodeGraph() && lhs.GetNodeID() == rhs.GetNodeID();
	}

	struct GlobalNodeRefHasher final
	{
		size_t operator()(const GlobalNodeRef& nodeRef) const
		{
			return reinterpret_cast<size_t>(&nodeRef.GetNodeGraph()) + static_cast<size_t>(nodeRef.GetNodeID());
		}

	};

	class GlobalNodeRefConst final
	{
	public:

		GlobalNodeRefConst() = default;
		GlobalNodeRefConst(NodeID nodeID, const NodeGraph& nodeGraph, const Class& flyClass)
			: mClass(&flyClass)
			, mNodeGraph(&nodeGraph)
			, mNodeID(nodeID)
		{

		}

		GlobalNodeRefConst(const GlobalNodeRef& nodeRef)
			: mClass(&nodeRef.GetClass())
			, mNodeGraph(&nodeRef.GetNodeGraph())
			, mNodeID(nodeRef.GetNodeID())
		{

		}

		[[nodiscard]] constexpr const Class& GetClass() const
		{
			return *mClass;
		}

		[[nodiscard]] constexpr const NodeGraph& GetNodeGraph() const
		{
			return *mNodeGraph;
		}

		[[nodiscard]] constexpr NodeID GetNodeID() const
		{
			return mNodeID;
		}

	private:
		const Class* mClass = nullptr;
		const NodeGraph* mNodeGraph = nullptr;
		NodeID mNodeID = InvalidID<NodeID>();
	};

	GlobalNodeRefConst CreateGlobalNodeRef(NodeID nodeID, const NodeGraph& nodeGraph, const Class& flyClass);

	[[nodiscard]] constexpr bool operator==(const GlobalNodeRefConst& lhs, const GlobalNodeRefConst& rhs)
	{
		return &lhs.GetNodeGraph() == &rhs.GetNodeGraph() && lhs.GetNodeID() == rhs.GetNodeID();
	}

	struct GlobalNodeRefConstHasher final
	{
		size_t operator()(const GlobalNodeRef& nodeRef) const
		{
			return reinterpret_cast<size_t>(&nodeRef.GetNodeGraph()) + static_cast<size_t>(nodeRef.GetNodeID());
		}

	};


}