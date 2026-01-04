#pragma once
#include "../FlyDefines.hpp"
#include "../Utilities/FlyMeta.hpp"
#include "../Utilities/FlyUtilities.hpp"
#include "../DataType/FlyDataTypeManager.hpp"
#include "../SystemTypes/FlyWildcard.hpp"
#include "../Node/FlyNodeTrait.hpp"
#include "../Node/FlyNodeTypeRegistry.hpp"

namespace FLY_NAMESPACE
{

	struct OperatorNodes
	{
		template<HasOperator_EqualTo T>
		static constexpr inline bool EqualTo(const T& aValue1, const T& aValue2)
		{
			return aValue1 == aValue2;
		}

		template<HasOperator_NotEqualTo T>
		static constexpr inline bool NotEqualTo(const T& aValue1, const T& aValue2)
		{
			return aValue1 != aValue2;
		}

		template<HasOperator_Greater T>
		static constexpr inline bool Greater(const T& aValue1, const T& aValue2)
		{
			return aValue1 > aValue2;
		}

		template<HasOperator_Less T>
		static constexpr inline bool Less(const T& aValue1, const T& aValue2)
		{
			return aValue1 < aValue2;
		}

		template<HasOperator_GreaterEqual T>
		static constexpr  inline bool GreaterEqual(const T& aValue1, const T& aValue2)
		{
			return aValue1 >= aValue2;
		}

		template<HasOperator_LessEqual T>
		static constexpr inline bool LessEqual(const T& aValue1, const T& aValue2)
		{
			return aValue1 <= aValue2;
		}

		template<HasOperator_And T>
		static constexpr inline T And(const T& aValue1, const T& aValue2)
		{
			return aValue1 && aValue2;
		}

		template<HasOperator_Or T>
		static constexpr inline T Or(const T& aValue1, const T& aValue2)
		{
			return aValue1 || aValue2;
		}

		template<HasOperator_Not T>
		static constexpr inline T Not(const T& aValue)
		{
			return !aValue;
		}

		template<HasOperator_Print T>
		static constexpr inline void Print(const T& aValue)
		{
			std::cout << aValue << '\n';
		}

		template<HasOperator_Add T>
		static constexpr inline T Add(const T& aValue1, const T& aValue2)
		{
			return aValue1 + aValue2;
		}

		template<HasOperator_Subtract T>
		static constexpr inline T Substract(const T& aValue1, const T& aValue2)
		{
			return aValue1 - aValue2;
		}

		template<HasOperator_Multiply T>
		static constexpr inline T Multiply(const T& aValue1, const T& aValue2)
		{
			return aValue1 * aValue2;
		}

		template<HasOperator_Divide T>
		static constexpr inline T Divide(const T& aValue1, const T& aValue2)
		{
			return aValue1 / aValue2;
		}

		template<HasOperator_Modulo T>
		static constexpr inline T Modulo(const T& aValue1, const T& aValue2)
		{
			return aValue1 % aValue2;
		}

		template<HasOperator_IncrementPrefix T>
		static constexpr inline void IncrementPrefix(T* aValue)
		{
			if (!aValue)
			{
				return;
			}
			++(*aValue);
		}

		template<HasOperator_IncrementPostfix T>
		static constexpr inline void IncrementPostfix(T* aValue)
		{
			if (!aValue)
			{
				return;
			}
			(*aValue)++;
		}

		template<HasOperator_DecrementPrefix T>
		static constexpr inline void DecrementPrefix(T* aValue)
		{
			if (!aValue)
			{
				return;
			}
			--(*aValue);
		}

		template<HasOperator_DecrementPostfix T>
		static constexpr inline void DecrementPostfix(T* aValue)
		{
			if (!aValue)
			{
				return;
			}
			(*aValue)--;
		}
	};


	template<typename T>
	inline bool AssertOperator()
	{
		return false;
	}

	template<typename CastTo, IsStaticCastable<CastTo> CastFrom>
	inline CastTo StaticCastNodeTemplate(CastFrom aValue)
	{
		return static_cast<CastTo>(aValue);
	}

