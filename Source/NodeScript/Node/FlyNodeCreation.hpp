#pragma once
#include <span>

#include "../FlyDefines.hpp"
#include "../Pin/FlyIODirection.hpp"
#include "../Execution/FlyExecutionTypes.hpp"
#include "../Execution/FlyInternalExecutionContext.hpp"
#include "../DataType/FlyStruct.hpp"
#include "../Execution/FlyNodeExecutor.hpp"
#include "../Execution/FlyNodeExecutionQueue.hpp"
#include "FlyNode.hpp"
#include "../Pin/FlyPinTypeManager.hpp"
#include "../DataType/FlyDataTypeManager.hpp"
#include "../Internal/FlyInternal.hpp"
#include "../SystemTypes/FlyWildcard.hpp"
#include "../SystemTypes/FlyFlow.hpp"
#include "../Utilities/FlyMeta.hpp"
#include "../Instance/FlyClassInstance.hpp"
#include "../SystemTypes/FlyReferenceWrapper.hpp"
#include "../Execution/FlyExecuteNode.hpp"
#include "../DataType/FlyDataTypeManager.hpp"
#include "../Utilities/FlyUtilities.hpp"

namespace FLY_NAMESPACE
{

    struct NodeCreationData final
    {
        EventID mEventID = InvalidID<EventID>();
        DataTypeID mOwnerDataTypeID;
        std::string mName;
        NodeTypeDesc mDescription;
        TraitID mTraitID;
        eNodeOperatorType mOperatorType = eNodeOperatorType::None;
    };

    using InternalExecutionContextPtr = InternalExecutionContext*;


    void CopyPinValueFromPin(const InternalExecutionContext& context, const PinID destinationPinID, NodeGraph& destinationNodeGraph, const PinID sourcePinID, const NodeGraph& sourceNodeGraph);


    template<typename T>
    void SetPinValue(const InternalExecutionContext& context, const std::vector<PinID>& destination, NodeGraph& destinationNodeGraph, T&& value, const std::size_t index)
    {
        const PinID destinationPinID = destination[index];
        const Pin& destinationPin = destinationNodeGraph.GetPin(destinationPinID);

        const PinType& outputPinType = context.pinTypeManager.GetPinType(destinationPin.GetTypeID());

        outputPinType.GetSetPinValueFunction().Invoke(SetPinValueData
            {
                .mNodeGraph = &destinationNodeGraph,
                .mReadFromDataPtr = &value,
                .mWriteToPinID = destinationPinID,
#ifdef FLY_DEBUG
                .mReadFromDataTypeID = GenericDataTypeID{ GetDataTypeID<std::remove_cvref_t<T>>() }
#endif
            }, context);

    }

    template<typename... Args>
    void SetPinValues(const InternalExecutionContext& context, const std::vector<PinID>& destination, NodeGraph& destinationNodeGraph, Args&&... args)
    {
        size_t index = 0;
        (SetPinValue(context, destination, destinationNodeGraph, std::forward<Args>(args), index++), ...);
    }

    inline void CopyPinData(const InternalExecutionContext& context, const std::vector<PinID>& destination, const std::vector<PinID>& source, NodeGraph& destinationNodeGraph, const NodeGraph& sourceNodeGraph, const size_t startIndex)
    {
        assert(destination.size() == source.size());
        for (size_t i = startIndex; i < destination.size(); i++)
        {
            CopyPinValueFromPin(context, destination[i], destinationNodeGraph, source[i], sourceNodeGraph);
        }
    }

    template<Decayed T, eIODirection IODirection>
    SetPinValueF CreateSetPinValueFunction()
    {
        return SetPinValueF([](const SetPinValueData& setPinValueData, [[maybe_unused]] const InternalExecutionContext& context) -> void
            {
                Pin& writeToPin = setPinValueData.mNodeGraph->GetPin(setPinValueData.mWriteToPinID);
#ifdef FLY_DEBUG
                [[maybe_unused]] const PinType& writeToPinType = context.pinTypeManager.GetPinType(writeToPin.GetTypeID());
                assert(writeToPinType.GetDataTypeID() == GenericDataTypeID{ GetDataTypeID<T>() });
#endif

                const T& readFromValue = *reinterpret_cast<const T*>(setPinValueData.mReadFromDataPtr);

                T& writeToValue = *reinterpret_cast<T*>(writeToPin.GetDataPtr().Get());
                writeToValue = readFromValue;

                if constexpr (std::same_as<T, Flow>)
                {
                    if (writeToValue)
                    {
                        if constexpr (IODirection == eIODirection::Output)
                        {
                            for (const PinID connectedInputPinID : writeToPin.GetConnectedPinIDs())
                            {
                                const Pin& connectedInputPin = setPinValueData.mNodeGraph->GetPin(connectedInputPinID);
                                context.nodeExecutionQueue->Push(NodeExecutionData{ NodeRef(connectedInputPin.GetNodeID(), *setPinValueData.mNodeGraph), eNodeTriggerReason::Flow });

#ifdef FLY_DEBUG
                                const LinkID linkID = Internal::GetLinkIDByPinIDs(*setPinValueData.mNodeGraph, connectedInputPinID, setPinValueData.mWriteToPinID);
                                context.nodeExecutor.GetDebugger().AddTraversedLink(linkID, context.nodeGraphVariantHandle);
#endif
                            }
                        }
                    }
                }
            });
    }

