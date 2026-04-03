#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <concepts>
#include <type_traits>
#include <typeinfo>
#include <any>
#include <limits>
#include <functional>
#include <optional>

#include <External/nlohmann/json.hpp>

#include "Engine/Reflection/DataTypeID.hpp"
#include "Engine/Reflection/ViewAndEditResult.hpp"
#include "Engine/Utility/Algorithm.hpp"

namespace CLX
{
    using InPlaceAllocateFunction = void(*)(void* data, const void* defaultValuePtr);
    using DestroyFunction = void(*)(void* data);
    using CopyFunction = void (*)(void* destinationPtr, const void* sourcePtr);
    using SwapFunction = void (*)(void* dataPtr1, void* dataPtr2);

    class DataTypeRegistry;

    template<typename T>
    concept Editable = requires(T & data, const Blackboard & blackboard)
    {
        { ViewAndEditValue(data, blackboard) } -> std::same_as<ViewAndEditResult>;
    };

    template<typename T>
    concept Savable = requires(const T & data, const DataTypeRegistry & dataTypeRegistry)
    {
        { ToJSON(data, dataTypeRegistry) } -> std::same_as<nlohmann::json>;
    };

    template<typename T>
    concept Loadable = requires(T & data, const nlohmann::json & json, const Blackboard & blackboard)
    {
        { FromJSON(data, json, blackboard) };
    };

    struct DataTypeMemberVariable final
    {
        std::string name;
        std::string customName;
        DataTypeID dataTypeID = InvalidDataTypeID;
        std::size_t byteOffset = std::numeric_limits<std::size_t>::max();
        bool shouldExpose = true;
        bool canEdit = true;
        std::any defaultValue;
        std::optional<std::any> minValue;
        std::optional<std::any> maxValue;
        std::optional<float> editorSpeed;
        std::function<void(void* ownerPtr, void* outPtr)> getFunction;
        std::function<void(void* ownerPtr, const void* newValuePtr)> setFunction;
    };

    class DataType final
    {
    public:
        std::string name;
        std::string prettyName;
        std::vector<DataTypeMemberVariable> memberVariables;

        ViewAndEditResult(*viewAndEdit)(void* dataPtr, const Blackboard& blackboard) = nullptr;
        nlohmann::json(*toJSON)(const void* dataPtr, const class DataTypeRegistry& dataTypeRegistry) = nullptr;
        void (*fromJSON)(void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard) = nullptr;

        InPlaceAllocateFunction inplaceAllocate = nullptr;
        DestroyFunction destroy = nullptr;
        CopyFunction copy = nullptr;
        SwapFunction swap = nullptr;

        std::size_t size = std::numeric_limits<std::size_t>::max();
        std::size_t alignment = std::numeric_limits<std::size_t>::max();
        std::reference_wrapper<const std::type_info> typeInfo;
        DataTypeID containingValueDataTypeID = InvalidDataTypeID;
        bool isComponent = false;
    };

    struct MemberMetaData
    {
        std::optional<std::string> customName;
        bool shouldExpose = true;
        bool canEdit = true;
        std::optional<std::any> defaultValue;
        std::optional<std::any> minValue;
        std::optional<std::any> maxValue;
        std::optional<float> editorSpeed;
        DataTypeID dataTypeID;
        DataTypeID ownerDataTypeID;
        std::function<void(void* ownerPtr, void* outPtr)> getFunction;
        std::function<void(void* ownerPtr, const void* newValuePtr)> setFunction;
    };

    template<typename MemberType, typename OwnerType>
    [[nodiscard]] constexpr std::size_t GetByteOffset(MemberType OwnerType::* member)
    {
        return (std::size_t) & reinterpret_cast<const volatile char&>((((OwnerType*)0)->*member));
    }

    class DataTypeRegistry final
    {
    public:

        DataTypeRegistry() = default;

        ViewAndEditResult ViewAndEditData(DataTypeID dataTypeID, void* data, const Blackboard& blackboard) const;
        void LoadDataJSON(const DataType& dataType, void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard) const;
        void LoadDataJSON(DataTypeID dataTypeID, void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard) const;
        nlohmann::json SaveDataJSON(const DataType& dataType, const void* dataPtr) const;
        nlohmann::json SaveDataJSON(DataTypeID dataTypeID, const void* dataPtr) const;

