#pragma once
#include <unordered_map>
#include "../FlyDefines.hpp"
#include "../Memory/FlyMemoryArena.hpp"

namespace FLY_NAMESPACE
{

	class NodeGraph;

	class NodeGraphInstance final
	{
	public:

		NodeGraphInstance(const NodeGraph& nodeGraph);

		void Mirror();

		template<typename T>
		T& GetNodeState(NodeID nodeID);

	private:

		const NodeGraph* mNodeGraph = nullptr;

		std::unordered_map<NodeID, void*> mNodeStateMap;
		MemoryArena<1024> mMemoryArena;

	};

	template<typename T>
	inline T& NodeGraphInstance::GetNodeState(NodeID nodeID)
	{
		return *reinterpret_cast<T*>(mNodeStateMap.at(nodeID));
	}
}