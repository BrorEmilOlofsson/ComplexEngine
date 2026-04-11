#pragma once
#include <vector>
#include <functional>

#include "Engine/Utility/MacroUtility.hpp"
#include "DataTypeRegistry.hpp"
#include "Engine/ECS/ECS.hpp"

namespace CLX
{

    struct IsDefaultComponent {};

    template<typename T>
    struct MinValue
    {
        T value;
    };

    template<typename T>
    struct MaxValue
    {
        T value;
    };

    struct EditorSpeed
    {
        float speed;
    };

    struct CustomGetSetFunctions
    {

        template<typename MemberType, typename OwnerType>
        CustomGetSetFunctions(void* (OwnerType::* getFunction)(OwnerType* owner), void(OwnerType::* setFunction)(OwnerType*, const void* newValuePtr))
        {
            this->getFunction = [getFunction](void* ownerPtr, void* outPtr)
                {
                    OwnerType* owner = static_cast<OwnerType*>(ownerPtr);
                    MemberType* out = static_cast<MemberType*>(outPtr);
                    *out = std::invoke(getFunction, owner);
                };
            this->setFunction = [setFunction](void* ownerPtr, const void* newValuePtr)
                {
                    OwnerType* owner = static_cast<OwnerType*>(ownerPtr);
                    const MemberType& newValue = *static_cast<const MemberType*>(newValuePtr);
                    std::invoke(setFunction, owner, newValue);
                };
        }
        std::function<void(void* ownerPtr, void* outPtr)> getFunction;
        std::function<void(void* ownerPtr, const void* newValuePtr)> setFunction;
    };

    struct CustomVariableData
    {

        template<typename MemberType, typename OwnerType>
        CustomVariableData(std::string name, MemberType (OwnerType::*  getFunction)() const, void(OwnerType::* setFunction)(MemberType))
            : name(std::move(name))
        {
            this->getFunction = [getFunction](void* ownerPtr, void* outPtr)
                {
                    OwnerType* owner = static_cast<OwnerType*>(ownerPtr);
                    MemberType* out = static_cast<MemberType*>(outPtr);
                    *out = std::invoke(getFunction, owner);
                };
            this->setFunction = [setFunction](void* ownerPtr, const void* newValuePtr)
                {
                    OwnerType* owner = static_cast<OwnerType*>(ownerPtr);
                    const MemberType& newValue = *static_cast<const MemberType*>(newValuePtr);
                    std::invoke(setFunction, owner, newValue);
                };

            dataTypeID = GetDataTypeID<MemberType>();
            ownerDataTypeID = GetDataTypeID<OwnerType>();
        }
        std::string name;
        std::function<void(void* ownerPtr, void* outPtr)> getFunction;
        std::function<void(void* ownerPtr, const void* newValuePtr)> setFunction;
        DataTypeID dataTypeID;
        DataTypeID ownerDataTypeID;
    };

    struct NoEdit {};

    struct NoExpose {};

    struct CustomName
    {
        std::string_view customName;
    };

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

        template<typename T, typename... Args>
        static constexpr std::optional<T> Extract(Args&&... args)
        {
            std::optional<T> value;
            ([&]
                {
                    if constexpr (std::is_same_v<T, std::decay_t<Args>>)
                    {
                        value = std::forward<Args>(args);
                    }
                }(), ...);
            return value;
        }

