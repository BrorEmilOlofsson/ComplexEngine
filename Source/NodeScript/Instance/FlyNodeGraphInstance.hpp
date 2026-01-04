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

		NodeGraphInstance(const NodeGraph& aNodeGraph);
		~NodeGraphInstance();

		void Mirror();

		template<typename T>
		T& GetNodeState(NodeID aNodeID);

	private:

		const NodeGraph* mNodeGraph;

		std::unordered_map<NodeID, void*> mNodeStateMap;
		MemoryArena<1024> mMemoryArena;

	};

	template<typename T>
	inline T& NodeGraphInstance::GetNodeState(NodeID aNodeID)
	{
		return *reinterpret_cast<T*>(mNodeStateMap.at(aNodeID));
	}
}