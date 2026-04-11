#pragma once
#include <vector>
#include <string_view>
#include <variant>

#include "Engine/ECS/EntityID.hpp"
#include "Engine/Utility/Index.hpp"

#include "Engine/ECS/ECS.hpp"
#include "DataTypeRegistry.hpp"
#include "Engine/Utility/Visitor.hpp"

namespace CLX
{

    using PropertyElement = std::variant<std::string_view, Index>;

    struct PropertyPath final
    {
        EntityID entityID;
        DataTypeID componentDataTypeID;
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