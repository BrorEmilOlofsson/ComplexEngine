#pragma once
#include <tuple>
#include <vector>

#include "../FlyDefines.hpp"
#include "../Utilities/FlyMeta.hpp"
#include "../Node/FlyNodeTypeManager.hpp"
#include "FlyInternalExecutionContext.hpp"
#include "FlyExecutionTypes.hpp"
#include "../SystemTypes/FlyReferenceWrapper.hpp"
#include "../Instance/FlyNodeGraphInstance.hpp"
#include "../Pin/FlyPinTypeManager.hpp"
//#include "../Internal/FlyInternal.hpp"

namespace FLY_NAMESPACE
{

    void EvaluateInputValues(const std::vector<PinID>& inputPinIDs, const InternalExecutionContext& internalExecutionContext, const std::size_t startIndex = 0);


    template<size_t Index = 0, typename TupleType>
    void SetOutputValues(const TupleType& outputValues, const std::vector<PinID>& outputPinIDs, InternalExecutionContext& context)
    {
        if constexpr (Index < std::tuple_size_v<TupleType>)
        {
            const PinID outputPinID = outputPinIDs[Index];
            const Pin& pin = context.nodeData.nodeRef.GetNodeGraph().GetPin(outputPinID);
            const PinType& pinType = context.pinTypeManager.GetPinType(pin.GetTypeID());
            assert(pinType.GetIODirection() == eIODirection::Output);

            const void* dataPtr = &std::get<Index>(outputValues);

            SetPinValueData setPinValueData
            {
                .mNodeGraph = &context.nodeData.nodeRef.GetNodeGraph(),
                .mReadFromDataPtr = dataPtr,
                .mWriteToPinID = outputPinID,
#ifdef FLY_DEBUG
                .mReadFromDataTypeID = GetDataTypeID<std::tuple_element_t<Index, TupleType>>()
#endif
            };

            pinType.GetSetPinValueFunction().Invoke(setPinValueData, context);

            SetOutputValues<Index + 1>(outputValues, outputPinIDs, context);
        }
    }

    template<typename... Args>
    std::tuple<ReferenceWrapper<Args>...> CreateInputPack(const std::vector<PinID>& pinIDs, const InternalExecutionContext& context)
    {
        std::tuple<ReferenceWrapper<Args>...> tuple;

        [&] <std::size_t... Index>(std::index_sequence<Index...>)
        {
            ([&]
                {
                    Pin& pin = context.nodeData.nodeRef.GetNodeGraph().GetPin(pinIDs[Index]);
                    std::get<Index>(tuple) = ReferenceWrapper<Args>(*reinterpret_cast<Args*>(pin.GetDataPtr().Get()));
                }(), ...);
        }(std::make_index_sequence<sizeof...(Args)>{});

        return tuple;
    }

    // Maybe we can use fold expression to call function without std::tuple
    template<typename... InputTypes>
    void CallInternalTest(const auto& callable, InternalExecutionContext& context)
    {
        const Node& node = context.nodeData.nodeRef.GetNodeGraph().GetNode(context.nodeData.nodeRef.GetNodeID());
        [&] <std::size_t... Index>(std::index_sequence<Index...>)
        {
            callable(std::ref(*reinterpret_cast<InputTypes*>(context.nodeData.nodeRef.GetNodeGraph().GetPin(node.GetInputPins()[Index]).GetDataPtr().Get()))...);
        }(std::make_index_sequence<sizeof...(InputTypes)>{});
    }