    template<Decayed T, eIODirection IODirection>
    SetPinValueFromPinF CreateSetPinValueFromPinFunction()
    {
        return SetPinValueFromPinF([](const SetPinValueFromPinData& setPinValueData, const InternalExecutionContext& context) -> void
            {
                const Pin& readFromPin = setPinValueData.mReadFromPinNodeGraph->GetPin(setPinValueData.mReadFromPinID);
                Pin& writeToPin = setPinValueData.mWriteToPinNodeGraph->GetPin(setPinValueData.mWriteToPinID);
                const PinType& readFromPinType = context.pinTypeManager.GetPinType(readFromPin.GetTypeID());
                [[maybe_unused]] const PinType& writeToPinType = context.pinTypeManager.GetPinType(writeToPin.GetTypeID());

                assert(writeToPinType.GetDataTypeID() == GenericDataTypeID{ GetDataTypeID<T>() });

                const T& readFromValue = *reinterpret_cast<const T*>(readFromPin.GetDataPtr().Get());

                T& writeToValue = *reinterpret_cast<T*>(writeToPin.GetDataPtr().Get());

                if (HasFlag(context.dataTypeManager.GetDataTypeTraits(readFromPinType.GetDataTypeID()), eDataTypeTrait::Pointer))
                {
                    if constexpr (!PointerType<T>)
                    {
                        // Since ReadFromPin is pointer -> ReadFromPin.GetDataPtr is actually T**
                        writeToValue = **((const T**)((const T*)readFromPin.GetDataPtr().Get()));
                    }
                    else
                    {
                        writeToValue = readFromValue;
                    }
                }
                else
                {
                    if constexpr (!PointerType<T>)
                    {
                        writeToValue = readFromValue;
                    }
                    else
                    {
                        using RemovePtrType = std::remove_pointer_t<T>;

                        RemovePtrType* const* r1 = reinterpret_cast<const T*>(readFromPin.GetDataPtr().Get());
                        RemovePtrType* r = (RemovePtrType*)r1;

                        writeToValue = r;
                    }
                }

                if constexpr (std::same_as<T, Flow>)
                {
                    if (writeToValue)
                    {
                        if constexpr (IODirection == eIODirection::Output)
                        {
                            for (const PinID connectedInputPinID : writeToPin.GetConnectedPinIDs())
                            {
                                const Pin& connectedInputPin = setPinValueData.mWriteToPinNodeGraph->GetPin(connectedInputPinID);
                                context.nodeExecutionQueue->Push(NodeExecutionData{ NodeRef(connectedInputPin.GetNodeID(), *setPinValueData.mWriteToPinNodeGraph), eNodeTriggerReason::Flow });
                            }
                        }
                    }
                }
            });
    }

    // Define the type transformation logic
    template <typename T>
    struct TransformType
    {
        using type = std::decay_t<T>; // Default to T
    };

    // Specialization: const T& -> T
    template <typename T>
    struct TransformType<const T&>
    {
        using type = T;
    };

    // Specialization: T& -> T&
    template <typename T>
    struct TransformType<T&>
    {
        //static_assert()
        using type = T&;
    };

    // Specialization: T* -> T* (already handled by default, but shown for clarity)
    template <typename T>
    struct TransformType<T*>
    {
        using type = T*;
    };

    // Helper alias template for convenience
    template <typename T>
    using TransformType_t = typename TransformType<T>::type;

    template<size_t Index, size_t Size, eIODirection IODirection, typename Type, typename... Types>
    constexpr void CreatePinTypesInternal(auto& pinTypeIDs, const NodeCreationData& creationData)
    {
        static_assert(!std::is_reference_v<Type>);

        using AllocationType = TransformType_t<Type>;

        const std::vector<std::string>& pinNames = SelectByIODirection(IODirection, creationData.mDescription.mInputPinNames, creationData.mDescription.mOutputPinNames);

        const MemoryPoolID defaultValueMemoryID = [&creationData]() -> MemoryPoolID
            {
                if constexpr (IODirection == eIODirection::Input)
                {
                    const std::any& defaultValueAny = creationData.mDescription.mDefaultValues.at(Index);
                    if (defaultValueAny.has_value())
                    {
                        return Internal::GetMemoryPool().Allocate<AllocationType>(std::any_cast<AllocationType>(defaultValueAny));
                    }
                }

                return InvalidID<MemoryPoolID>();
            }();

        pinTypeIDs[Index] = Internal::GetPinTypeManager().CreatePinType<AllocationType>(
            pinNames[Index],
            IODirection, 
            CreateSetPinValueFunction<AllocationType, IODirection>(), 
            CreateSetPinValueFromPinFunction<AllocationType, IODirection>(),
            InvalidID<PinTypeID>(),
            defaultValueMemoryID
        );

        if constexpr (Index + 1 < Size)
        {
            CreatePinTypesInternal<Index + 1, Size, IODirection, Types...>(pinTypeIDs, creationData);
        }

    }

    template<eIODirection IODirection, typename... Types>
    constexpr std::vector<PinTypeID> CreatePinTypes(const NodeCreationData& creationData)
    {
        constexpr std::size_t Size = sizeof...(Types);
        std::vector<PinTypeID> pinTypeIDs(Size);

        if constexpr (!EmptyParams<Types...>)
        {
            CreatePinTypesInternal<0, Size, IODirection, Types...>(pinTypeIDs, creationData);
        }
        return pinTypeIDs;
    }


    struct NodeCreationContext
    {
        NodeTypeManager& nodeTypeManager;
        PinTypeManager& pinTypeManager;
        DataTypeManager& dataTypeManager;
    };

