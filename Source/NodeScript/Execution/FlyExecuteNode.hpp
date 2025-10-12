#pragma once
#include "../FlyDefines.hpp"
#include "../Utilities/FlyMeta.hpp"
#include "../Node/FlyNodeTypeManager.hpp"
#include "FlyInternalExecutionContext.hpp"
#include "FlyExecutionTypes.hpp"
#include "../SystemTypes/FlyReferenceWrapper.hpp"
#include "../Instance/FlyNodeGraphInstance.hpp"
#include "../Pin/FlyPinTypeManager.hpp"
#include "../Internal/FlyInternal.hpp"

namespace FLY_NAMESPACE
{

	void EvaluateInputValues(const std::vector<PinID>& inputPinIDs, const InternalExecutionContext& anInternalExecutionContext, size_t aStartIndex = 0);


	template<size_t Index = 0, typename TupleType>
	void SetOutputValues(const TupleType& aOutputValues, const std::vector<PinID>& aOutputPinIDs, InternalExecutionContext& context)
	{
		if constexpr (Index < std::tuple_size_v<TupleType>)
		{
			const PinID outputPinID = aOutputPinIDs[Index];
			const Pin& pin = context.mNodeData.mNodeRef.GetNodeGraph().GetPin(outputPinID);
			const PinType& pinType = Internal::GetPinTypeManager().GetPinType(pin.GetTypeID());
			assert(pinType.GetIODirection() == eIODirection::Output);

			const void* dataPtr = &std::get<Index>(aOutputValues);

			SetPinValueData setPinValueData
			{
				.mNodeGraph = &context.mNodeData.mNodeRef.GetNodeGraph(),
				.mReadFromDataPtr = dataPtr,
				.mWriteToPinID = outputPinID,
#ifdef FLY_DEBUG
				.mReadFromDataTypeID = GetDataTypeID<std::tuple_element_t<Index, TupleType>>()
#endif
			};

			pinType.GetSetPinValueFunction().Invoke(setPinValueData, context);

			SetOutputValues<Index + 1>(aOutputValues, aOutputPinIDs, context);
		}
	}

	template<typename... Args>
	std::tuple<ReferenceWrapper<Args>...> CreateInputPack(const std::vector<PinID>& aPinIDs, const InternalExecutionContext& context)
	{
		std::tuple<ReferenceWrapper<Args>...> tuple;

		[&] <std::size_t... Index>(std::index_sequence<Index...>)
		{
			([&]
				{
					Pin& pin = context.mNodeData.mNodeRef.GetNodeGraph().GetPin(aPinIDs[Index]);
					std::get<Index>(tuple) = ReferenceWrapper<Args>(*reinterpret_cast<Args*>(pin.GetDataPtr().Get()));
				}(), ...);
		}(std::make_index_sequence<sizeof...(Args)>{});

		return tuple;
	}

	// Maybe we can use fold expression to call function without std::tuple
	template<typename... InputTypes>
	void CallInternalTest(const auto& aCallable, InternalExecutionContext& context)
	{
		const Node& node = context.mNodeData.mNodeRef.GetNodeGraph().GetNode(context.mNodeData.mNodeRef.GetNodeID());
		[&] <size_t... Index>(std::index_sequence<Index...>)
		{
			aCallable(std::ref(*reinterpret_cast<InputTypes*>(context.mNodeData.mNodeRef.GetNodeGraph().GetPin(node.GetInputPins()[Index]).GetDataPtr().Get()))...);
		}(std::make_index_sequence<sizeof...(InputTypes)>{});
	}

