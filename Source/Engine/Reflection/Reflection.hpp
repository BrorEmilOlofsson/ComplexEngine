#pragma once
#include <vector>
#include <functional>

#include "DataTypeRegistry.hpp"
#include "Engine/Utility/MacroUtility.hpp"

namespace CLX
{

	struct IsDefaultComponent {};

	class TypeRegistration final
	{
	public:
		template<typename T, typename... Args>
		static bool RegisterComponentType(Args&&...)
		{
			sRegistrationFunctions.push_back([](DataTypeRegistry& dataTypeRegistry, ECSRegistry& ecsRegistry)
				{
					const bool isDefault = (std::is_same_v<IsDefaultComponent, std::decay_t<Args>> || ...);
					dataTypeRegistry.RegisterType<T>(true);
					ecsRegistry.RegisterComponentType<T>(isDefault);
                });
			return true;
		}
		template<typename T>
		static bool RegisterType()
		{
			sRegistrationFunctions.push_back([](DataTypeRegistry& dataTypeRegistry, ECSRegistry&)
				{
					dataTypeRegistry.RegisterType<T>(false);
				});
			return true;
		}

		template<typename MemberType, typename Owner>
		static bool RegisterMember(MemberType Owner::* variable, const char* variableName, const char* customName = nullptr, const bool shouldExpose = true, const bool canEdit = true)
		{
			sMemberRegistrationFunctions.push_back([variable, variableName, customName, shouldExpose, canEdit](DataTypeRegistry& dataTypeRegistry)
				{
					dataTypeRegistry.RegisterMemberVariable(variable, variableName, customName, shouldExpose, canEdit);
				});
			return true;
		}

		template<typename T>
		static void RegisterSystem()
		{
			sRegistrationFunctions.push_back([](DataTypeRegistry&, ECSRegistry& ecsRegistry)
				{
					ecsRegistry.RegisterSystem<T>();
                });
		}

		static void ExecuteRegistrations(DataTypeRegistry& dataTypeRegistry, ECSRegistry& ecsRegistry)
		{
			for (const auto& registrationFunction : sRegistrationFunctions)
			{
				registrationFunction(dataTypeRegistry, ecsRegistry);
			}

			for (const auto& memberRegistrationFunction : sMemberRegistrationFunctions)
			{
                memberRegistrationFunction(dataTypeRegistry);
			}
        }

	private:

        inline static std::vector<std::function<void(DataTypeRegistry&, ECSRegistry&)>> sRegistrationFunctions;
        inline static std::vector<std::function<void(DataTypeRegistry&)>> sMemberRegistrationFunctions;
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

#define REGISTER_COMPONENT(COMPONENT_TYPE, ...) \
    static bool ComponentRegistered_##COMPONENT_TYPE = []() { \
        CLX::TypeRegistration::RegisterComponentType<COMPONENT_TYPE>(__VA_ARGS__); \
        return true; \
    }();

#define REGISTER_DATATYPE(DATATYPE) inline bool  Reflection_ECS_Registered_Datatype_##DATATYPE = CLX::TypeRegistration::RegisterType<DATATYPE>();

#define REGISTER_MEMBER(VARIABLE, ...) \
	static bool COMBINE_FOR_UNIQUE_NAME(Reflection_ECS_Registered_Property_, __COUNTER__) = \
		CLX::TypeRegistration::RegisterMember(&VARIABLE, CLX::ExtractVariableNameFromDataTypeName(CONVERT_TO_STRING(VARIABLE)), __VA_ARGS__);

#define REGISTER_DATATYPE_CONST_POINTER(DATATYPE) inline bool Reflection_ECS_Registered_Datatype_Const_Pointer_##DATATYPE = CLX::TypeRegistration::RegisterType<const DATATYPE*>();
#define REGISTER_DATATYPE_POINTER(DATATYPE) inline bool Reflection_ECS_Registered_Datatype_Pointer_##DATATYPE = CLX::TypeRegistration::RegisterType<DATATYPE*>();
#define REGISTER_DATATYPE_ARRAY(ARRAY, DATATYPE, SIZE) inline bool Reflection_ECS_Registered_Datatype_Array_##ARRAY = CLX::TypeRegistration::RegisterType<ARRAY<DATATYPE, SIZE>>();
#define REGISTER_DATATYPE_ARRAY_POINTER(ARRAY, DATATYPE, SIZE) inline bool Reflection_ECS_Registered_Datatype_Array_Pointer_##ARRAY = CLX::TypeRegistration::RegisterType<ARRAY<DATATYPE*, SIZE>>();
#define REGISTER_DATATYPE_ARRAY_CONST_POINTER(ARRAY, DATATYPE, SIZE) inline bool Reflection_ECS_Registered_Datatype_Array_Const_Pointer_##ARRAY = CLX::TypeRegistration::RegisterType<ARRAY<const DATATYPE*, SIZE>>();