    // Creates an input pin for a node
    template<typename InputType>
    PinID CreateInputPin(const NodeID nodeID, const NodeTypeID nodeTypeID, const std::size_t index, NodeGraph& nodeGraph, const NodeCreationContext& creationContext)
    {
        static_assert(!(std::is_reference_v<InputType> && !std::is_const_v<std::remove_reference_t<InputType>>), "Non const references are not supported");

        using DecayedType = std::decay_t<InputType>;

        const PinTypeID pinTypeID = creationContext.nodeTypeManager.GetNodeType(nodeTypeID).GetInputPinTypeIDs().at(index);
        
        // Allocate memory for the pin's data
        void* const dataPtr = [pinTypeID, &nodeGraph, &creationContext]() -> void*
            {
                const PinType& pinType = creationContext.pinTypeManager.GetPinType(pinTypeID);
                if (pinType.GetDefaultValueMemoryID() != InvalidID<MemoryPoolID>())
                {
                    const MemoryPool& globalMemoryPool = Internal::GetMemoryPool();
                    return &nodeGraph.GetMemoryArena().Allocate<DecayedType>(globalMemoryPool.At<InputType>(pinType.GetDefaultValueMemoryID()));
                }
                else
                {
                    return &nodeGraph.GetMemoryArena().Allocate<DecayedType>();
                }
            }();

        return Internal::CreatePin(nodeGraph, nodeID, pinTypeID, dataPtr, creationContext);
    }

    template<std::size_t Index, typename InputType, typename... InputTypes>
    void CreateInputPinsInternal(const NodeID nodeID, const NodeTypeID nodeTypeID, std::span<PinID> pinIDs, NodeGraph& nodeGraph, const NodeCreationContext& creationContext)
    {
        pinIDs[Index] = CreateInputPin<InputType>(nodeID, nodeTypeID, Index, nodeGraph, creationContext);
        if constexpr (!EmptyParams<InputTypes...>)
        {
            CreateInputPinsInternal<Index + 1, InputTypes...>(nodeID, nodeTypeID, pinIDs, nodeGraph, creationContext);
        }
    }

    template<bool IsCPPGenerated, typename... Inputs>
    std::tuple<std::array<PinID, sizeof...(Inputs)>, const void*> CreateInputPins(const NodeID nodeID, const NodeTypeID nodeTypeID, NodeGraph& nodeGraph, const NodeCreationContext& creationContext)
    {

        if constexpr (EmptyParams<Inputs...>)
        {
            return {};
        }
        else
        {
            // If the node is cpp generated, we can create a tuple with the data directly
            if constexpr (IsCPPGenerated)
            {
                using TupleType = std::tuple<Inputs...>;
                static_assert(sizeof(TupleType) <= NodeGraphBufferSize);
                auto& tuple = nodeGraph.GetMemoryArena().Allocate<std::tuple<Inputs...>>();
                std::array<PinID, sizeof...(Inputs)> pinIDs{};
                [&] <std::size_t... Index>(std::index_sequence<Index...>)
                {
                    (
                        [&] {
                            using InputType = std::tuple_element_t<Index, TupleType>;
                            static_assert(!(std::is_reference_v<InputType> && !std::is_const_v<std::remove_reference_t<InputType>>), "Non const references are not supported");

                            using DecayedType = std::decay_t<InputType>;
                            static_assert(std::same_as<DecayedType, InputType>);
                            const PinTypeID pinTypeID = creationContext.nodeTypeManager.GetNodeType(nodeTypeID).GetInputPinTypeIDs().at(Index);
                            void* const dataPtr = &std::get<Index>(tuple);
                            pinIDs[Index] = Internal::CreatePin(nodeGraph, nodeID, pinTypeID, dataPtr, creationContext);
                        }(), ...
                            );
                }(std::make_index_sequence<sizeof...(Inputs)>{});
                return { pinIDs, &tuple };
            }
            else
            {
                std::array<PinID, sizeof...(Inputs)> pinIDs{};
                CreateInputPinsInternal<0, Inputs...>(nodeID, nodeTypeID, pinIDs, nodeGraph, creationContext);
                return { pinIDs, nullptr };
            }
        }
    }


    template<typename OutputType>
    PinID CreateOutputPin(const NodeID nodeID, const PinTypeID pinTypeID, NodeGraph& nodeGraph, const NodeCreationContext& creationContext)
    {
        static_assert(!std::is_reference_v<OutputType>, "Return type can't be a reference");
        static_assert(!std::is_same_v<void, OutputType>, "Return type can't be void");

        void* const dataPtr = &nodeGraph.GetMemoryArena().Allocate<OutputType>();
        return Internal::CreatePin(nodeGraph, nodeID, pinTypeID, dataPtr, creationContext);
    }

    template<typename... OutputTypes>
    std::array<PinID, sizeof...(OutputTypes)> CreateOutputPins(const NodeID nodeID, const NodeTypeID nodeTypeID, NodeGraph& nodeGraph, const NodeCreationContext& creationContext)
    {
        std::array<PinID, sizeof...(OutputTypes)> pinIDs{};

        [&] <std::size_t... Index>(std::index_sequence<Index...>)
        {
            [[maybe_unused]] const NodeType& nodeType = Internal::GetNodeTypeManager().GetNodeType(nodeTypeID);
            (
                [&] {
                    const PinTypeID pinTypeID = nodeType.GetOutputPinTypeIDs().at(Index);
                    pinIDs[Index] = CreateOutputPin<OutputTypes>(nodeID, pinTypeID, nodeGraph, creationContext);
                }(), ...
                    );
        }(std::make_index_sequence<sizeof...(OutputTypes)>{});

        return pinIDs;
    }

