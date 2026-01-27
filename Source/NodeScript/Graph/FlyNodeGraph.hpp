#pragma once
#include "../FlyDefines.hpp"
#include "../Node/FlyNode.hpp"
#include "../Pin/FlyPin.hpp"
#include "../Pin/FlyLink.hpp"
#include "../Memory/FlyMemoryArena.hpp"

namespace FLY_NAMESPACE
{

	constexpr size_t NodeGraphBufferSize = 1024;

	class NodeGraph final
	{
	public:

		NodeGraph() = default;
		~NodeGraph() = default;

		NodeGraph(const NodeGraph&);
		NodeGraph(NodeGraph&&) noexcept = default;

		NodeGraph& operator=(const NodeGraph&);
		NodeGraph& operator=(NodeGraph&&) noexcept = default;


		[[nodiscard]] Node& GetNode(NodeID nodeID)
		{
			return mNodes[nodeID];
		}

		[[nodiscard]] const Node& GetNode(NodeID nodeID) const
		{
			return mNodes[nodeID];
		}
		
		[[nodiscard]] size_t GetNodeCount() const
		{
			return mNodes.size();
		}

		[[nodiscard]] const std::vector<Node>& GetNodes() const
		{
			return mNodes;
		}

		[[nodiscard]] NodeID GetCurrentNodeID() const
		{
			return NodeID{ static_cast<NodeID::value_type>(GetNodeCount()) };
		}

		NodeID AddNode(Node node)
		{
			NodeID id = GetCurrentNodeID();
			mNodes.push_back(std::move(node));
			return id;
		}

		[[nodiscard]] Pin& GetPin(PinID pinID)
		{
			return mPins[pinID];
		}

		[[nodiscard]] const Pin& GetPin(PinID pinID) const
		{
			return mPins[pinID];
		}

		[[nodiscard]] size_t GetPinCount() const
		{
			return mPins.size();
		}

		[[nodiscard]] PinID AddPin(Pin pin)
		{
			PinID id{ static_cast<PinID::value_type>(mPins.size()) };
			mPins.push_back(std::move(pin));
			return id;
		}

		[[nodiscard]] Link& GetLink(LinkID linkID)
		{
			return mLinks[linkID];
		}

		[[nodiscard]] const Link& GetLink(LinkID linkID) const
		{
			return mLinks[linkID];
		}

		[[nodiscard]] const std::vector<Link>& GetLinks() const
		{
			return mLinks;
		}

		[[nodiscard]] size_t GetLinkCount() const
		{
			return mLinks.size();
		}

		[[nodiscard]] LinkID AddLink(Link link)
		{
			const LinkID id{ static_cast<LinkID::value_type>(mLinks.size()) };
			mLinks.push_back(std::move(link));
			return id;
		}

		[[nodiscard]] MemoryArena<NodeGraphBufferSize>& GetMemoryArena()
		{
			return mMemoryArena;
		}

		[[nodiscard]] const MemoryArena<NodeGraphBufferSize>& GetMemoryArena() const
		{
			return mMemoryArena;
		}

	private:


		std::vector<Node> mNodes;
		std::vector<Pin> mPins;
		std::vector<Link> mLinks;

		MemoryArena<NodeGraphBufferSize> mMemoryArena;
	};
}