        template<typename MemberType, typename Owner, typename... Args>
        static bool RegisterMember(MemberType Owner::* variable, std::string variableName, Args&&... args)
        {
            sMemberRegistrationFunctions.push_back([variable, variableName, args...](DataTypeRegistry& dataTypeRegistry)
                {
                    const bool shouldExpose = !(std::is_same_v<NoExpose, std::decay_t<Args>> || ...);
                    const bool canEdit = !(std::is_same_v<NoEdit, std::decay_t<Args>> || ...);
                    std::optional<std::string> customName;
                    ((std::is_same_v<CustomName, std::decay_t<Args>> ? customName = args.customName : void()), ...);
                    std::optional<MemberType> minValue = Extract<MinValue<MemberType>>(args...).transform([](const auto& min) { return min.value; });
                    std::optional<MemberType> maxValue = Extract<MaxValue<MemberType>>(args...).transform([](const auto& max) { return max.value; });

                    std::optional<CustomGetSetFunctions> customGetSetFunctions = Extract<CustomGetSetFunctions>(std::forward<Args>(args)...);

                    MemberMetaData memberMetaData
                    {
                        .customName = customName,
                        .shouldExpose = shouldExpose,
                        .canEdit = canEdit,
                        .minValue = minValue.has_value() ? std::make_optional(std::any(std::move(minValue.value()))) : std::nullopt,
                        .maxValue = maxValue.has_value() ? std::make_optional(std::any(std::move(maxValue.value()))) : std::nullopt,
                        .editorSpeed = Extract<EditorSpeed>(args...).transform([](const auto& editorSpeed) { return editorSpeed.speed; }),
                        .getFunction = customGetSetFunctions.transform([](const auto& customGetSetFunctions) { return customGetSetFunctions.getFunction; }).value_or(nullptr),
                        .setFunction = customGetSetFunctions.transform([](const auto& customSetFunction) { return customSetFunction.setFunction; }).value_or(nullptr)
                    };

                    dataTypeRegistry.RegisterMemberVariable(
                        variable,
                        variableName,
                        memberMetaData
                    );
                });
            return true;
        }

        template<typename... Args>
        static bool RegisterMember2(CustomVariableData customVariableData, Args&&... args)
        {
            sMemberRegistrationFunctions.push_back([customVariableData, args...](DataTypeRegistry& dataTypeRegistry)
                {
                    const bool shouldExpose = !(std::is_same_v<NoExpose, std::decay_t<Args>> || ...);
                    const bool canEdit = !(std::is_same_v<NoEdit, std::decay_t<Args>> || ...);
                   
                    MemberMetaData memberMetaData
                    {
                        .customName = std::string(customVariableData.name),
                        .shouldExpose = shouldExpose,
                        .canEdit = canEdit,
                        .editorSpeed = Extract<EditorSpeed>(args...).transform([](const auto& editorSpeed) { return editorSpeed.speed; }),
                        .dataTypeID = customVariableData.dataTypeID,
                        .ownerDataTypeID = customVariableData.ownerDataTypeID,
                        .getFunction = customVariableData.getFunction,
                        .setFunction = customVariableData.setFunction,
                    };

                    dataTypeRegistry.RegisterMemberVariable2(
                        std::string(customVariableData.name),
                        memberMetaData
                    );
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


#define REGISTER_MEMBER_2(VARIABLE_DATA, ...) \
	static bool COMBINE_FOR_UNIQUE_NAME(Reflection_ECS_Registered_Property_, __COUNTER__) = \
		CLX::TypeRegistration::RegisterMember2(VARIABLE_DATA, __VA_ARGS__);

#define REGISTER_DATATYPE_CONST_POINTER(DATATYPE) inline bool Reflection_ECS_Registered_Datatype_Const_Pointer_##DATATYPE = CLX::TypeRegistration::RegisterType<const DATATYPE*>();
#define REGISTER_DATATYPE_POINTER(DATATYPE) inline bool Reflection_ECS_Registered_Datatype_Pointer_##DATATYPE = CLX::TypeRegistration::RegisterType<DATATYPE*>();
#define REGISTER_DATATYPE_ARRAY(ARRAY, DATATYPE, SIZE) inline bool Reflection_ECS_Registered_Datatype_Array_##ARRAY = CLX::TypeRegistration::RegisterType<ARRAY<DATATYPE, SIZE>>();
#define REGISTER_DATATYPE_ARRAY_POINTER(ARRAY, DATATYPE, SIZE) inline bool Reflection_ECS_Registered_Datatype_Array_Pointer_##ARRAY = CLX::TypeRegistration::RegisterType<ARRAY<DATATYPE*, SIZE>>();
#define REGISTER_DATATYPE_ARRAY_CONST_POINTER(ARRAY, DATATYPE, SIZE) inline bool Reflection_ECS_Registered_Datatype_Array_Const_Pointer_##ARRAY = CLX::TypeRegistration::RegisterType<ARRAY<const DATATYPE*, SIZE>>();