    template<template<typename> typename ReturnType>
    auto AppendContainers(auto&& a, auto&& b)
    {
        using FirstContainerType = std::decay_t<decltype(a)>;
        using value_type = typename FirstContainerType::value_type;
        ReturnType<value_type> r{};
        r.insert(begin(r), begin(a), end(a));
        r.insert(end(r), begin(b), end(b));
        return r;
    }

    template<bool IsCPPGenerated, typename... OutputTypes, typename... InputTypes>
    CreateNodeFunction CreateCreateNodeFunction(TypeList<OutputTypes...>, TypeList<InputTypes...>)
    {
        return [](const NodeID nodeID, const NodeTypeID nodeTypeID, NodeGraph& nodeGraph, const NodeCreationContext& creationContext) -> Node
            {
                // Create Node Function

                // Input pins
                auto[preExistingInputPinIDs, inputTuplePtr] = CreateInputPins<IsCPPGenerated, InputTypes...>(nodeID, nodeTypeID, nodeGraph, creationContext);

                std::vector<PinID> addedInputPinIDs = Internal::CreateInputPins(nodeGraph, nodeID, nodeTypeID, creationContext, preExistingInputPinIDs.size());

                const std::vector<PinID> totalInputPinIDs = AppendContainers<std::vector>(std::move(preExistingInputPinIDs), std::move(addedInputPinIDs));

                // Output pins
                auto preExistingOutputPinIDs = CreateOutputPins<OutputTypes...>(nodeID, nodeTypeID, nodeGraph, creationContext);

                std::vector<PinID> addedOutputPinIDs = Internal::CreateOutputPins(nodeGraph, nodeID, nodeTypeID, creationContext, preExistingOutputPinIDs.size());

                const std::vector<PinID> totalOutputPinIDs = AppendContainers<std::vector>(std::move(preExistingOutputPinIDs), std::move(addedOutputPinIDs));

                return Node(nodeTypeID, totalInputPinIDs, totalOutputPinIDs, inputTuplePtr);
            };

    }

    template<eNodeTrait Traits = eNodeTrait::None, typename NodeExecutionContextType = Wildcard, typename NodeStateDataType = Wildcard, typename Callable, typename... OutputTypes, typename... InputTypes>
    NodeType CreateNodeType(Callable callable, TypeList<OutputTypes...> outputList, TypeList<InputTypes...>, NodeCreationData creationData)
    {
        static_assert(!AnyArgIsRawReference<InputTypes...> && "Function parameter cannot be of reference type. Consider adding const or change to pointer");

        constexpr bool TakesExecutionContext = HasFlag(Traits, eNodeTrait::TakesExecutionContext);
        constexpr bool TakesNodeState = !std::same_as<Wildcard, NodeStateDataType>;
        constexpr bool TakesInternalExecutionContext = HasFlag(Traits, eNodeTrait::TakesInternalExecutionContext);
        constexpr bool HasFlow = ContainsType<Flow, OutputTypes...> || ContainsType<Flow, InputTypes...>;

        DataTypeID nodeStateDataTypeID = InvalidID<DataTypeID>();
        if constexpr (TakesNodeState)
        {
            TypeParameters nodeStateTypeParams;
            nodeStateTypeParams.mColor = Internal::GetDataTypeManager().GetDefaultColor();
            nodeStateTypeParams.mName = typeid(NodeStateDataType).name();
            nodeStateTypeParams.mIsTargetable = false;
            nodeStateTypeParams.mRegisterPointer = false;
            Internal::GetDataTypeManager().Register<NodeStateDataType>(nodeStateTypeParams);
            nodeStateDataTypeID = GetDataTypeID<NodeStateDataType>();
        }

        eNodeTrait traits = Traits;
        if constexpr (HasFlow)
        {
            traits |= eNodeTrait::HasFlow;
        }

        constexpr bool IsCPPGenerated = HasFlag(Traits, eNodeTrait::CPPGenerated);
        if constexpr (!IsCPPGenerated)
        {
            traits |= eNodeTrait::EditorGenerated;
        }

        creationData.mDescription.mInputPinNames.resize(sizeof...(InputTypes), TypeIdentifierStr);
        creationData.mDescription.mOutputPinNames.resize(sizeof...(OutputTypes), TypeIdentifierStr);
        creationData.mDescription.mDefaultValues.resize(sizeof...(InputTypes));

        const std::vector<PinTypeID> inputPinTypeIDs = CreatePinTypes<eIODirection::Input, TransformType_t<InputTypes>...>(creationData);
        const std::vector<PinTypeID> outputPinTypeIDs = CreatePinTypes<eIODirection::Output, OutputTypes...>(creationData);

        const MemoryPoolID functionMemoryID = Internal::GetMemoryPool().Allocate<Callable>(callable);

        FastExecuteNodeFunction fastExecute = nullptr;
        if constexpr (HasFlag(Traits, eNodeTrait::Trait))
        {
            fastExecute = CreateFastExecuteNodeFunction<TakesInternalExecutionContext, Callable, InputTypes...>(callable);
        }

        CreateNodeFunction createNodeFunction = CreateCreateNodeFunction<IsCPPGenerated>(TypeList<TransformType_t<OutputTypes>...>{}, TypeList<TransformType_t<InputTypes>...>{});
        ExecuteNodeFunction executeNodeFunction = CreateExecuteNodeFunction<TakesExecutionContext, TakesNodeState, TakesInternalExecutionContext, IsCPPGenerated, NodeExecutionContextType, NodeStateDataType, Callable>(outputList, TypeList<TransformType_t<InputTypes>...>{});

        return NodeType(
            creationData.mName,
            createNodeFunction,
            executeNodeFunction,
            fastExecute,
            traits,
            creationData.mEventID,
            creationData.mOwnerDataTypeID,
            inputPinTypeIDs,
            outputPinTypeIDs,
            functionMemoryID,
            nodeStateDataTypeID,
            creationData.mTraitID,
            creationData.mOperatorType
        );
    }

