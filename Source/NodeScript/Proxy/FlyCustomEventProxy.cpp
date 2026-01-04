#include "FlyCustomEventProxy.hpp"
#include "../Node/FlyNodeTypeManager.hpp"
#include "../Fly.hpp"
#include "../Internal/FlyInternal.hpp"

namespace FLY_NAMESPACE
{

	CustomEventProxy::CustomEventProxy(const CustomEventID id)
		: mCustomEventID(id)
	{
	}

	NodeTypeProxy CustomEventProxy::GetCallerNodeType() const
	{
		return NodeTypeProxy(GetCustomEvent().GetCallerTypeID());
	}

	NodeTypeProxy CustomEventProxy::GetExecutorNodeType() const
	{
		return NodeTypeProxy(GetCustomEvent().GetExecutorTypeID());
	}

	CustomEventID CustomEventProxy::GetID() const
	{
		return mCustomEventID;
	}

	void CustomEventProxy::SetName(std::string name, CommandTracker* commandTracker)
	{
		Internal::SetCustomEventName(GetID(), std::move(name), commandTracker);
	}

	void CustomEventProxy::AddPin(GenericDataTypeProxy dataTypeProxy, std::string name, CommandTracker* commandTracker)
	{
		Internal::AddPinTypeToCustomEvent(GetID(), dataTypeProxy.GetID(), std::move(name), commandTracker);
	}

	void CustomEventProxy::SetPinNameAtIndex(std::string name, size_t index, CommandTracker* commandTracker)
	{
		Internal::SetPinNameAtIndexCustomEvent(GetID(), std::move(name), index, commandTracker);
	} 

	void CustomEventProxy::SetPinDataTypeAtIndex(GenericDataTypeProxy dataTypeProxy, size_t index, CommandTracker* commandTracker)
	{
		Internal::SetPinDataTypeAtIndexCustomEvent(GetID(), dataTypeProxy.GetID(), index, commandTracker);
	}

	void CustomEventProxy::DeletePinAtIndex(size_t index, CommandTracker* commandTracker)
	{
		Internal::DeletePinAtIndexCustomEvent(GetID(), index, commandTracker);
	}

	CustomEventProxy::operator bool() const
	{
		return mCustomEventID != InvalidID<CustomEventID>();
	}

	const CustomEvent& CustomEventProxy::GetCustomEvent() const
	{
		return Internal::GetNodeTypeManager().GetCustomEvent(mCustomEventID);
	}

	bool operator==(const CustomEventProxy& lhs, const CustomEventProxy& rhs)
	{
		return lhs.mCustomEventID == rhs.mCustomEventID;
	}
}
