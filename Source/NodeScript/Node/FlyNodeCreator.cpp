#include "FlyNodeCreator.hpp"

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

	void CopyPinValueFromPin(const InternalExecutionContext& context, const PinID aDestinationPinID, NodeGraph& aDestinationNodeGraph, const PinID aSourcePinID, const NodeGraph& aSourceNodeGraph)
	{
		const Pin& destinationPin = aDestinationNodeGraph.GetPin(aDestinationPinID);

		const PinType& outputPinType = context.mPinTypeManager->GetPinType(destinationPin.GetTypeID());

		outputPinType.GetSetPinValueFromPinFunction().Invoke(SetPinValueFromPinData
			{
				.mWriteToPinNodeGraph = &aDestinationNodeGraph,
				.mReadFromPinNodeGraph = &aSourceNodeGraph,
				.mWriteToPinID = aDestinationPinID,
				.mReadFromPinID = aSourcePinID,
			}, context);
	}
}