    // For functions with 1 return value
    template<eNodeTrait Traits = eNodeTrait::None, typename OutputType, typename... InputTypes>
    constexpr NodeType FilterNodeType(FuncPtr<OutputType, InputTypes...> function, const NodeCreationData& creationData)
    {
        constexpr bool IsOutputVoid = std::same_as<OutputType, void>;
        if constexpr (HasFlag(Traits, eNodeTrait::HasImplicitFlow))
        {
            if constexpr (IsOutputVoid)
            {
                return CreateNodeType<Traits>(
                    [function](Flow, InputTypes... inputs) -> Flow
                    {
                        function(std::forward<InputTypes>(inputs)...);
                        return Flow(true);
                    },
                    TypeList<Flow>(),
                    TypeList<Flow, InputTypes...>(),
                    creationData
                );
            }
            else
            {
                return CreateNodeType<Traits>(
                    [function](Flow, InputTypes... inputs) -> std::tuple<Flow, OutputType>
                    {
                        OutputType output = function(std::forward<InputTypes>(inputs)...);
                        return { Flow(true), output };
                    },
                    TypeList<Flow, OutputType>(),
                    TypeList<Flow, InputTypes...>(),
                    creationData
                );
            }
        }
        else
        {
            if constexpr (IsOutputVoid)
            {
                return CreateNodeType<Traits>(
                    function,
                    TypeList<>(),
                    TypeList<InputTypes...>(),
                    creationData
                );
            }
            else
            {
                return CreateNodeType<Traits>(
                    function,
                    TypeList<OutputType>(),
                    TypeList<InputTypes...>(),
                    creationData
                );
            }
        }
    }

    // For functions with 1 return value && takes in an execution context
    template<eNodeTrait Traits = eNodeTrait::None, typename ExecutionContextType, typename OutputType, typename... InputTypes>
    constexpr NodeType FilterNodeType(FuncPtr<OutputType, NodeExecutionContext<ExecutionContextType>, InputTypes...> function, const NodeCreationData& creationData)
    {
        constexpr bool IsOutputTypeVoid = std::same_as<OutputType, void>;
        if constexpr (HasFlag(Traits, eNodeTrait::HasImplicitFlow))
        {
            if constexpr (IsOutputTypeVoid)
            {
                return CreateNodeType<Traits | eNodeTrait::TakesExecutionContext, ExecutionContextType>(
                    [function](NodeExecutionContext<ExecutionContextType> context, Flow, InputTypes&&... inputs) -> std::tuple<Flow>
                    {
                        function(context, std::forward<InputTypes>(inputs)...);
                        return { Flow(true) };
                    },
                    TypeList<Flow>(),
                    TypeList<Flow, InputTypes...>(),
                    creationData
                );
            }
            else
            {
                return CreateNodeType<Traits | eNodeTrait::TakesExecutionContext, ExecutionContextType>(
                    [function](NodeExecutionContext<ExecutionContextType> context, Flow, InputTypes&&... inputs) -> std::tuple<Flow, OutputType>
                    {
                        OutputType output = function(context, std::forward<InputTypes>(inputs)...);
                        return { Flow(true), output };
                    },
                    TypeList<Flow, OutputType>(),
                    TypeList<Flow, InputTypes...>(),
                    creationData
                );
            }
        }
        else
        {
            if constexpr (IsOutputTypeVoid)
            {
                return CreateNodeType<Traits | eNodeTrait::TakesExecutionContext, ExecutionContextType>(
                    function,
                    TypeList<>(),
                    TypeList<InputTypes...>(),
                    creationData
                );

            }
            else
            {
                return CreateNodeType<Traits | eNodeTrait::TakesExecutionContext, ExecutionContextType>(
                    function,
                    TypeList<OutputType>(),
                    TypeList<InputTypes...>(),
                    creationData
                );
            }
        }
    }

    // For functions with 1 return value and takes in internal data
    template<eNodeTrait Traits = eNodeTrait::None, typename NodeStateDataType, typename OutputType, typename... InputTypes>
    constexpr NodeType FilterNodeType(FuncPtr<OutputType, NodeState<NodeStateDataType>, InputTypes...> function, const NodeCreationData& creationData)
    {
        constexpr bool IsOutputVoid = std::same_as<OutputType, void>;
        if constexpr (HasFlag(Traits, eNodeTrait::HasImplicitFlow))
        {
            if constexpr (IsOutputVoid)
            {
                return CreateNodeType<Traits, Wildcard, NodeStateDataType>(
                    [function](NodeState<NodeStateDataType> nodeState, Flow, InputTypes&&... inputs) -> std::tuple<Flow>
                    {
                        function(nodeState, inputs...);
                        return { Flow(true) };
                    },
                    TypeList<Flow>(),
                    TypeList<Flow, InputTypes...>(),
                    creationData
                );
            }
            else
            {
                return CreateNodeType<Traits, Wildcard, NodeStateDataType>(
                    [function](NodeState<NodeStateDataType> nodeState, Flow, InputTypes&&... inputs) -> std::tuple<Flow, OutputType>
                    {
                        OutputType output = function(nodeState, inputs...);
                        return { Flow(true), output };
                    },
                    TypeList<Flow, OutputType>(),
                    TypeList<Flow, InputTypes...>(),
                    creationData
                );
            }
        }
        else
        {
            if constexpr (IsOutputVoid)
            {
                return CreateNodeType<Traits, Wildcard, NodeStateDataType>(
                    function,
                    TypeList<>(),
                    TypeList<InputTypes...>(),
                    creationData
                );

            }
            else
            {
                return CreateNodeType<Traits, Wildcard, NodeStateDataType>(
                    function,
                    TypeList<OutputType>(),
                    TypeList<InputTypes...>(),
                    creationData
                );
            }
        }
    }