    // For returning void
    template<bool TakesExecutionContext, bool TakesNodeState, bool TakesInternalExecutionContext, bool IsCPPGenerated, typename NodeExecutionContextType, typename NodeStateDataType, typename Callable, typename... OutputTypes, typename... InputTypes>
        requires(sizeof...(OutputTypes) == 0)
    void CallFunction(InternalExecutionContext& context, TypeList<OutputTypes...>, TypeList<InputTypes...>)
    {
        const Node& node = context.nodeData.nodeRef.GetNodeGraph().GetNode(context.nodeData.nodeRef.GetNodeID());
        const NodeType& nodeType = context.nodeTypeManager.GetNodeType(node.GetTypeID());

        Callable& callable = context.foundationMemoryPool.At<Callable>(nodeType.GetFunctionMemoryID());

        std::tuple<ReferenceWrapper<InputTypes>...> inputTuple = CreateInputPack<InputTypes...>(node.GetInputPins(), context);

        if constexpr (TakesExecutionContext && TakesNodeState)
        {
            NodeState<NodeStateDataType> nodeState
            {
                context.nodeGraphInstance->GetNodeState<NodeStateDataType>(context.nodeData.nodeRef.GetNodeID())
            };

            NodeExecutionContext<NodeExecutionContextType> executionContext
            {
                *reinterpret_cast<const NodeExecutionContextType*>(context.executionContext)
            };
            return std::apply(callable, std::tuple_cat(std::forward_as_tuple(executionContext, nodeState), inputTuple));
        }
        else if constexpr (TakesExecutionContext)
        {
            NodeExecutionContext<NodeExecutionContextType> executionContext
            {
                *reinterpret_cast<const NodeExecutionContextType*>(context.executionContext)
            };
            return std::apply(callable, std::tuple_cat(std::forward_as_tuple(executionContext), inputTuple));
        }
        else if constexpr (TakesNodeState && TakesInternalExecutionContext)
        {
            NodeState<NodeStateDataType> nodeState
            {
                context.nodeGraphInstance->GetNodeState<NodeStateDataType>(context.nodeData.nodeRef.GetNodeID())
            };
            return std::apply(callable, std::tuple_cat(std::forward_as_tuple(&context, nodeState), inputTuple));

        }
        else if constexpr (TakesNodeState)
        {
            NodeState<NodeStateDataType> nodeState
            {
                context.nodeGraphInstance->GetNodeState<NodeStateDataType>(context.nodeData.nodeRef.GetNodeID())
            };
            return std::apply(callable, std::tuple_cat(std::forward_as_tuple(nodeState), inputTuple));
        }
        else if constexpr (TakesInternalExecutionContext)
        {
            std::apply(callable, std::tuple_cat(std::forward_as_tuple(&context), inputTuple));
        }
        else
        {
            if constexpr (IsCPPGenerated)
            {
                const auto& tuple = *reinterpret_cast<const std::tuple<InputTypes...>*>(node.GetInputTuplePtr());
                std::apply(callable, tuple);
            }
            else
            {
                CallInternalTest(callable, context);
                //return std::apply(callable, inputTuple);
            }
        }

    }

