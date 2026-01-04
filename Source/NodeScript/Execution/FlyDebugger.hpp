#pragma once
#include "../FlyDefines.hpp"
#include "FlyInternalExecutionContext.hpp"
#include "Graph/FlyNodeGraphVariant.hpp"

namespace FLY_NAMESPACE
{
	struct NodeBreakpoint
	{
		NodeExecutionData mNodeExecutionData;

		explicit operator bool() const
		{
			return static_cast<bool>(mNodeExecutionData.nodeRef);
		}
	};

	struct LinkRef
	{
		NodeGraphVariantHandle nodeGraphVariantHandle;
		LinkID linkID = InvalidID<LinkID>();
	};

	class Debugger final
	{
	public:
		Debugger() = default;



		void AddBreakpoint(const NodeRef& nodeRef)
		{
			mBreakpoint.mNodeExecutionData.nodeRef = nodeRef;	
		}

		void AddTraversedLink(LinkID linkID, const NodeGraphVariantHandle& nodeGraph);

		void ClearTraversedLinks();

		const std::vector<LinkRef>& GetTraversedLinks() const
		{
			return mTraversedLinks;
		}

	private:

		NodeBreakpoint mBreakpoint;
		std::vector<LinkRef> mTraversedLinks;
	};
}