    // For functions with 1 return value && takes in execution context && takes in internal data 
    template<eNodeTrait Traits = eNodeTrait::None, typename ExecutionContextType, typename NodeStateDataType, typename OutputType, typename... InputTypes>
    constexpr NodeType FilterNodeType(FuncPtr<OutputType, NodeExecutionContext<ExecutionContextType>, NodeState<NodeStateDataType>, InputTypes...> function, const NodeCreationData& creationData)
    {
        constexpr bool IsOutputVoid = std::same_as<OutputType, void>;
        if constexpr (HasFlag(Traits, eNodeTrait::HasImplicitFlow))
        {
            if constexpr (IsOutputVoid)
            {
                return CreateNodeType<Traits, ExecutionContextType, NodeStateDataType>(
                    [function](NodeExecutionContext<ExecutionContextType> context, NodeState<NodeStateDataType> nodeState, Flow, InputTypes&&... inputs) -> std::tuple<Flow>
                    {
                        function(context, nodeState, std::forward<InputTypes>(inputs)...);
                        return { Flow(true) };
                    },
                    TypeList<Flow>(),
                    TypeList<Flow, InputTypes...>(),
                    creationData
                );
            }
            else
            {
                return CreateNodeType<Traits, ExecutionContextType, NodeStateDataType>(
                    [function](NodeExecutionContext<ExecutionContextType> context, NodeState<NodeStateDataType> nodeState, Flow, InputTypes&&... inputs) -> std::tuple<Flow, OutputType>
                    {
                        OutputType output = function(context, nodeState, std::forward<InputTypes>(inputs)...);
                        return { Flow(true), output };
                    },
                    TypeList<Flow, OutputType>(),
                    TypeList<Flow, InputTypes...>(),
                    creationData
                );
            }
        }
        else
        {
            if constexpr (IsOutputVoid)
            {
                return CreateNodeType<Traits | eNodeTrait::TakesExecutionContext, ExecutionContextType, NodeStateDataType>(
                    function,
                    TypeList<>(),
                    TypeList<InputTypes...>(),
                    creationData
                );

            }
            else
            {
                return CreateNodeType<Traits | eNodeTrait::TakesExecutionContext, ExecutionContextType, NodeStateDataType>(
                    function,
                    TypeList<OutputType>(),
                    TypeList<InputTypes...>(),
                    creationData
                );
            }
        }
    }

    // For functions with tuple return value
    template<eNodeTrait Traits = eNodeTrait::None, typename... OutputTypes, typename... InputTypes>
    constexpr NodeType FilterNodeType(FuncPtr<std::tuple<OutputTypes...>, InputTypes...> function, const NodeCreationData& creationData)
    {
        if constexpr (HasFlag(Traits, eNodeTrait::HasImplicitFlow))
        {
            return CreateNodeType<Traits>(
                [function](Flow, InputTypes... inputs) -> std::tuple<Flow, OutputTypes...>
                {
                    return std::tuple_cat(std::make_tuple(Flow(true)), function(std::forward<InputTypes>(inputs)...));
                },
                TypeList<Flow, OutputTypes...>(),
                TypeList<Flow, InputTypes...>(),
                creationData
            );

        }
        else
        {

            return CreateNodeType<Traits>(
                function,
                TypeList<OutputTypes...>(),
                TypeList<InputTypes...>(),
                creationData
            );

        }
    }

    // For functions with tuple return value and takes in execution context
    template<eNodeTrait Traits = eNodeTrait::None, typename ExecutionContextType, typename... OutputTypes, typename... InputTypes>
    constexpr NodeType FilterNodeType(FuncPtr<std::tuple<OutputTypes...>, NodeExecutionContext<ExecutionContextType>, InputTypes...> function, const NodeCreationData& creationData)
    {
        if constexpr (HasFlag(Traits, eNodeTrait::HasImplicitFlow))
        {
            return CreateNodeType<Traits | eNodeTrait::TakesExecutionContext, ExecutionContextType>(
                [function](NodeExecutionContext<ExecutionContextType> context, Flow, InputTypes&&... inputs) -> std::tuple<Flow, OutputTypes...>
                {
                    return std::tuple_cat(std::make_tuple(Flow(true)), function(context, std::forward<InputTypes>(inputs)...));
                },
                TypeList<Flow, OutputTypes...>(),
                TypeList<Flow, InputTypes...>(),
                creationData
            );

        }
        else
        {
            return CreateNodeType<Traits | eNodeTrait::TakesExecutionContext, ExecutionContextType>(function,
                TypeList<OutputTypes...>(),
                TypeList<InputTypes...>(),
                creationData
            );
        }
    }

