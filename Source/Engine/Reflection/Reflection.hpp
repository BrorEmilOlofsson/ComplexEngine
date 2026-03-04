#pragma once
#include "DataTypeRegistry.hpp"
#include "Engine/Utility/MacroUtility.hpp"

namespace Simple
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
        Simple::__RegisterComponent::RegisterComponent<ComponentType>(__VA_ARGS__); \
        return true; \
    }();

#define REGISTER_DATATYPE(dataType) inline Simple::__RegisterDataType<dataType> Reflection_ECS_Registered_Datatype_##dataType;

#define REGISTER_MEMBER(VARIABLE, ...) \
	static Simple::__RegisterMemberVariable COMBINE_FOR_UNIQUE_NAME(Reflection_ECS_Registered_Property_, __COUNTER__) = \
		Simple::__RegisterMemberVariable(&VARIABLE, Simple::ExtractVariableNameFromDataTypeName(CONVERT_TO_STRING(VARIABLE)), __VA_ARGS__);

//TO-DO(v11.0.3): maybe figure out a more modular way to register different type of const, pointers and array of different sizes
#define REGISTER_DATATYPE_CONST_POINTER(dataType) inline Simple::__RegisterDataType<const dataType*> Reflection_ECS_Registered_Datatype_Const_Pointer_##dataType;
#define REGISTER_DATATYPE_POINTER(dataType) inline Simple::__RegisterDataType<dataType*> Reflection_ECS_Registered_Datatype_Pointer_##dataType;
#define REGISTER_DATATYPE_ARRAY(array, dataType, size) inline Simple::__RegisterDataType<array<dataType, size>> Reflection_ECS_Registered_Datatype_Array_##array;
#define REGISTER_DATATYPE_ARRAY_POINTER(aArray, dataType, aSize) inline Simple::__RegisterDataType<aArray<dataType*, aSize>> Reflection_ECS_Registered_Datatype_Array_Const_Pointer_##aArray;
#define REGISTER_DATATYPE_ARRAY_CONST_POINTER(aArray, dataType, aSize) inline Simple::__RegisterDataType<aArray<const dataType*, aSize>> Reflection_ECS_Registered_Datatype_Array_Const_Pointer_##aArray;