    // For not returning void
    template<bool TakesExecutionContext, bool TakesNodeState, bool TakesInternalExecutionContext, bool IsCPPGenerated, typename NodeExecutionContextType, typename NodeStateDataType, typename Callable, typename... OutputTypes, typename... InputTypes>
        requires(sizeof...(OutputTypes) > 0)
    std::tuple<OutputTypes...> CallFunction(InternalExecutionContext& context, TypeList<OutputTypes...>, TypeList<InputTypes...>)
    {

        const Node& node = context.nodeData.nodeRef.GetNodeGraph().GetNode(context.nodeData.nodeRef.GetNodeID());
        const NodeType& nodeType = context.nodeTypeManager.GetNodeType(node.GetTypeID());

        Callable& callable = context.foundationMemoryPool.At<Callable>(nodeType.GetFunctionMemoryID());

        std::tuple<ReferenceWrapper<InputTypes>...> inputTuple = CreateInputPack<InputTypes...>(node.GetInputPins(), context);

        if constexpr (TakesExecutionContext && TakesNodeState)
        {
            NodeState<NodeStateDataType> nodeState
            {
                context.nodeGraphInstance->GetNodeState<NodeStateDataType>(context.nodeData.nodeRef.GetNodeID())
            };

            NodeExecutionContext<NodeExecutionContextType> executionContext
            {
                *reinterpret_cast<const NodeExecutionContextType*>(context.executionContext)
            };
            return std::apply(callable, std::tuple_cat(std::forward_as_tuple(executionContext, nodeState), inputTuple));
        }
        else if constexpr (TakesExecutionContext)
        {
            NodeExecutionContext<NodeExecutionContextType> executionContext
            {
                *reinterpret_cast<const NodeExecutionContextType*>(context.executionContext)
            };
            return std::apply(callable, std::tuple_cat(std::forward_as_tuple(executionContext), inputTuple));
        }
        else if constexpr (TakesNodeState && TakesInternalExecutionContext)
        {
            NodeState<NodeStateDataType> nodeState
            {
                context.nodeGraphInstance->GetNodeState<NodeStateDataType>(context.nodeData.nodeRef.GetNodeID())
            };
            return std::apply(callable, std::tuple_cat(std::forward_as_tuple(&context, nodeState), inputTuple));

        }
        else if constexpr (TakesNodeState)
        {
            NodeState<NodeStateDataType> nodeState
            {
                context.nodeGraphInstance->GetNodeState<NodeStateDataType>(context.nodeData.nodeRef.GetNodeID())
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

    template<bool TakesExecutionContext, bool TakesNodeState, bool TakesInternalExecutionContext, bool IsCPPGenerated, typename NodeExecutionContextType, typename NodeStateDataType, typename Callable, typename... OutputTypes, typename... InputTypes>
    ExecuteNodeFunction CreateExecuteNodeFunction(TypeList<OutputTypes...>, TypeList<InputTypes...>)
    {

        constexpr bool HasInputs = !EmptyParams<InputTypes...>;

        return [](const NodeExecutionData& nodeExecutionData, InternalExecutionContext& context) -> void
            {

                [[maybe_unused]] const Node& node = nodeExecutionData.nodeRef.GetNodeGraph().GetNode(nodeExecutionData.nodeRef.GetNodeID());

                // Evaluate input values
                if constexpr (HasInputs)
                {
                    EvaluateInputValues(node.GetSplitInputPins(), context);
                }

                // Set current node data before calling function
                context.nodeData = nodeExecutionData;

                // Call function and retrieve output values
                if constexpr (sizeof...(OutputTypes) > 0)
                {
                    std::tuple<OutputTypes...> outputValues = CallFunction<TakesExecutionContext, TakesNodeState, TakesInternalExecutionContext, IsCPPGenerated,
                        NodeExecutionContextType, NodeStateDataType, Callable>(context, TypeList<OutputTypes...>{}, TypeList<InputTypes...>{});

                    // Set output of function
                    SetOutputValues(outputValues, node.GetOutputPins(), context);
                }
                else
                {
                    CallFunction<TakesExecutionContext, TakesNodeState, TakesInternalExecutionContext, IsCPPGenerated,
                        NodeExecutionContextType, NodeStateDataType, Callable>(context, TypeList<OutputTypes...>{}, TypeList<InputTypes...>{});
                }
            };
    }

    template<typename First, typename... Rest>
    auto GetFirstParamAsTuple(const void* ptr)
    {
        const First& value = *reinterpret_cast<const First*>(ptr);
        return std::tuple<First>{ value };
    }

    template<typename First, typename... Rest>
    decltype(auto) GetRestAsTuple(const void* restTuple)
    {
        return *reinterpret_cast<const std::tuple<Rest...>*>(restTuple);
    }

    template<bool TakesInternalExecutionContext, typename Callable, typename... InputTypes>
    FastExecuteNodeFunction CreateFastExecuteNodeFunction(Callable)
    {
        //using OutputType = GetOutputType<Callable>;
        return []([[maybe_unused]] InternalExecutionContext& context, const MemoryPool& foundationMemoryPool, const NodeType& nodeType, const void* mainInput, const void* inputTuple, void* outputValue) -> void
            {
                mainInput;
                inputTuple;
                outputValue;
                foundationMemoryPool;
                nodeType;



                auto firstTuple = GetFirstParamAsTuple<InputTypes...>(mainInput);
                auto restTuple = GetRestAsTuple<InputTypes...>(inputTuple);

                if constexpr (false)
                {

                    auto inputTuple = std::tuple_cat(std::make_tuple(&context), firstTuple, restTuple);

                    const Callable& callable = foundationMemoryPool.At<Callable>(nodeType.GetFunctionMemoryID());

                    using OutputType = decltype(std::apply(callable, inputTuple));
                    if constexpr (!std::same_as<OutputType, void>)
                    {

                        OutputType outputValue = std::apply(callable, inputTuple);


                        OutputType& output = *reinterpret_cast<OutputType*>(outputValue);
                        output = outputValue;
                    }
                }
                else
                {
                    auto inputTuple = std::tuple_cat(firstTuple, restTuple);

                    const Callable& callable = foundationMemoryPool.At<Callable>(nodeType.GetFunctionMemoryID());

                    using OutputType = decltype(std::apply(callable, inputTuple));
                    if constexpr (!std::same_as<OutputType, void>)
                    {

                        OutputType outputValue = std::apply(callable, inputTuple);


                        OutputType& output = *reinterpret_cast<OutputType*>(outputValue);
                        output = outputValue;
                    }
                }
            };
    }


}