#include "FlyNodeCreation.hpp"

namespace FLY_NAMESPACE
{

	/*PinSetFunction CreatePinSetFunction()
	{
		return [](const PinSetData& aPinSetData, const InternalExecutionContext& context) -> void
			{
				Pin& pin = aPinSetData.mNodeGraph->mPins[aPinSetData.mID];
				const PinType& pinType = Global::GetPinTypeManager().GetPinType(pin.mTypeID);
#ifdef FLY_DEBUG
				assert(aPinSetData.mDataTypeID == pinType.mDataTypeID);
#endif

				Global::GetDataTypeManager().CopyData(pinType.mDataTypeID, pin.mDataPtr, aPinSetData.mValue);

				if (pinType.mDataTypeID == Flow::mTypeID)
				{
					const Flow& flow = *reinterpret_cast<const Flow*>(aPinSetData.mValue);

					if (flow)
					{
						if (pinType.mFlowType == eFlowType::Output)
						{
							assert(context.mExecutionQueue);
							for (const PinID connectedInputPinID : pin.mConnectedPinIDs)
							{
								const Pin& connectedInputPin = aPinSetData.mNodeGraph->mPins[connectedInputPinID];
								context.mExecutionQueue->Push({ CreateContextualNodeRef(connectedInputPin.mNodeID, *aPinSetData.mNodeGraph), eNodeTriggerReason::Flow});
							}
						}
					}
				}
			};
	}*/

	void CopyPinValueFromPin(const InternalExecutionContext& context, const PinID destinationPinID, NodeGraph& destinationNodeGraph, const PinID sourcePinID, const NodeGraph& sourceNodeGraph)
	{
		const Pin& destinationPin = destinationNodeGraph.GetPin(destinationPinID);

		const PinType& outputPinType = context.pinTypeManager.GetPinType(destinationPin.GetTypeID());

		outputPinType.GetSetPinValueFromPinFunction().Invoke(SetPinValueFromPinData
			{
				.mWriteToPinNodeGraph = &destinationNodeGraph,
				.mReadFromPinNodeGraph = &sourceNodeGraph,
				.mWriteToPinID = destinationPinID,
				.mReadFromPinID = sourcePinID,
			}, context);
	}
}