        void InplaceAllocateData(DataTypeID dataTypeID, void* destinationPtr, const void* defaultValuePtr = nullptr) const;
        void CopyData(DataTypeID dataTypeID, void* destinationPtr, const void* sourcePtr) const;
        void SwapData(DataTypeID dataTypeID, void* dataPtr1, void* dataPtr2) const;
        void DestroyData(DataTypeID dataTypeID, void* dataPtr) const;

        [[nodiscard]] DataType* Find(DataTypeID dataTypeID);
        [[nodiscard]] const DataType* Find(DataTypeID dataTypeID) const;

        [[nodiscard]] DataTypeID Find(std::string_view name) const;

        template<typename T>
        [[nodiscard]] DataType* Find();

        template<typename T>
        [[nodiscard]] const DataType* Find() const;

        template<typename Filter>
        [[nodiscard]] auto GetDataTypesFiltered(Filter&& filter) const;

    public:

        [[nodiscard]] size_t GetDataTypeSize(DataTypeID dataTypeID) const;
        [[nodiscard]] InPlaceAllocateFunction GetInplaceAllocateFunction(DataTypeID dataTypeID) const;
        [[nodiscard]] DestroyFunction GetDestroyFunction(DataTypeID dataTypeID) const;
        [[nodiscard]] CopyFunction GetCopyFunction(DataTypeID dataTypeID) const;

    public:

        void Assert() const;

        template<typename T>
        void RegisterType(const bool isComponent);

        template<typename MemberType, typename ParentType>
        void RegisterMemberVariable(MemberType ParentType::* variable, const std::string& variableName, const MemberMetaData& memberMetaData);

        void RegisterMemberVariable2(const std::string& variableName, const MemberMetaData& memberMetaData);

    private:

        std::unordered_map<DataTypeID, DataType> mDataTypes;
        std::unordered_map<std::string, DataTypeID> mNameToID;
    };

    template<typename T>
    DataType* DataTypeRegistry::Find()
    {
        return Find(GetDataTypeID<T>());
    }

    template<typename T>
    const DataType* DataTypeRegistry::Find() const
    {
        return Find(GetDataTypeID<T>());
    }

    template<typename Filter>
    auto DataTypeRegistry::GetDataTypesFiltered(Filter&& filter) const
    {
        return mDataTypes | std::views::filter([filter](const auto& p) { return filter(p.second); });
    }

    // Concept that matches std::vector of any type and allocator
    template<typename T>
    concept IsVector = requires
    {
        typename T::value_type;
        requires std::same_as<T, std::vector<typename T::value_type, typename T::allocator_type>>;
    };