	template<Decayed T, eNodeOperatorType OperatorTrait>
	constexpr inline bool HasOperator()
	{
		switch (OperatorTrait)
		{
		case eNodeOperatorType::None:
			break;
		case eNodeOperatorType::Equal:
			return HasOperator_EqualTo<T>;
			break;
		case eNodeOperatorType::NotEqual:
			return HasOperator_NotEqualTo<T>;
			break;
		case eNodeOperatorType::Greater:
			return HasOperator_Greater<T>;
			break;
		case eNodeOperatorType::Less:
			return HasOperator_Less<T>;
			break;
		case eNodeOperatorType::GreaterEqual:
			return HasOperator_GreaterEqual<T>;
			break;
		case eNodeOperatorType::LessEqual:
			return HasOperator_LessEqual<T>;
			break;
		case eNodeOperatorType::And:
			return HasOperator_And<T>;
			break;
		case eNodeOperatorType::Or:
			return HasOperator_Or<T>;
			break;
		case eNodeOperatorType::Not:
			return HasOperator_Not<T>;
			break;
		case eNodeOperatorType::Print:
			return HasOperator_Print<T>;
			break;
		case eNodeOperatorType::Add:
			return HasOperator_Add<T>;
			break;
		case eNodeOperatorType::Subtract:
			return HasOperator_Subtract<T>;
			break;
		case eNodeOperatorType::Multiply:
			return HasOperator_Multiply<T>;
			break;
		case eNodeOperatorType::Divide:
			return HasOperator_Divide<T>;
			break;
		case eNodeOperatorType::Modulo:
			return HasOperator_Modulo<T>;
			break;
		case eNodeOperatorType::IncrementPrefix:
			return HasOperator_IncrementPrefix<T>;
			break;
		default:
			break;
		}
		return false;
	}

	template<Decayed T, eNodeOperatorType OperatorTrait>
	constexpr inline auto GetFunctionByOperator()
	{
		if constexpr (OperatorTrait == eNodeOperatorType::Equal)
		{
			return OperatorNodes::EqualTo<T>;
		}
		else if constexpr (OperatorTrait == eNodeOperatorType::NotEqual)
		{
			return OperatorNodes::NotEqualTo<T>;
		}
		else if constexpr (OperatorTrait == eNodeOperatorType::Greater)
		{
			return OperatorNodes::Greater<T>;
		}
		else if constexpr (OperatorTrait == eNodeOperatorType::Less)
		{
			return OperatorNodes::Less<T>;
		}
		else if constexpr (OperatorTrait == eNodeOperatorType::GreaterEqual)
		{
			return OperatorNodes::GreaterEqual<T>;
		}
		else if constexpr (OperatorTrait == eNodeOperatorType::LessEqual)
		{
			return OperatorNodes::LessEqual<T>;
		}

		else if constexpr (OperatorTrait == eNodeOperatorType::And)
		{
			return OperatorNodes::And<T>;
		}
		else if constexpr (OperatorTrait == eNodeOperatorType::Or)
		{
			return OperatorNodes::Or<T>;
		}
		else if constexpr (OperatorTrait == eNodeOperatorType::Not)
		{
			return OperatorNodes::Not<T>;
		}


		else if constexpr (OperatorTrait == eNodeOperatorType::Print)
		{
			return OperatorNodes::Print<T>;
		}


		else if constexpr (OperatorTrait == eNodeOperatorType::Add)
		{
			return OperatorNodes::Add<T>;
		}
		else if constexpr (OperatorTrait == eNodeOperatorType::Subtract)
		{
			return OperatorNodes::Substract<T>;
		}
		else if constexpr (OperatorTrait == eNodeOperatorType::Multiply)
		{
			return OperatorNodes::Multiply<T>;
		}
		else if constexpr (OperatorTrait == eNodeOperatorType::Divide)
		{
			return OperatorNodes::Divide<T>;
		}
		else if constexpr (OperatorTrait == eNodeOperatorType::Modulo)
		{
			return OperatorNodes::Modulo<T>;
		}

		else if constexpr (OperatorTrait == eNodeOperatorType::IncrementPrefix)
		{
			return OperatorNodes::IncrementPrefix<T>;
		}

		else if constexpr (OperatorTrait == eNodeOperatorType::DecrementPrefix)
		{
			return OperatorNodes::DecrementPrefix<T>();
		}

		else
		{
			return AssertOperator<T>;
		}
	}


	/*struct NodeTypeDesc;

	template<eNodeTrait, typename OutputType, typename... InputTypes>
	constexpr void RegisterSystemNodeType(FuncPtr<OutputType, InputTypes...>, NodeCreationData&& = NodeCreationData());*/


