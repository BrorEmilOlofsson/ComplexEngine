#pragma once
#include <vector>
#include <string_view>
#include <variant>
#include <format>

//#include "Engine/ECS/EntityID.hpp"
#include "Engine/Utility/Index.hpp"

//#include "Engine/ECS/ECS.hpp"
//#include "DataTypeRegistry.hpp"
#include "Engine/Reflection/DataTypeID.hpp"
#include "Engine/Utility/Visitor.hpp"

namespace CLX
{

    using PropertyElement = std::variant<std::string_view, Index>;

    struct PropertyPath final
    {
        DataTypeID dataTypeID;
        std::vector<PropertyElement> elements;
    };

    /*void* GetPropertyPtr(void* currentDataPtr, const PropertyElement& element)
    {
        if (member.name == std::get<std::string>(element))
        {
            return static_cast<char*>(currentDataPtr) + member.byteOffset;
        }
    }*/

    //inline std::pair<void*, const DataType*> GetPropertyPtr(void* currentDataPtr, const DataType& dataType, std::string_view name)
    //{
    //    for (auto& member : dataType.memberVariables)
    //    {
    //        if (member.name == name)
    //        {
    //            return { static_cast<char*>(currentDataPtr) + member.byteOffset, &dataType };
    //        }
    //    }
    //    return { nullptr, nullptr };
    //}

    //inline void* GetProperty(ECS& ecs, const DataTypeRegistry& dataTypeRegistry, const PropertyPath& path)
    //{
    //    const DataType* dataType = dataTypeRegistry.Find(path.componentDataTypeID);
    //    ASSERT(dataType != nullptr);

    //    void* currentDataPtr = ecs.GetComponent(path.entityID, path.componentDataTypeID);

    //    for (auto& element : path.elements)
    //    {
    //        std::visit(Visitor
    //            {
    //                [&](const std::string_view memberName)
    //                {
    //                    auto [ptr, newDataType] = GetPropertyPtr(currentDataPtr, *dataType, memberName);
    //                    currentDataPtr = ptr;
    //                    dataType = newDataType;
    //                },
    //                [&](const Index& index)
    //                {
    //                    index;
    //                    ASSERT(false);
    //                //currentDataPtr = GetPropertyPtr(currentDataPtr, element);
    //                }
    //            }, element);
    //    }

    //    return currentDataPtr;
    //}
}

template<>
struct std::formatter<CLX::PropertyPath> : std::formatter<std::string, char>
{
    template<typename FormatContext>
    [[nodiscard]] auto format(const CLX::PropertyPath& propertyPath, FormatContext& ctx) const
    {
        std::string result;/*"EntityID: " + std::to_string(static_cast<uint32_t>(propertyPath.entityID.id)) + ", ComponentDataTypeID: " + std::to_string(propertyPath.componentDataTypeID.typeIndex.hash_code());*/
        for (const auto& element : propertyPath.elements)
        {
            result += " -> ";
            std::visit(CLX::Visitor
                {
                    [&](const std::string_view memberName)
                    {
                        result += memberName;
                    },
                    [&](const CLX::Index& index)
                    {
                        result += "[" + std::to_string(index.value) + "]";
                    }
                }, element);
        }
        return std::format_to(ctx.out(), "{}", result);
    }
};