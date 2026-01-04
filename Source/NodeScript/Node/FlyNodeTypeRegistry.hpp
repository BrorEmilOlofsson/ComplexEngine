#pragma once
#include "../FlyDefines.hpp"
#include "../Utilities/FlyUtilities.hpp"
#include "FlyNodeCreation.hpp"
#include "FlyNodeTypeManager.hpp"
#include "../Utilities/FlyMeta.hpp"
#include "../Pin/FlyPinTypeManager.hpp"
#include "../DataType/FlyDataTypeManager.hpp"
#include "../Trait/FlyTraitManager.hpp"
#include "../DataType/FlyClass.hpp"
#include "../Instance/FlyClassInstance.hpp"
#include "../DataType/FlyVariableRef.hpp"
#include "../Trait/FlyTraitObject.hpp"

namespace FLY_NAMESPACE
{

	inline std::string CleanUpNodeName(const std::string& str)
	{
		std::string result;
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (i > 0 && std::isupper(str[i]) && std::islower(str[i - 1]))
			{
				result += ' ';
			}
			else if (i < str.size() - 1 && str[i] == ':' && str[i + 1] == ':')
			{
				result += '/' + result + ' ';
				i++;
				continue;
			}
			result += str[i];
		}
		return result;
	}

	inline std::string GetTraitFunctionName(const std::string& nodeFullName)
	{
		const size_t directoryIndex = nodeFullName.find_first_of('/');

		if (directoryIndex == std::string::npos)
		{
			assert(false);
			return std::string();
		}


		const size_t parenthesisIndex = nodeFullName.find_first_of('(');
		if (parenthesisIndex == std::string::npos)
		{
			assert(false);
		}

		return std::string();
	}

	inline std::string GetClassNameFromMemberName(const std::string& str)
	{
		return str.substr(0, str.find_first_of(':'));
	}

	inline NodeTypeID RegisterInternal(NodeType&& nodeType, const NodeTypeDesc& description = NodeTypeDesc())
	{
		const std::string defaultPinNames = description.mShowDataTypePinNames ? TypeIdentifierStr : "";
        std::vector<std::string> cleanedInputPinNames = description.mInputPinNames;
        std::vector<std::string> cleanedOutputPinNames = description.mOutputPinNames;
		cleanedInputPinNames.resize(nodeType.GetInputPinTypeIDs().size(), defaultPinNames);
		cleanedOutputPinNames.resize(nodeType.GetOutputPinTypeIDs().size(), defaultPinNames);


		for (size_t i = 0; i < cleanedInputPinNames.size(); ++i)
		{
			Internal::GetPinTypeManager().GetPinType(nodeType.GetInputPinTypeIDs()[i]).SetName(cleanedInputPinNames[i]);
		}
		for (size_t i = 0; i < cleanedOutputPinNames.size(); ++i)
		{
			Internal::GetPinTypeManager().GetPinType(nodeType.GetOutputPinTypeIDs()[i]).SetName(cleanedOutputPinNames[i]);
		}
		nodeType.SetName(CleanUpNodeName(nodeType.GetName()));

		assert(!nodeType.GetName().empty());

		return Internal::GetNodeTypeManager().Register(nodeType);
	}

	template<eNodeTrait Traits = eNodeTrait::None, typename OutputType, typename... InputTypes>
	inline NodeTypeID RegisterSystemNodeType(FuncPtr<OutputType, InputTypes...> function, const NodeCreationData& creationData = NodeCreationData())
	{
		return RegisterInternal(FilterNodeType<Traits>(function, creationData), creationData.mDescription);
	}

	template<typename T>
	inline T* GetterNode(InternalExecutionContextPtr context)
	{
		const VariableRef& variableRef = Internal::GetVariableRefByNodeRef(CreateGlobalNodeRef(context->nodeData.nodeRef, *context->flyClass));
		VariableInstance& variableInstance = context->classInstance->mVariableContainerInstance.mVariableInstances[variableRef.GetVarID()];
		T* outputValue = reinterpret_cast<T*>(variableInstance.mRuntimeValueDataPtr.Get());
		return outputValue;
	}

	template<typename T>
	inline void SetterNode(InternalExecutionContextPtr context, const T& value)
	{
		const VariableRef& variableRef = Internal::GetVariableRefByNodeRef(CreateGlobalNodeRef(context->nodeData.nodeRef, *context->flyClass));
		VariableInstance& variableInstance = context->classInstance->mVariableContainerInstance.mVariableInstances[variableRef.GetVarID()];
		T& runtimeValue = *reinterpret_cast<T*>(variableInstance.mRuntimeValueDataPtr.Get());
		runtimeValue = value;
	}

	template<typename T>
	inline void RegisterGetterNodeType()
	{
		const std::string& typeName = Internal::GetDataTypeManager().Find<T>()->Name();
		NodeCreationData getterData;
		getterData.mName = "Get " + typeName;
		const NodeTypeID nodeTypeID = RegisterInternal(FilterNodeType<eNodeTrait::Getter>(GetterNode<T>, getterData));
		Internal::GetNodeTypeManager().SetGetterNodeTypeID(GetDataTypeID<T>(), nodeTypeID);
	}


	template<typename T>
	inline void RegisterSetterNodeType()
	{
		const std::string& typeName = Internal::GetDataTypeManager().Find<T>()->Name();

		NodeCreationData setterData;
		setterData.mName = "Set " + typeName;
		const NodeTypeID nodeTypeID = RegisterInternal(FilterNodeType<eNodeTrait::Setter | eNodeTrait::HasImplicitFlow>(SetterNode<T>, setterData));
		Internal::GetNodeTypeManager().SetSetterNodeTypeID(GetDataTypeID<T>(), nodeTypeID);
	}

	template<PointerType T>
	static T GetTargetNode(InternalExecutionContextPtr context)
	{
		return reinterpret_cast<T>(context->target);
	}


	template<typename ContainerType>
	std::tuple<Flow, std::add_pointer_t<typename ContainerType::value_type>, Flow> ForEachNode(InternalExecutionContextPtr context, Flow, const ContainerType& aContainer)
	{

		const Node& node = context->nodeData.nodeRef.GetNodeGraph().GetNode(context->nodeData.nodeRef.GetNodeID());
		const NodeExecutionData nodeData = context->nodeData;
		NodeExecutionQueue* previousNodeExecutionQueue = context->nodeExecutionQueue;

		for (auto& element : aContainer)
		{

			NodeExecutionQueue nodeExecutionQueue(context->nodeExecutor);
			context->nodeExecutionQueue = &nodeExecutionQueue;

			SetOutputValues(std::tuple{ Flow(true), &element }, node.GetOutputPins(), *context);

			nodeExecutionQueue.Execute();

			context->nodeData = nodeData;

		}

		context->nodeExecutionQueue = previousNodeExecutionQueue;

		return { Flow(false), nullptr, Flow(true) };
	}

	class NodeTypeRegistry
	{
	public:

		template<eNodeTrait Traits, typename OutputType, typename... InputTypes>
		static void RegisterNodeType(FuncPtr<OutputType, InputTypes...> function, auto&& creationData = NodeCreationData())
		{
			RegisterInternal(FilterNodeType<Traits>(function, std::forward<NodeCreationData>(creationData)), std::forward<NodeTypeDesc>(creationData.mDescription));
		}

		template<eNodeTrait Traits, typename ClassType, typename OutputType, typename... InputTypes>
		static void RegisterFakeMemberNodeType(FuncPtr<OutputType, InputTypes...> function, auto&& creationData = NodeCreationData())
		{
			creationData.mOwnerDataTypeID = GetDataTypeID<ClassType*>();
			const NodeTypeID nodeTypeID = RegisterInternal(FilterNodeType<Traits>(function, std::forward<NodeCreationData>(creationData)), std::forward<NodeTypeDesc>(creationData.mDescription));

			if (DataType* dataType = Internal::GetDataTypeManager().Find<ClassType*>())
			{
				dataType->AddMemberNodeTypeID(nodeTypeID);
			}
		}

		template<typename ClassType, typename OutputType, typename... InputTypes>
		static void RegisterMemberNodeType(FuncPtrMember<ClassType, OutputType, InputTypes...> function, auto&& creationData = NodeCreationData())
		{
			creationData.mOwnerDataTypeID = GetDataTypeID<ClassType*>();
			const NodeTypeID nodeTypeID = RegisterInternal(FilterMemberNodeType(function, std::forward<NodeCreationData>(creationData)), std::forward<NodeTypeDesc>(creationData.mDescription));

			if (DataType* dataType = Internal::GetDataTypeManager().Find<ClassType*>())
			{
				dataType->mNodeTypeIDs.push_back(nodeTypeID);
			}
		}

		template<typename ClassType, typename OutputType, typename... InputTypes>
		static void RegisterMemberNodeType(FuncPtrMember_Const<ClassType, OutputType, InputTypes...> function, auto&& creationData = NodeCreationData())
		{
			creationData.mOwnerDataTypeID = GetDataTypeID<ClassType*>();
			const NodeTypeID nodeTypeID = RegisterInternal(FilterMemberNodeType(function, std::forward<NodeCreationData>(creationData)), std::forward<NodeTypeDesc>(creationData.mDescription));

			if (DataType* dataType = Internal::GetDataTypeManager().Find<ClassType*>())
			{
				dataType->mNodeTypeIDs.push_back(nodeTypeID);
			}
		}

		template<typename ClassType, typename MemberType>
		static void RegisterMemberVariable(MemberType ClassType::* member, const std::string& directory, const std::string& variableName)
		{
			auto getterFunc = [member](ClassType* object) -> MemberType
				{
					if (!object)
					{
						return MemberType{};
					}
					return object->*member;
				};


			NodeCreationData getterData;
			getterData.mName = directory + "/Get " + variableName;
			getterData.mOwnerDataTypeID = GetDataTypeID<ClassType*>();

			const NodeTypeID getterNodeTypeID = RegisterInternal(
				CreateNodeType(
					getterFunc,
					TypeList<std::remove_const_t<MemberType>>(),
					TypeList<ClassType*>(),
					std::move(getterData)
				)
			);

			DataType* dataType = Internal::GetDataTypeManager().Find<ClassType*>();
			if (dataType)
			{
				dataType->AddMemberNodeTypeID(getterNodeTypeID);
			}

			if constexpr (!std::is_const_v<MemberType>)
			{
				auto setterFunc = [member](Flow, ClassType* object, const MemberType& value) -> Flow
					{
						if (object)
						{
							object->*member = value;
						}
						return Flow(true);
					};
				;

				NodeCreationData setterData;
				setterData.mName = directory + "/Set " + variableName;
				setterData.mOwnerDataTypeID = GetDataTypeID<ClassType*>();
				const NodeTypeID setterNodeTypeID = RegisterInternal(
					CreateNodeType(
						setterFunc,
						TypeList<Flow>(),
						TypeList<Flow, ClassType*, std::remove_const_t<MemberType>>(),
						std::move(setterData)
					)
				);

				if (dataType)
				{
					dataType->AddMemberNodeTypeID(setterNodeTypeID);
				}
			}
		}
	};

	struct Event final
	{
	};

	struct AutoTick final
	{
	};

	struct Pure final
	{
	};

	struct InputNames final
	{
		constexpr InputNames(std::initializer_list<std::string> aNames)
			: mNames(aNames)
		{
		}
		std::vector<std::string> mNames;
	};

	struct OutputNames final
	{
		constexpr OutputNames(std::initializer_list<std::string> aNames)
			: mNames(aNames)
		{
		}
		std::vector<std::string> mNames;
	};

	struct DefaultValues final
	{

		constexpr DefaultValues(std::initializer_list<std::any> values)
			: mValues(values)
		{
		}
		std::vector<std::any> mValues;
	};

	template<typename... Args>
	struct DefaultValues2 final
	{
		constexpr DefaultValues2(Args&&... aDefaultValues)
			: mValues(std::tuple<Args...>{ std::forward<Args>(aDefaultValues)... })
		{

		}

		std::tuple<Args...> mValues;
	};

	template<typename... Args>
	DefaultValues2(Args&&...) -> DefaultValues2<Args...>;

	struct Directory final
	{
		std::string mDirectory;
	};

	template<typename T>
	struct MemberOf final
	{
		using type = T;
	};

	template<typename T>
	struct TraitImplOf final
	{
		using type = T;
	};

	// Helper to get the type at a given index in a parameter pack
	template<std::size_t Index, typename... Types>
	struct TypeAtIndex
	{
		// Use std::tuple_element to get the type
		using type = typename std::tuple_element<Index, std::tuple<Types...>>::type;
	};

	// Helper alias to simplify syntax
	template<std::size_t Index, typename... Types>
	using TypeAtIndex_t = typename TypeAtIndex<Index, Types...>::type;

	template<typename T>
	concept IsProtectedType = SameAsTemplateType<NodeState, std::decay_t<T>> ||
		std::same_as<InternalExecutionContextPtr, std::decay_t<T>> ||
		SameAsTemplateType<NodeExecutionContext, std::decay_t<T>>;

	template<size_t Index, typename T>
	struct TypeListElement;

	template<size_t Index, typename... Types>
	struct TypeListElement<Index, TypeList<Types...>>
	{
		using type = TypeAtIndex<Index, Types...>;
	};


	//template<typename OutputType, typename... InputTypes>
	//constexpr auto CreateTypeListFromFunction(FuncPtr<OutputType, InputTypes...>)
	//{
	//return TypeList<InputTypes...>{};
	//}

	//template<typename T, typename... Rest>
	//constexpr auto CreateTypeListWithoutProtectedTypesInternal()
	//{
	//	if constexpr (IsProtectedType<std::decay_t<T>>)
	//	{
	//		if constexpr (sizeof...(Rest) > 0)
	//		{
	//			return CreateTypeListWithoutProtectedTypesInternal<Rest...>();
	//		}
	//		else
	//		{
	//			return TypeList<None>{};
	//		}
	//	}
	//	else
	//	{
	//		return TypeList<TransformType_t<T>, TransformType_t<Rest>...>{};
	//	}
	//}

	//template<typename OutputType, typename... InputTypes>
	//constexpr auto CreateTypeListFromFunctionWithoutProtectedTypes(FuncPtr<OutputType, InputTypes...>)
	//{
	//	return CreateTypeListWithoutProtectedTypesInternal<InputTypes...>();
	//}

	//template<typename... Pack1, typename... Pack2>
	//constexpr std::tuple<Pack2...> ConvertTuple(const std::tuple<Pack1...>& inputTuple, std::tuple<Pack2...>* = nullptr)
	//{
	//	static_assert(sizeof...(Pack1) == sizeof...(Pack2), "Parameter packs must have the same size");

	//	// Helper lambda to unpack the tuple and transform types
	//	return[&]<std::size_t... Indices>(std::index_sequence<Indices...>)
	//	{
	//		return std::make_tuple(static_cast<Pack2>(std::get<Indices>(inputTuple))...);
	//	}(std::make_index_sequence<sizeof...(Pack1)>{});
	//}



	template<typename TraitType>
	struct TraitModel final : TraitObject {};


	struct RegisterFunctionNode final
	{
		constexpr RegisterFunctionNode() = default;

		template<typename ContainerType, typename ContainerType2>
		constexpr static void InsertAtEnd(ContainerType& aContainer, ContainerType2&& aContainer2)
		{
			aContainer.insert(std::end(aContainer), std::begin(aContainer2), std::end(aContainer2));
		}

		//template<size_t CurrentIndex, typename T, typename... Rest>
		//constexpr static size_t GetFunctionParameterStartIndexInternal()
		//{
		//	if constexpr (IsProtectedType<T>)
		//	{
		//		if constexpr (sizeof...(Rest) > 0)
		//		{
		//			return GetFunctionParameterStartIndexInternal<CurrentIndex + 1, Rest...>();

		//		}
		//		else
		//		{
		//			return CurrentIndex;
		//		}
		//	}
		//	else
		//	{
		//		return CurrentIndex;
		//	}
		//}

		//template<typename... Args>
		//constexpr static size_t GetFunctionParameterStartIndex()
		//{
		//	return GetFunctionParameterStartIndexInternal<0, Args...>();
		//}


		//template<size_t CurrentIndex, size_t FuncParamStartIndex, typename Current, typename... Rest>
		//constexpr static void InsertStuffInternal(std::vector<std::any>& aAnyVector, const auto& aTuple, auto aTypeList)
		//{
		//	if constexpr (CurrentIndex < std::tuple_size_v<decltype(aTuple)>)
		//	{
		//		using RealType = std::decay_t<TypeListElement<decltype(aTypeList)>::type>;
		//		aAnyVector.emplace_back(RealType(std::get<CurrentIndex>(aTuple)));

		//		InsertStuffInternal<CurrentIndex + 1, FuncParamStartIndex, Rest...>(aAnyVector, aTuple, aTypeList);
		//	}
		//}

		//template<size_t StartIndex, typename... Args>
		//constexpr static void InsertStuff(std::vector<std::any>& aAnyVector, const auto& aTuple, auto aTypeList)
		//{
		//	constexpr size_t FuncParamStartIndex = 0;
		//	InsertStuffInternal<0, FuncParamStartIndex, Args...>(aAnyVector, aTuple, aTypeList);
		//}

		//template<typename... Pack1, typename... Pack2>
		//constexpr static void TryConvertTuple(const std::tuple<Pack1...>& inputTuple, std::tuple<Pack2...>& aOutputTuple)
		//{
		//	static_assert(sizeof...(Pack1) == sizeof...(Pack2), "Parameter packs must have the same size");

		//	// Helper lambda to unpack the tuple and transform types
		//	return[&]<std::size_t... Indices>(std::index_sequence<Indices...>)
		//	{
		//		return std::make_tuple(static_cast<Pack2>(std::get<Indices>(inputTuple))...);
		//	}(std::make_index_sequence<sizeof...(Pack1)>{});
		//}

		//template<typename... Pack1, typename... Pack2>
		//constexpr static void TryConvertTuple(const std::tuple<Pack1...>& inputTuple, TypeList<Pack2...>, std::vector<std::any>& aOutputVector)
		//{
		//	std::tuple<TransformType_t<Pack2>...> outputTuple;
		//	TryConvertTuple(inputTuple, outputTuple);
		//}

		template<typename ClassType = None, typename Function, typename... Extra>
		constexpr static auto CreateNodeCreationData(Function function, const std::string& functionName, [[maybe_unused]] Extra&&... aExtraTypes)
		{
			constexpr bool isPure = ContainsType<Pure, Extra...>;
			NodeCreationData nodeCreationData;
			if constexpr (ContainsType<Event, Extra...>)
			{
				const EventID eventID{ std::hash<Function>()(function) };
				nodeCreationData.mEventID = eventID;
			}

			if constexpr (ContainsType<InputNames, Extra...>)
			{
				if constexpr (!isPure)
				{
					nodeCreationData.mDescription.mInputPinNames.emplace_back(TypeIdentifierStr);
				}

				InsertAtEnd(nodeCreationData.mDescription.mInputPinNames, std::move(Extract<InputNames>(std::forward<Extra>(aExtraTypes)...).mNames));
			}

			if constexpr (ContainsType<OutputNames, Extra...>)
			{
				if constexpr (!isPure)
				{
					nodeCreationData.mDescription.mOutputPinNames.emplace_back(TypeIdentifierStr);
				}
				InsertAtEnd(nodeCreationData.mDescription.mOutputPinNames, std::move(Extract<OutputNames>(std::forward<Extra>(aExtraTypes)...).mNames));
			}

			if constexpr (ContainsType<DefaultValues, Extra...>)
			{
				if constexpr (!isPure)
				{
					nodeCreationData.mDescription.mDefaultValues.emplace_back();
				}

				InsertAtEnd(nodeCreationData.mDescription.mDefaultValues, std::move(Extract<DefaultValues>(std::forward<Extra>(aExtraTypes)...).mValues));
			}

			if constexpr (ContainsTemplateType<DefaultValues2, Extra...>)
			{
				if constexpr (!isPure)
				{
					nodeCreationData.mDescription.mDefaultValues.emplace_back();
				}

				//const auto& defaultValuesTuple = ExtractTemplate<DefaultValues2>(std::forward<Extra>(aExtraTypes)...).mValues;

				//TryConvertTuple(defaultValuesTuple, CreateTypeListFromFunctionWithoutProtectedTypes(function), nodeCreationData.mDescription.mDefaultValues);
			}


			if constexpr (ContainsType<AutoTick, Extra...>)
			{
				nodeCreationData.mEventID = AutoTickEventID;

				static_assert(!ContainsType<Event, Extra...>);
			}

			if constexpr (ContainsType<Directory, Extra...>)
			{
				nodeCreationData.mName = Extract<Directory>(std::forward<Extra>(aExtraTypes)...).mDirectory + "/" + functionName;
			}
			else
			{
				if constexpr (!std::same_as<ClassType, None>)
				{
					const DataType* c = Internal::GetDataTypeManager().Find(GetDataTypeID<ClassType>());
					const DataType* classDataType = c ? c : Internal::GetDataTypeManager().Find<ClassType*>();
					assert(classDataType);
					nodeCreationData.mName = classDataType->Name() + "/" + functionName;
				}
				else
				{
					if (functionName.find("::") == std::string::npos && functionName.find('/') == std::string::npos)
					{
						nodeCreationData.mName = "Default/" + functionName;
					}
					else
					{
						nodeCreationData.mName = functionName;
					}
				}
			}

			return nodeCreationData;
		}



		template<typename OutputType, typename... InputTypes, typename... Extra>
		constexpr static RegisterFunctionNode RegisterFunction(FuncPtr<OutputType, InputTypes...> function, const std::string& functionName, Extra&&... extraTypes)
		{
			constexpr eNodeTrait Traits = (ContainsType<Pure, Extra...> || ContainsType<Event, Extra...> ? eNodeTrait::None : eNodeTrait::HasImplicitFlow) 
				| eNodeTrait::CPPGenerated;

			if constexpr (ContainsTemplateType<MemberOf, Extra...>)
			{
				using ClassType = std::decay_t<decltype(ExtractTemplate<MemberOf, Extra...>(std::forward<Extra>(extraTypes)...))>::type;
				NodeTypeRegistry::RegisterFakeMemberNodeType<Traits, ClassType>(function, CreateNodeCreationData<ClassType>(function, functionName, std::forward<Extra>(extraTypes)...));
			}
			else
			{
				NodeTypeRegistry::RegisterNodeType<Traits>(function, CreateNodeCreationData(function, functionName, std::forward<Extra>(extraTypes)...));
			}

			return RegisterFunctionNode();
		}


		template<typename ClassType, typename OutputType, typename... InputTypes, typename... Extra>
		static constexpr RegisterFunctionNode RegisterFunction(FuncPtrMember<ClassType, OutputType, InputTypes...> function, const std::string& functionName, Extra&&... extraTypes)
		{
			//NodeTypeRegistry::RegisterMemberNodeType(function, GetNodeCreationData<ClassType>(function, functionName, std::forward<Extra>(aExtraTypes)...));

			return RegisterFunctionNode();
		}

		template<typename ClassType, typename OutputType, typename... InputTypes, typename... Extra>
		static constexpr RegisterFunctionNode RegisterFunction(FuncPtrMember_Const<ClassType, OutputType, InputTypes...> function, const std::string& functionName, Extra&&... extraTypes)
		{
			//NodeTypeRegistry::RegisterMemberNodeType(function, GetNodeCreationData<ClassType>(function, functionName, std::forward<Extra>(aExtraTypes)...));

			return RegisterFunctionNode();
		}



		template<typename TraitType, typename OutputType, typename... InputTypes>
		static OutputType CallerTraitNode(InternalExecutionContextPtr context, TraitModel<TraitType> traitObject, InputTypes... inputTypes)
		{
			TraitImplementation* traitImplementation = Internal::GetTraitManager().GetTraitImplementation(traitObject.mDataTypeID, traitObject.mTraitID);

			if (!traitImplementation)
			{
				assert(false);
				return OutputType{};
			}

			return std::visit(Visitor{
				[context, traitObject, &inputTypes...](const CPPTraitImplementation& traitImplementation) -> OutputType
				{
					const NodeType& nodeType = context->nodeTypeManager.GetNodeType(traitImplementation.mNodeTypeID);

					std::tuple<InputTypes...> inputTuple{};

					OutputType outputValue{};

					nodeType.GetFastExecuteFunction().Invoke(*context, context->foundationMemoryPool, nodeType, traitObject.mDataPtr.Get(), &inputTuple, &outputValue);
					return outputValue;
				},
				[context, &inputTypes...](FlyTraitImplementation& traitImplementation) -> OutputType
				{

					const Node& inputNode = traitImplementation.mEventGraph.GetNodeGraph().GetNode(FlyTraitImplementation::_InputNodeID);

					SetPinValues(*context, inputNode.GetInputPins(), traitImplementation.mEventGraph.GetNodeGraph(), std::forward<InputTypes>(inputTypes)...);

					context->nodeExecutionQueue->Push(NodeExecutionData{ .nodeRef = NodeRef{ FlyTraitImplementation::_InputNodeID, traitImplementation.mEventGraph.GetNodeGraph() }, .triggerReason = eNodeTriggerReason::Flow});


					return OutputType{};
				}
				}, *traitImplementation);
		}

		template<typename TraitType>
		static constexpr RegisterFunctionNode RegisterTrait(std::string traitName)
		{
			Internal::GetTraitManager().CreateTrait(traitName, GetDataTypeID<TraitType>());

			TypeParameters typeParameters;
			typeParameters.mName = traitName;
			typeParameters.mColor = Colors::Orange;
			typeParameters.mIsTargetable = false;
			typeParameters.mRegisterPointer = false;
			Internal::GetDataTypeManager().Register<TraitModel<TraitType>>(typeParameters);

			//static_assert(ViewAndEditable<TraitModel<TraitType>>);

			Internal::GetDataTypeManager().CreateDataTypeDuplication<TraitObject>();

			return RegisterFunctionNode();
		}

		template<typename TraitType, typename OutputType, typename... InputTypes, typename... Extra>
		static constexpr RegisterFunctionNode RegisterTraitFunction(FuncPtrMember<TraitType, OutputType, InputTypes...>, const std::string& functionName, [[maybe_unused]] Extra&&... aExtraTypes)
		{
			const TraitID traitID = Internal::GetTraitManager().GetTraitIDByDataTypeID(GetDataTypeID<TraitType>());

			Trait& trait = Internal::GetTraitManager().GetTrait(traitID);

			const size_t functionIndex = trait.mFunctions.size();
			TraitFunction& traitFunction = trait.mFunctions.emplace_back();

			NodeCreationData nodeCreationData;
			nodeCreationData.mDescription.mDefaultValues.resize(2);
			TraitModel<TraitType> t;
			t.mTraitID = traitID;
			t.mFunctionIndex = functionIndex;
			nodeCreationData.mDescription.mDefaultValues[1] = t;
			nodeCreationData.mTraitID = traitID;
			nodeCreationData.mName = functionName;

			const NodeTypeID callerNodeTypeID = RegisterSystemNodeType<eNodeTrait::HasImplicitFlow>(CallerTraitNode<TraitType, OutputType, InputTypes...>, std::move(nodeCreationData));

			Internal::GetNodeTypeManager().MapNodeTypeIDToTrait(traitID, GetDataTypeID<TraitObject>(), callerNodeTypeID);

			traitFunction.mCallerNodeTypeID = callerNodeTypeID;


			return RegisterFunctionNode();
		}

		template<typename TraitImplOf, typename OutputType, typename... InputTypes>
		static constexpr RegisterFunctionNode RegisterTraitImplementation(FuncPtr<OutputType, InputTypes...> function)
		{
			using TraitType = TraitImplOf::type;


			const TraitID traitID = Internal::GetTraitManager().GetTraitIDByDataTypeID(GetDataTypeID<TraitType>());

			Trait& trait = Internal::GetTraitManager().GetTrait(traitID);


			using OverloadedT = TypeAtIndex_t<0, InputTypes...>;

			const DataTypeID overloadedTypeID = GetDataTypeID<OverloadedT>();



			Internal::GetTraitManager().CreateTraitImplementation(overloadedTypeID, traitID);

			const NodeTypeID traitObjectNodeTypeID = Internal::GetNodeTypeManager().GetNodeTypeIDByTraitAndDataType(traitID, GetDataTypeID<TraitObject>());
			const std::string traitFunctionName = Internal::GetNodeTypeManager().GetShortName(traitObjectNodeTypeID);

			NodeCreationData nodeCreationData;
			nodeCreationData.mName = trait.mName + "/" + traitFunctionName + " (" + Internal::GetDataTypeManager().Find(overloadedTypeID)->Name() + ")";
			nodeCreationData.mTraitID = traitID;


			

			const NodeTypeID nodeTypeID = RegisterSystemNodeType<eNodeTrait::HasImplicitFlow | eNodeTrait::Trait>(function, std::move(nodeCreationData));

			const NodeType& traitObjectNodeType = Internal::GetNodeType(traitObjectNodeTypeID);
			const NodeType& createdNodeType = Internal::GetNodeType(nodeTypeID);

			auto areParameterTypesEqual = [](const std::vector<PinTypeID>& aPinTypeIDs1, const std::vector<PinTypeID>& aPinTypeIDs2, const size_t aExceptionIndex = std::numeric_limits<size_t>::max()) -> bool
				{
					if (aPinTypeIDs1.size() != aPinTypeIDs2.size())
					{
						return false;
					}
					for (size_t i = 0; i < aPinTypeIDs1.size(); i++)
					{
						if (i == aExceptionIndex)
						{
							continue;
						}
						if (Internal::GetPinType(aPinTypeIDs1[i]).GetDataTypeID() != Internal::GetPinType(aPinTypeIDs2[i]).GetDataTypeID())
						{
							return false;
						}
					}

					return true;
				};

			if (!areParameterTypesEqual(traitObjectNodeType.GetInputPinTypeIDs(), createdNodeType.GetInputPinTypeIDs(), 1))
			{
				assert(false && "Input Parameter types are not equal");
			}
			if (!areParameterTypesEqual(traitObjectNodeType.GetOutputPinTypeIDs(), createdNodeType.GetOutputPinTypeIDs()))
			{
				assert(false && "Output Parameter types are not equal");
			}
			


			Internal::GetNodeTypeManager().MapNodeTypeIDToTrait(traitID, overloadedTypeID, nodeTypeID);


			return RegisterFunctionNode();
		}

	};



}

#define FLY_FUNCTION(Function, ...) \
    inline FLY_NAMESPACE::RegisterFunctionNode FLY_UNIQUE_NAME(fly_function) = FLY_NAMESPACE::RegisterFunctionNode::RegisterFunction(&Function, #Function, __VA_ARGS__);

#define FLY_TRAIT(Trait, ...) \
	inline static FLY_NAMESPACE::RegisterFunctionNode FLY_UNIQUE_NAME(fly_trait) = FLY_NAMESPACE::RegisterFunctionNode::RegisterTrait<Trait>(#Trait);

#define FLY_TRAIT_FUNCTION(Function, ...) \
	inline static FLY_NAMESPACE::RegisterFunctionNode FLY_UNIQUE_NAME(fly_trait_function) = FLY_NAMESPACE::RegisterFunctionNode::RegisterTraitFunction(&Function, #Function, __VA_ARGS__);

#define FLY_TRAIT_IMPLEMENTATION(Function, TraitImplOf) \
	inline static FLY_NAMESPACE::RegisterFunctionNode FLY_UNIQUE_NAME(fly_trait_impl) = FLY_NAMESPACE::RegisterFunctionNode::RegisterTraitImplementation<TraitImplOf>(&Function);