    template<typename T>
    void DataTypeRegistry::RegisterType(const bool isComponent)
    {
        const DataTypeID dataTypeID = GetDataTypeID<T>();
        const bool alreadyExistOrHashCollision = mDataTypes.contains(dataTypeID);

        ASSERT(alreadyExistOrHashCollision == false);

        std::string name = ConvertTypeIndexNameToPrettyName(typeid(T).name());
        std::string prettyName = RemoveSubStringIfExist(name, "Component");

        const std::type_info& typeInfo = typeid(T);

        DataTypeID containingValueDataTypeID = InvalidDataTypeID;

        constexpr bool HasContainingValue = requires (T t)
        {
            typename T::value_type;
        };
        if constexpr (HasContainingValue)
        {
            containingValueDataTypeID = GetDataTypeID<typename T::value_type>();
        }

        DataType dataType
        {
            .name = std::move(name),
            .prettyName = std::move(prettyName),
            .size = sizeof(T),
            .alignment = alignof(T),
            .typeInfo = typeInfo,
            .containingValueDataTypeID = containingValueDataTypeID,
            .isComponent = isComponent
        };

        if constexpr (Editable<T>)
        {
            dataType.viewAndEdit = [](void* dataPtr, const Blackboard& blackboard) -> ViewAndEditResult
                {
                    T* pointer = reinterpret_cast<T*>(dataPtr);
                    return ViewAndEditValue(*pointer, blackboard);
                };
        }

        if constexpr (std::is_fundamental_v<T>)
        {
            dataType.toJSON = [](const void* dataPtr, const DataTypeRegistry&) -> nlohmann::json
                {
                    const T* pointer = reinterpret_cast<const T*>(dataPtr);
                    return ::ToJSON(*pointer);
                };
        }
        else if constexpr (Savable<T>)
        {
            dataType.toJSON = [](const void* dataPtr, const DataTypeRegistry& dataTypeRegistry) -> nlohmann::json
                {
                    const T* pointer = reinterpret_cast<const T*>(dataPtr);
                    return ToJSON(*pointer, dataTypeRegistry);
                };
        }

        if constexpr (std::is_fundamental_v<T>)
        {
            dataType.fromJSON = [](void* dataPtr, const nlohmann::json& json, const Blackboard&)
                {
                    T* pointer = reinterpret_cast<T*>(dataPtr);
                    ::FromJSON(*pointer, json);
                };
        }
        else if constexpr (Loadable<T>)
        {
            dataType.fromJSON = [](void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard)
                {
                    T* pointer = reinterpret_cast<T*>(dataPtr);
                    FromJSON(*pointer, json, blackboard);
                };
        }

        mNameToID[dataType.name] = dataTypeID;
        mDataTypes.emplace(dataTypeID, std::move(dataType));

        if constexpr (!IsVector<T>)
        {
            RegisterType<std::vector<T>>(false);
        }
    }

    template<typename MemberType, typename OwnerType>
    void DataTypeRegistry::RegisterMemberVariable(MemberType OwnerType::* variable, const std::string& variableName, const MemberMetaData& memberMetaData)
    {
        DataTypeMemberVariable member
        {
            .name = variableName,
            .dataTypeID = GetDataTypeID<MemberType>(),
            .byteOffset = GetByteOffset(variable),
            .shouldExpose = memberMetaData.shouldExpose,
            .canEdit = memberMetaData.canEdit,
            .minValue = memberMetaData.minValue,
            .maxValue = memberMetaData.maxValue,
            .editorSpeed = memberMetaData.editorSpeed,
            .setFunction = memberMetaData.setFunction
        };
        
        if (memberMetaData.customName.has_value())
        {
            member.customName = std::move(memberMetaData.customName.value());
        }
        else
        {
            member.customName = ConvertAndAddSpaceToSubStringWithUpperCase(variableName);
        }

        ASSERT(Find<OwnerType>() != nullptr);

        auto& ownerMembers = Find<OwnerType>()->memberVariables;
        auto it = std::ranges::find_if(ownerMembers, [&variableName](const DataTypeMemberVariable& obj) { return obj.name == variableName; });

        if (it == end(ownerMembers))
        {
            ownerMembers.push_back(member);
        }
    }

    inline void DataTypeRegistry::RegisterMemberVariable2(const std::string& variableName, const MemberMetaData& memberMetaData)
    {
        DataTypeMemberVariable member
        {
            .name = variableName,
            .dataTypeID = memberMetaData.dataTypeID,
            .byteOffset = 0,
            .shouldExpose = memberMetaData.shouldExpose,
            .canEdit = memberMetaData.canEdit,
            .minValue = memberMetaData.minValue,
            .maxValue = memberMetaData.maxValue,
            .editorSpeed = memberMetaData.editorSpeed,
            .getFunction = memberMetaData.getFunction,
            .setFunction = memberMetaData.setFunction
        };

        if (memberMetaData.customName.has_value())
        {
            member.customName = std::move(memberMetaData.customName.value());
        }
        else
        {
            member.customName = ConvertAndAddSpaceToSubStringWithUpperCase(variableName);
        }

        ASSERT(memberMetaData.ownerDataTypeID != InvalidDataTypeID);

        auto& ownerMembers = Find(memberMetaData.ownerDataTypeID)->memberVariables;
        auto it = std::ranges::find_if(ownerMembers, [&variableName](const DataTypeMemberVariable& obj) { return obj.name == variableName; });

        if (it == end(ownerMembers))
        {
            ownerMembers.push_back(member);
        }
    }
}