    // For functions with tuple return value and takes in internal data
    template<eNodeTrait Traits = eNodeTrait::None, typename NodeStateDataType, typename... OutputTypes, typename... InputTypes>
    constexpr NodeType FilterNodeType(FuncPtr<std::tuple<OutputTypes...>, NodeState<NodeStateDataType>, InputTypes...> function, const NodeCreationData& creationData)
    {
        if constexpr (HasFlag(Traits, eNodeTrait::HasImplicitFlow))
        {
            return CreateNodeType<Traits, Wildcard, NodeStateDataType>(
                [function](NodeState<NodeStateDataType> nodeState, Flow, InputTypes&&... inputs) -> std::tuple<Flow, OutputTypes...>
                {
                    return std::tuple_cat(std::make_tuple(Flow(true)), function(nodeState, std::forward<InputTypes>(inputs)...));
                },
                TypeList<Flow, OutputTypes...>(),
                TypeList<Flow, InputTypes...>(),
                creationData
            );

        }
        else
        {
            return CreateNodeType<Traits, Wildcard, NodeStateDataType>(
                function,
                TypeList<OutputTypes...>(),
                TypeList<InputTypes...>(),
                creationData
            );
        }
    }

    // For functions with tuple return value and takes in execution context and takes in NodeState
    template<eNodeTrait Traits = eNodeTrait::None, typename ExecutionContextType, typename NodeStateDataType, typename... OutputTypes, typename... InputTypes>
    constexpr NodeType FilterNodeType(FuncPtr<std::tuple<OutputTypes...>, NodeExecutionContext<ExecutionContextType>, NodeState<NodeStateDataType>, InputTypes...> function, const NodeCreationData& creationData)
    {
        if constexpr (HasFlag(Traits, eNodeTrait::HasImplicitFlow))
        {
            return CreateNodeType<Traits | eNodeTrait::TakesExecutionContext, ExecutionContextType, NodeStateDataType>(
                [function](NodeExecutionContext<ExecutionContextType> context, NodeState<NodeStateDataType> nodeState, Flow, InputTypes&&... inputs) -> std::tuple<Flow, OutputTypes...>
                {
                    return std::tuple_cat(std::make_tuple(Flow(true)), function(context, nodeState, std::forward<InputTypes>(inputs)...));
                },
                TypeList<Flow, OutputTypes...>(),
                TypeList<Flow, InputTypes...>(),
                creationData
            );
        }
        else
        {
            return CreateNodeType<Traits | eNodeTrait::TakesExecutionContext, ExecutionContextType, NodeStateDataType>(function,
                TypeList<OutputTypes...>(),
                TypeList<InputTypes...>(),
                creationData
            );
        }
    }


    // For functions with 1 return value and takes in InternalExecutionContext
    template<eNodeTrait Traits = eNodeTrait::None, typename OutputType, typename... InputTypes>
    constexpr NodeType FilterNodeType(FuncPtr<OutputType, InternalExecutionContextPtr, InputTypes...> function, const NodeCreationData& creationData)
    {
        constexpr bool IsOutputVoid = std::same_as<OutputType, void>;
        if constexpr (HasFlag(Traits, eNodeTrait::HasImplicitFlow))
        {
            if constexpr (IsOutputVoid)
            {
                return CreateNodeType<Traits | eNodeTrait::TakesInternalExecutionContext>(
                    [function](InternalExecutionContextPtr context, Flow, InputTypes... inputs) -> Flow
                    {
                        function(context, std::forward<InputTypes>(inputs)...);
                        return Flow(true);
                    },
                    TypeList<Flow>(), TypeList<Flow, InputTypes...>(), creationData
                );
            }
            else
            {
                return CreateNodeType<Traits | eNodeTrait::TakesInternalExecutionContext>(
                    [function](InternalExecutionContextPtr context, Flow, InputTypes... inputs) -> std::tuple<Flow, OutputType>
                    {
                        return { Flow(true), function(context, std::forward<InputTypes>(inputs)...) };
                    },
                    TypeList<Flow, OutputType>(), TypeList<Flow, InputTypes...>(), creationData
                );
            }
        }
        else
        {
            if constexpr (IsOutputVoid)
            {
                return CreateNodeType<Traits | eNodeTrait::TakesInternalExecutionContext>(
                    [function](InternalExecutionContextPtr context, InputTypes... inputs) -> OutputType
                    {
                        return function(context, std::forward<InputTypes>(inputs)...);
                    },
                    TypeList<>(), TypeList<InputTypes...>(), creationData
                );
            }
            else
            {
                return CreateNodeType<Traits | eNodeTrait::TakesInternalExecutionContext>(
                    [function](InternalExecutionContextPtr context, InputTypes... inputs) -> OutputType
                    {
                        return function(context, std::forward<InputTypes>(inputs)...);
                    },
                    TypeList<OutputType>(), TypeList<InputTypes...>(), creationData
                );
            }
        }
    }


