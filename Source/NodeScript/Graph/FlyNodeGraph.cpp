#include "FlyNodeGraph.hpp"

namespace FLY_NAMESPACE
{

	NodeGraph::NodeGraph(const NodeGraph& other)
		: mNodes(other.mNodes)
		, mPins(other.mPins)
		, mMemoryArena(other.mMemoryArena)
	{
		for (Pin& pin : mPins)
		{
			pin.SetDataPtr(mMemoryArena.GetRenewedPointer(pin.GetDataPtr(), other.mMemoryArena));
		}
	}

	NodeGraph& NodeGraph::operator=(const NodeGraph& other)
	{
		mNodes = other.mNodes;
		mPins = other.mPins;
		mMemoryArena = other.mMemoryArena;

		for (Pin& pin : mPins)
		{
			pin.SetDataPtr(mMemoryArena.GetRenewedPointer(pin.GetDataPtr(), other.mMemoryArena));
		}

		return *this;
	}
}