	template<Decayed T, eNodeOperatorType OperatorType, eNodeTrait ExtraTraits = eNodeTrait::None>
	constexpr inline void RegisterOperatorNode(std::string defaultNodeName)
	{

		const DataTypeID dataTypeID = GetDataTypeID<T>();

		if constexpr (!std::same_as<T, Wildcard>)
		{
			const std::string& customTypeName = Internal::GetDataTypeManager().Find(dataTypeID)->Name();
			if (!customTypeName.empty())
			{
				defaultNodeName += " (" + customTypeName + ")";
			}
			else
			{
				const std::string typeName = typeid(T).name();
				defaultNodeName += " (" + typeName + ")";
			}
		}

		constexpr eNodeTrait Traits = (std::same_as<T, Wildcard> ? eNodeTrait::None : eNodeTrait::Operator) | ExtraTraits;

		auto operatorFunc = GetFunctionByOperator<T, OperatorType>();

		const NodeTypeID nodeTypeID = RegisterSystemNodeType<Traits>(operatorFunc, NodeCreationData{ .mName = defaultNodeName, .mOperatorType = OperatorType });

		Internal::GetNodeTypeManager().SetOperatorNodeTypeID(dataTypeID, OperatorType, nodeTypeID);
	}

	template<Decayed T, eNodeOperatorType OperatorType, eNodeOperatorType RegisteredTraits, eNodeTrait ExtraTraits = eNodeTrait::None>
	constexpr inline void TryRegisterOperatorNode(const std::string& defaultNodeName)
	{
		if constexpr (HasFlag(OperatorType, RegisteredTraits))
		{
			if constexpr (HasOperator<T, OperatorType>())
			{
				RegisterOperatorNode<T, OperatorType, ExtraTraits>(defaultNodeName);
			}
		}

	}

	template<Decayed T, eNodeOperatorType RegisteredTraits>
	inline void RegisterOperatorNodeTypes()
	{
		TryRegisterOperatorNode<T, eNodeOperatorType::Equal, RegisteredTraits>("Operators/Compare/Equal To");
		TryRegisterOperatorNode<T, eNodeOperatorType::NotEqual, RegisteredTraits>("Operators/Compare/Not Equal To");
		TryRegisterOperatorNode<T, eNodeOperatorType::Greater, RegisteredTraits>("Operators/Compare/Greater Than");
		TryRegisterOperatorNode<T, eNodeOperatorType::Less, RegisteredTraits>("Operators/Compare/Less Than");
		TryRegisterOperatorNode<T, eNodeOperatorType::GreaterEqual, RegisteredTraits>("Operators/Compare/Greater Or Equal To");
		TryRegisterOperatorNode<T, eNodeOperatorType::LessEqual, RegisteredTraits>("Operators/Compare/Less Or Equal To");

		TryRegisterOperatorNode<T, eNodeOperatorType::And, RegisteredTraits>("Operators/Logical/And");
		TryRegisterOperatorNode<T, eNodeOperatorType::Or, RegisteredTraits>("Operators/Logical/Or");
		TryRegisterOperatorNode<T, eNodeOperatorType::Not, RegisteredTraits>("Operators/Logical/Not");

		TryRegisterOperatorNode<T, eNodeOperatorType::Print, RegisteredTraits, eNodeTrait::HasImplicitFlow>("Operators/Print");

		TryRegisterOperatorNode<T, eNodeOperatorType::Add, RegisteredTraits>("Operators/Math/Add");
		TryRegisterOperatorNode<T, eNodeOperatorType::Subtract, RegisteredTraits>("Operators/Math/Subtract");
		TryRegisterOperatorNode<T, eNodeOperatorType::Multiply, RegisteredTraits>("Operators/Math/Multiply");
		TryRegisterOperatorNode<T, eNodeOperatorType::Divide, RegisteredTraits>("Operators/Math/Divide");
		TryRegisterOperatorNode<T, eNodeOperatorType::Modulo, RegisteredTraits>("Operators/Math/Modulo");

		TryRegisterOperatorNode<T, eNodeOperatorType::IncrementPrefix, RegisteredTraits, eNodeTrait::HasImplicitFlow>("Operators/Math/Increment");
		TryRegisterOperatorNode<T, eNodeOperatorType::DecrementPrefix, RegisteredTraits, eNodeTrait::HasImplicitFlow>("Operators/Math/Decrement");

	}
}