	// For returning void
	template<bool TakesExecutionContext, bool TakesNodeState, bool TakesInternalExecutionContext, typename NodeExecutionContextType, typename NodeStateDataType, typename Callable, typename... OutputTypes, typename... InputTypes>
		requires(sizeof...(OutputTypes) == 0)
	void CallFunction(InternalExecutionContext& context, TypeList<OutputTypes...>, TypeList<InputTypes...>)
	{
		const Node& node = context.mNodeData.mNodeRef.GetNodeGraph().GetNode(context.mNodeData.mNodeRef.GetNodeID());
		const NodeType& nodeType = Internal::GetNodeTypeManager().GetNodeType(node.GetTypeID());

		Callable& callable = context.mFoundationMemoryPool->At<Callable>(nodeType.GetFunctionMemoryID());

		std::tuple<ReferenceWrapper<InputTypes>...> inputTuple = CreateInputPack<InputTypes...>(node.GetInputPins(), context);

		if constexpr (TakesExecutionContext && TakesNodeState)
		{
			NodeState<NodeStateDataType> nodeState
			{
				context.mNodeGraphInstance->GetNodeState<NodeStateDataType>(context.mNodeData.mNodeRef.GetNodeID())
			};

			NodeExecutionContext<NodeExecutionContextType> executionContext
			{
				*reinterpret_cast<const NodeExecutionContextType*>(context.mExecutionContext)
			};
			return std::apply(callable, std::tuple_cat(std::forward_as_tuple(executionContext, nodeState), inputTuple));
		}
		else if constexpr (TakesExecutionContext)
		{
			NodeExecutionContext<NodeExecutionContextType> executionContext
			{
				*reinterpret_cast<const NodeExecutionContextType*>(context.mExecutionContext)
			};
			return std::apply(callable, std::tuple_cat(std::forward_as_tuple(executionContext), inputTuple));
		}
		else if constexpr (TakesNodeState && TakesInternalExecutionContext)
		{
			NodeState<NodeStateDataType> nodeState
			{
				context.mNodeGraphInstance->GetNodeState<NodeStateDataType>(context.mNodeData.mNodeRef.GetNodeID())
			};
			return std::apply(callable, std::tuple_cat(std::forward_as_tuple(&context, nodeState), inputTuple));

		}
		else if constexpr (TakesNodeState)
		{
			NodeState<NodeStateDataType> nodeState
			{
				context.mNodeGraphInstance->GetNodeState<NodeStateDataType>(context.mNodeData.mNodeRef.GetNodeID())
			};
			return std::apply(callable, std::tuple_cat(std::forward_as_tuple(nodeState), inputTuple));
		}
		else if constexpr (TakesInternalExecutionContext)
		{
			std::apply(callable, std::tuple_cat(std::forward_as_tuple(&context), inputTuple));
		}
		else
		{
			CallInternalTest(callable, context);
			//return std::apply(callable, inputTuple);
		}

	}

	// For not returning void
	template<bool TakesExecutionContext, bool TakesNodeState, bool TakesInternalExecutionContext, typename NodeExecutionContextType, typename NodeStateDataType, typename Callable, typename... OutputTypes, typename... InputTypes>
	std::tuple<OutputTypes...> CallFunction(InternalExecutionContext& context, TypeList<OutputTypes...>, TypeList<InputTypes...>)
	{

		const Node& node = context.mNodeData.mNodeRef.GetNodeGraph().GetNode(context.mNodeData.mNodeRef.GetNodeID());
		const NodeType& nodeType = Internal::GetNodeTypeManager().GetNodeType(node.GetTypeID());

		Callable& callable = context.mFoundationMemoryPool->At<Callable>(nodeType.GetFunctionMemoryID());

		std::tuple<ReferenceWrapper<InputTypes>...> inputTuple = CreateInputPack<InputTypes...>(node.GetInputPins(), context);

		if constexpr (TakesExecutionContext && TakesNodeState)
		{
			NodeState<NodeStateDataType> nodeState
			{
				context.mNodeGraphInstance->GetNodeState<NodeStateDataType>(context.mNodeData.mNodeRef.GetNodeID())
			};

			NodeExecutionContext<NodeExecutionContextType> executionContext
			{
				*reinterpret_cast<const NodeExecutionContextType*>(context.mExecutionContext)
			};
			return std::apply(callable, std::tuple_cat(std::forward_as_tuple(executionContext, nodeState), inputTuple));
		}
		else if constexpr (TakesExecutionContext)
		{
			NodeExecutionContext<NodeExecutionContextType> executionContext
			{
				*reinterpret_cast<const NodeExecutionContextType*>(context.mExecutionContext)
			};
			return std::apply(callable, std::tuple_cat(std::forward_as_tuple(executionContext), inputTuple));
		}
		else if constexpr (TakesNodeState && TakesInternalExecutionContext)
		{
			NodeState<NodeStateDataType> nodeState
			{
				context.mNodeGraphInstance->GetNodeState<NodeStateDataType>(context.mNodeData.mNodeRef.GetNodeID())
			};
			return std::apply(callable, std::tuple_cat(std::forward_as_tuple(&context, nodeState), inputTuple));

		}
		else if constexpr (TakesNodeState)
		{
			NodeState<NodeStateDataType> nodeState
			{
				context.mNodeGraphInstance->GetNodeState<NodeStateDataType>(context.mNodeData.mNodeRef.GetNodeID())
			};
			return std::apply(callable, std::tuple_cat(std::forward_as_tuple(nodeState), inputTuple));
		}
		else if constexpr (TakesInternalExecutionContext)
		{
			return std::apply(callable, std::tuple_cat(std::forward_as_tuple(&context), inputTuple));
		}
		else
		{
			return std::apply(callable, inputTuple);
		}
	}

