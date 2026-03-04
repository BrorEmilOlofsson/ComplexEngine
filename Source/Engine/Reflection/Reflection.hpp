#pragma once
#include "DataTypeRegistry.hpp"
#include "Engine/Utility/MacroUtility.hpp"

namespace CLX
{

	struct IsDefaultComponent {};

	class __RegisterComponent final
	{
	public:
		template<typename T, typename... Args>
		static void RegisterComponent(Args&&...)
		{
			const bool isDefault = (std::is_same_v<IsDefaultComponent, std::decay_t<Args>> || ...);
			DataTypeRegistry::GetInstance().RegisterComponentType<T>(isDefault);
		}
	};

	template<typename T>
	class __RegisterDataType final
	{
	public:
		__RegisterDataType()
		{
			DataTypeRegistry::GetInstance().RegisterDataType<T>();
		}
	};

	class __RegisterMemberVariable final
	{
	public:
		template<typename MemberType, typename Owner>
		__RegisterMemberVariable(MemberType Owner::* variable, const char* variableName, const char* customName = nullptr, const bool shouldExpose = true, const bool canEdit = true)
		{
			DataTypeRegistry::GetInstance().RegisterMemberVariable(variable, variableName, customName, shouldExpose, canEdit);
		}
	};

	template<size_t N>
	constexpr const char* ExtractVariableNameFromDataTypeName(const char(&name)[N])
	{
		for (size_t i = N - 1; i > 0; --i)
		{
			if (name[i - 1] == ':')
			{
				return name + i;
			}
		}

		return name;
	}
}

#define REGISTER_COMPONENT(ComponentType, ...) \
    static bool ComponentRegistered_##ComponentType = []() { \
        CLX::__RegisterComponent::RegisterComponent<ComponentType>(__VA_ARGS__); \
        return true; \
    }();

#define REGISTER_DATATYPE(DATATYPE) inline CLX::__RegisterDataType<DATATYPE> Reflection_ECS_Registered_Datatype_##DATATYPE;

#define REGISTER_MEMBER(VARIABLE, ...) \
	static CLX::__RegisterMemberVariable COMBINE_FOR_UNIQUE_NAME(Reflection_ECS_Registered_Property_, __COUNTER__) = \
		CLX::__RegisterMemberVariable(&VARIABLE, CLX::ExtractVariableNameFromDataTypeName(CONVERT_TO_STRING(VARIABLE)), __VA_ARGS__);

#define REGISTER_DATATYPE_CONST_POINTER(DATATYPE) inline CLX::__RegisterDataType<const DATATYPE*> Reflection_ECS_Registered_Datatype_Const_Pointer_##DATATYPE;
#define REGISTER_DATATYPE_POINTER(DATATYPE) inline CLX::__RegisterDataType<DATATYPE*> Reflection_ECS_Registered_Datatype_Pointer_##DATATYPE;
#define REGISTER_DATATYPE_ARRAY(ARRAY, DATATYPE, SIZE) inline CLX::__RegisterDataType<ARRAY<DATATYPE, SIZE>> Reflection_ECS_Registered_Datatype_Array_##ARRAY;
#define REGISTER_DATATYPE_ARRAY_POINTER(ARRAY, DATATYPE, SIZE) inline CLX::__RegisterDataType<ARRAY<DATATYPE*, SIZE>> Reflection_ECS_Registered_Datatype_Array_Const_Pointer_##ARRAY;
#define REGISTER_DATATYPE_ARRAY_CONST_POINTER(ARRAY, DATATYPE, SIZE) inline CLX::__RegisterDataType<ARRAY<const DATATYPE*, SIZE>> Reflection_ECS_Registered_Datatype_Array_Const_Pointer_##ARRAY;