    // For functions with 1 return value and takes in InternalExecutionContext and NodeState
    template<eNodeTrait Traits = eNodeTrait::None, typename NodeStateDataType, typename OutputType, typename... InputTypes>
    constexpr NodeType FilterNodeType(FuncPtr<OutputType, InternalExecutionContextPtr, NodeState<NodeStateDataType>, InputTypes...> function, const NodeCreationData& creationData)
    {
        constexpr bool IsOutputVoid = std::same_as<OutputType, void>;
        if constexpr (HasFlag(Traits, eNodeTrait::HasImplicitFlow))
        {
            if constexpr (IsOutputVoid)
            {
                return CreateNodeType<Traits | eNodeTrait::TakesInternalExecutionContext, Wildcard, NodeStateDataType>(
                    [function](InternalExecutionContextPtr context, NodeState<NodeStateDataType> aNodeState, Flow, InputTypes... inputs) -> Flow
                    {
                        function(context, aNodeState, std::forward<InputTypes>(inputs)...);
                        return Flow(true);
                    },
                    TypeList<Flow>(),
                    TypeList<Flow, InputTypes...>(),
                    creationData
                );
            }
            else
            {
                return CreateNodeType<Traits | eNodeTrait::TakesInternalExecutionContext, Wildcard, NodeStateDataType>(
                    [function](InternalExecutionContextPtr context, NodeState<NodeStateDataType> aNodeState, Flow, InputTypes... inputs) -> std::tuple<Flow, OutputType>
                    {
                        return std::tuple_cat(std::make_tuple(Flow(true)), function(context, aNodeState, std::forward<InputTypes>(inputs)...));
                    },
                    TypeList<Flow, OutputType>(),
                    TypeList<Flow, InputTypes...>(),
                    creationData
                );
            }
        }
        else
        {
            if constexpr (IsOutputVoid)
            {
                return CreateNodeType<Traits | eNodeTrait::TakesInternalExecutionContext, Wildcard, NodeStateDataType>(
                    [function](InternalExecutionContextPtr context, NodeState<NodeStateDataType> aNodeState, InputTypes... inputs) -> OutputType
                    {
                        return function(context, aNodeState, std::forward<InputTypes>(inputs)...);
                    },
                    TypeList<>(),
                    TypeList<InputTypes...>(),
                    creationData
                );
            }
            else
            {
                return CreateNodeType<Traits | eNodeTrait::TakesInternalExecutionContext, Wildcard, NodeStateDataType>(
                    [function](InternalExecutionContextPtr context, NodeState<NodeStateDataType> aNodeState, InputTypes... inputs) -> OutputType
                    {
                        return function(context, aNodeState, std::forward<InputTypes>(inputs)...);
                    },
                    TypeList<OutputType>(),
                    TypeList<InputTypes...>(),
                    creationData
                );
            }
        }
    }

    // For functions with tuple return value and takes in InternalExecutionContext
    template<eNodeTrait Traits = eNodeTrait::None, typename... OutputTypes, typename... InputTypes>
    constexpr NodeType FilterNodeType(FuncPtr<std::tuple<OutputTypes...>, InternalExecutionContextPtr, InputTypes...> function, const NodeCreationData& creationData)
    {
        if constexpr (HasFlag(Traits, eNodeTrait::HasImplicitFlow))
        {
            return CreateNodeType<Traits | eNodeTrait::TakesInternalExecutionContext>(
                [function](InternalExecutionContextPtr context, Flow, InputTypes... inputs) -> std::tuple<Flow, OutputTypes...>
                {
                    return std::tuple_cat(std::make_tuple(Flow(true)), function(context, std::forward<InputTypes>(inputs)...));
                },
                TypeList<Flow, OutputTypes...>(),
                TypeList<Flow, InputTypes...>(),
                creationData
            );
        }
        else
        {
            return CreateNodeType<Traits | eNodeTrait::TakesInternalExecutionContext>(
                [function](InternalExecutionContextPtr context, InputTypes... inputs) -> std::tuple<OutputTypes...>
                {
                    return function(context, std::forward<InputTypes>(inputs)...);
                },
                TypeList<OutputTypes...>(),
                TypeList<InputTypes...>(),
                creationData
            );
        }
    }

    // non-const member functions with non-void return
    template<typename ClassType, typename OutputType, typename... InputTypes>
    NodeType FilterMemberNodeType(FuncPtrMember<ClassType, OutputType, InputTypes...> function, const NodeCreationData& creationData)
    {
        auto callable = [function](ClassType* classType, InputTypes&&... inputTypes) -> OutputType
            {
                return (classType->*function)(std::forward<InputTypes>(inputTypes)...);
            };
        return CreateNodeType(
            callable,
            TypeList<OutputType>(),
            TypeList<ClassType*, InputTypes...>(),
            creationData
        );
    }

    // void returning member functions - generates flow
    template<typename ClassType, typename OutputType, typename... InputTypes> requires std::same_as<OutputType, void>
    NodeType FilterMemberNodeType(FuncPtrMember<ClassType, OutputType, InputTypes...> function, const NodeCreationData& creationData)
    {
        auto callable = [function](Flow, ClassType* classType, InputTypes&&... inputTypes) -> Flow
            {
                (classType->*function)(std::forward<InputTypes>(inputTypes)...);
                return Flow(true);
            };
        return CreateNodeType<eNodeTrait::HasImplicitFlow>(
            callable,
            TypeList<Flow>(),
            TypeList<Flow, ClassType*, InputTypes...>(),
            creationData
        );
    }

    // const member functions
    template<typename ClassType, typename OutputType, typename... InputTypes>
    NodeType FilterMemberNodeType(FuncPtrMember_Const<ClassType, OutputType, InputTypes...> function, const NodeCreationData& creationData)
    {
        auto callable = [function](ClassType* classType, InputTypes&&... inputTypes) -> OutputType
            {
                return (classType->*function)(std::forward<InputTypes>(inputTypes)...);
            };
        return CreateNodeType(
            callable,
            TypeList<OutputType>(),
            TypeList<ClassType*, InputTypes...>(),
            creationData
        );
    }
}