	template<bool TakesExecutionContext, bool TakesNodeState, bool TakesInternalExecutionContext, typename NodeExecutionContextType, typename NodeStateDataType, typename Callable, typename... OutputTypes, typename... InputTypes>
	ExecuteNodeFunction CreateExecuteNodeFunction(TypeList<OutputTypes...>, TypeList<InputTypes...>)
	{

		constexpr bool HasInputs = !EmptyParams<InputTypes...>;

		return [](const NodeExecutionData& aNodeExecutionData, InternalExecutionContext& context) -> void
			{

				[[maybe_unused]] const Node& node = aNodeExecutionData.mNodeRef.GetNodeGraph().GetNode(aNodeExecutionData.mNodeRef.GetNodeID());

				// Evaluate input values
				if constexpr (HasInputs)
				{
					EvaluateInputValues(node.GetSplitInputPins(), context);
				}

				// Set current node data before calling function
				context.mNodeData = aNodeExecutionData;

				// Call function and retrieve output values
				if constexpr (sizeof...(OutputTypes) > 0)
				{
					std::tuple<OutputTypes...> outputValues = CallFunction<TakesExecutionContext, TakesNodeState, TakesInternalExecutionContext,
						NodeExecutionContextType, NodeStateDataType, Callable>(context, TypeList<OutputTypes...>{}, TypeList<InputTypes...>{});

					// Set output of function
					SetOutputValues(std::forward<std::tuple<OutputTypes...>>(std::move(outputValues)), node.GetOutputPins(), context);
				}
				else
				{
					CallFunction<TakesExecutionContext, TakesNodeState, TakesInternalExecutionContext,
						NodeExecutionContextType, NodeStateDataType, Callable>(context, TypeList<OutputTypes...>{}, TypeList<InputTypes...>{});
				}
			};
	}

	template<typename First, typename... Rest>
	auto GetFirstParamAsTuple(const void* aPtr)
	{
		const First& value = *reinterpret_cast<const First*>(aPtr);
		return std::tuple<First>{ value };
	}

	template<typename First, typename... Rest>
	decltype(auto) GetRestAsTuple(const void* aRestTuple)
	{
		return *reinterpret_cast<const std::tuple<Rest...>*>(aRestTuple);
	}

	template<bool TakesInternalExecutionContext, typename Callable, typename... InputTypes>
	FastExecuteNodeFunction CreateFastExecuteNodeFunction(Callable)
	{
		//using OutputType = GetOutputType<Callable>;
		return []([[maybe_unused]] InternalExecutionContext& context, const MemoryPool& aFoundationMemoryPool, const NodeType& aNodeType, const void* aMainInput, const void* inputTuple, void* aOutputValue) -> void
			{
				aMainInput;
				aMainInput;
				inputTuple;
				aOutputValue;
				aFoundationMemoryPool;
				aNodeType;



				auto firstTuple = GetFirstParamAsTuple<InputTypes...>(aMainInput);
				auto restTuple = GetRestAsTuple<InputTypes...>(inputTuple);

				if constexpr (false)
				{

					auto inputTuple = std::tuple_cat(std::make_tuple(&context), firstTuple, restTuple);

					const Callable& callable = aFoundationMemoryPool.At<Callable>(aNodeType.GetFunctionMemoryID());

					using OutputType = decltype(std::apply(callable, inputTuple));
					if constexpr (!std::same_as<OutputType, void>)
					{

						OutputType outputValue = std::apply(callable, inputTuple);


						OutputType& output = *reinterpret_cast<OutputType*>(aOutputValue);
						output = outputValue;
					}
				}
				else
				{
					auto inputTuple = std::tuple_cat(firstTuple, restTuple);

					const Callable& callable = aFoundationMemoryPool.At<Callable>(aNodeType.GetFunctionMemoryID());

					using OutputType = decltype(std::apply(callable, inputTuple));
					if constexpr (!std::same_as<OutputType, void>)
					{

						OutputType outputValue = std::apply(callable, inputTuple);


						OutputType& output = *reinterpret_cast<OutputType*>(aOutputValue);
						output = outputValue;
					}
				}
			};
	}


}