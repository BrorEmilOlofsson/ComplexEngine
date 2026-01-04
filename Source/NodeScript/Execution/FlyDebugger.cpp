#include "FlyDebugger.hpp"

namespace FLY_NAMESPACE
{

	void Debugger::AddTraversedLink(LinkID linkID, const NodeGraphVariantHandle& nodeGraph)
	{
		mTraversedLinks.push_back(LinkRef{ .nodeGraphVariantHandle = nodeGraph, .linkID = linkID });
	}

	void Debugger::ClearTraversedLinks()
	{
		mTraversedLinks.clear();
	}
}
