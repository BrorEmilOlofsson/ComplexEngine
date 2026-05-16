#include "Engine/Precompiled/EnginePch.hpp"
#include "Engine/Reflection/EditDataFunctions.hpp"

#include "Engine/Graphics/GraphicsConstants.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Asset/AssetManager.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"
#include "Engine/Graphics/Mesh/Mesh.hpp"
#include "Engine/Graphics/Model/Model.hpp"
#include "Engine/Graphics/Model/AnimatedModel.hpp"
#include "Engine/Graphics/Texture/Texture.hpp"
#include "Engine/Graphics/Animation/Animation.hpp"
#include "Engine/Reflection/DataTypeRegistry.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/NameComponent.hpp"
#include "Engine/Reflection/VariantReflection.hpp"

#include "Engine/Utility/ImGui/ImGuiUtility.hpp"
#include "Engine/Utility/File/FileUtility.hpp"

#include <External/imgui/imgui.h>
#include <External/imgui/imguizmo/imguizmo.h>

namespace CLX
{

    constexpr static float labelWidth = 120;

    static float CalculateItemWidth(const int itemCount)
    {
        return (ImGui::GetContentRegionAvail().x - labelWidth) / itemCount;
    }

    static ViewAndEditResult CustomDragFloat1(std::string_view str, float& value, const float width, const std::string& imguiTag, Color color, float speed, float min, float max)
    {
        ViewAndEditResult viewAndEditResult;
        ImGui::PushStyleColor(ImGuiCol_Button, ToImVec4(color));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ToImVec4(color));
        const std::string xStr = std::string(str) + "##" + imguiTag;
        ImGui::Button(xStr.c_str());
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::SetNextItemWidth(width);
        const std::string floatX = "##" + std::string(str) + imguiTag;
        ImGui::PushID(&value);
        if (ImGui::DragFloat(floatX.c_str(), &value, speed, min, max, "%.3f", ImGuiSliderFlags_AlwaysClamp))
        {
            viewAndEditResult.isEdited = true;
        }
        ImGui::PopID();
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    static ViewAndEditResult CustomDragFloat3(const char* label, float& x, float& y, float& z, float speed = 0.01f, float min = 0, float max = 0)
    {
        ViewAndEditResult viewAndEditResult;

        const float width = CalculateItemWidth(3);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, (ImVec2(0, 0)));

        ViewAndEditResult viewAndEditResultX = CustomDragFloat1("X", x, width, label, Colors::Red, speed, min, max);
        viewAndEditResult.isEdited |= viewAndEditResultX.isEdited;
        viewAndEditResult.isActive |= viewAndEditResultX.isActive;
        ImGui::SameLine();
        ViewAndEditResult viewAndEditResultY = CustomDragFloat1("Y", y, width, label, Color(0, 0.7f, 0), speed, min, max);
        viewAndEditResult.isEdited |= viewAndEditResultY.isEdited;
        viewAndEditResult.isActive |= viewAndEditResultY.isActive;
        ImGui::SameLine();
        ViewAndEditResult viewAndEditResultZ = CustomDragFloat1("Z", z, width, label, Colors::Blue, speed, min, max);
        viewAndEditResult.isEdited |= viewAndEditResultZ.isEdited;
        viewAndEditResult.isActive |= viewAndEditResultZ.isActive;

        ImGui::PopStyleVar();

        ImGui::SameLine();
        ImGui::SetNextItemWidth(labelWidth);
        ImGui::PushID((std::string(label) + "CustomDragFloat3").c_str());
        ImGui::Text(label);
        ImGui::PopID();

        return viewAndEditResult;
    }

    static ViewAndEditResult CustomDragFloat3(const char* label, Vector3f& vector, float speed = 0.01f, float min = 0, float max = 0)
    {
        return CustomDragFloat3(label, vector.x, vector.y, vector.z, speed, min, max);
    }

    static ViewAndEditResult CustomDragFloat3(const char* label, Point3f& point)
    {
        return CustomDragFloat3(label, point.x, point.y, point.z);
    }

    static ViewAndEditResult CustomDragFloat3(const char* label, Rotatorf& rotator)
    {
        return CustomDragFloat3(label, rotator.Pitch().Value(), rotator.Yaw().Value(), rotator.Roll().Value());
    }
}

namespace CLX
{
    static std::string ExtractStringFromImGuiIDFullName(const std::string& string)
    {
        const size_t position = string.find("##");

        std::string variableNameWithoutImGuiID;

        if (position != std::string::npos)
        {
            return string.substr(0, position);
        }

        return string;
    }

    static ViewAndEditResult ViewAndEditDataPtr(const DataTypeID dataTypeID, void* dataPtr, const Blackboard& blackboard, DataTypeRegistry& dataTypeRegistry)
    {
        return dataTypeRegistry.ViewAndEditData(dataTypeID, dataPtr, blackboard);
    }

    ViewAndEditResult ViewAndEditDataPtr(const DataTypeID dataTypeID, void* dataPtr, const Blackboard& blackboard)
    {
        return ViewAndEditDataPtr(dataTypeID, dataPtr, blackboard, blackboard.Get<Key_DataTypeRegistry>());
    }

    ViewAndEditResult ViewAndEditValue(bool& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        viewAndEditResult.isEdited = ImGui::Checkbox(variableName.c_str(), &value);
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(bool& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(char& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        const std::string constCharToString(1, value);

        std::string variableNameWithoutImGuiID = ExtractStringFromImGuiIDFullName(variableName);
        variableNameWithoutImGuiID += ": " + constCharToString;

        char c[1]{ value };
        if (ImGui::InputText(variableNameWithoutImGuiID.c_str(), c, 1))
        {
            value = c[0];
            viewAndEditResult.isEdited;
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(char& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(int& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        viewAndEditResult.isEdited = ImGui::DragInt(variableName.c_str(), &value);
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(int& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(float& value, const std::string& variableName, float speed, float min, float max)
    {
        const float width = CalculateItemWidth(1);
        ImGui::SetNextItemWidth(width);
        ViewAndEditResult viewAndEditResult;
        viewAndEditResult.isEdited = ImGui::DragFloat(variableName.c_str(), &value, speed, min, max);
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }


    ViewAndEditResult ViewAndEditValue(float& value, const std::string& variableName)
    {
        float speed = 0.1f;
        float min = 0.f;
        float max = 0.f;
        return ViewAndEditValue(value, variableName, speed, min, max);
    }

    ViewAndEditResult ViewAndEditValue(float& value, const DataTypeMemberVariable* memberData)
    {
        float speed = 0.1f;
        float min = 0.f;
        float max = 0.f;
        return ViewAndEditValue(value, memberData ? memberData->customName : "", speed, min, max);
    }

    ViewAndEditResult ViewAndEditValue(std::string& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        char buffer[256]{};
        memset(buffer, '\0', sizeof(buffer));
        strncpy_s(buffer, value.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';

        if (ImGui::InputTextWithHint(variableName.c_str(), "Text", buffer, sizeof(buffer)))
        {
            value = std::string(buffer);
            viewAndEditResult.isEdited = true;
        }

        viewAndEditResult.isActive = ImGui::IsItemActive();

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(std::string& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(Vector2f& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        viewAndEditResult.isEdited = ImGui::DragFloat2(variableName.c_str(), &value.x);
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(Vector2f& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(Vector2i& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        viewAndEditResult.isEdited = ImGui::DragInt2(variableName.c_str(), &value.x);
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(Vector2i& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(Vector3f& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        viewAndEditResult.isEdited = ImGui::DragFloat3(variableName.c_str(), &value.x, 0.01f);
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(Vector3f& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(Vector4f& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        viewAndEditResult.isEdited = ImGui::DragFloat4(variableName.c_str(), &value.x);
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(Vector4f& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(Point2f& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        viewAndEditResult.isEdited = ImGui::DragFloat2(variableName.c_str(), &value.x);
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(Point2f& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(Point2i& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        viewAndEditResult.isEdited = ImGui::DragInt2(variableName.c_str(), &value.x);
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(Point2i& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(Point3f& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        viewAndEditResult.isEdited = ImGui::DragFloat3(variableName.c_str(), &value.x, 0.01f);
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(Point3f& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(UnitVector2f& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        Vector2f values = value * 1.f;
        ImGui::SetNextItemWidth(CalculateItemWidth(1));
        if (ImGui::DragFloat2(variableName.c_str(), &values.x))
        {
            if (values != Vector2f::Zero())
            {
                value = UnitVector2f(values);
                viewAndEditResult.isEdited = true;
            }
        }
        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(UnitVector2f& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult VisualizeInScene(UnitVector3f& value, const Point3f& origin, const Camera& camera, RenderList& renderList)
    {
        ViewAndEditResult viewAndEditResult;
        Matrix4x4f matrix = Matrix4x4f::CreateTRMatrix(origin, RotationMatrix3f::FromZ(value));
        if (ImGuizmo::Manipulate(camera.GetViewMatrix().GetDataPtr(), camera.GetProjectionMatrix().GetDataPtr(), ImGuizmo::OPERATION::ROTATE, ImGuizmo::MODE::WORLD, matrix.GetDataPtr()))
        {
            value = matrix.GetForward();
            viewAndEditResult.isEdited = true;
        }

        viewAndEditResult.isActive = ImGuizmo::IsUsing();

        {
            const Point3f start = origin;
            const Point3f end = start + value;
            const Color color = Colors::Orange;
            renderList.AddArrow(DrawArrow{ start, end, color });
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(UnitVector3f& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;
        Vector3f values = value * 1.f;
        ImGui::SetNextItemWidth(CalculateItemWidth(1));
        if (ImGui::DragFloat3(variableName.c_str(), &values.x))
        {
            if (values != Vector3f::Zero())
            {
                value = UnitVector3f(values);
                viewAndEditResult.isEdited = true;
            }
        }
        viewAndEditResult.isActive = ImGui::IsItemActive();

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(UnitVector3f& value, const std::string& variableName, bool& showUnitVectorInScene, const Point3f& referencePoint, const Camera& currentCamera, RenderList& renderList)
    {
        ViewAndEditResult viewAndEditResult = ViewAndEditValue(value, variableName);

        ImGui::Checkbox("Show UnitVector in scene", &showUnitVectorInScene);
        if (!showUnitVectorInScene)
        {
            return viewAndEditResult;
        }

        ViewAndEditResult viewAndEditResultScene = VisualizeInScene(value, referencePoint, currentCamera, renderList);
        viewAndEditResult.isEdited |= viewAndEditResultScene.isEdited;
        viewAndEditResult.isActive |= viewAndEditResultScene.isActive;

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(UnitVector3f& value, const std::string& variableName, const Blackboard& blackboard)
    {
        const Transform& referenceTransform = blackboard.Get<Key_ReferenceTransform>();
        const Camera& currentCamera = blackboard.Get<Key_CurrentCamera>();
        RenderState& renderState = blackboard.Get<Key_CurrentRenderState>();
        bool& showInScene = blackboard.Get<Key_ShowUnitVectorInScene>();
        return ViewAndEditValue(value, variableName, showInScene, referenceTransform.GetPosition(), currentCamera, renderState.GetRenderList());
    }

    ViewAndEditResult ViewAndEditValue(UnitVector3f& value, const Blackboard& blackboard, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "", blackboard);
    }

    ViewAndEditResult ViewAndEditValue(Color& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;

        if (ImGui::ColorEdit4(variableName.c_str(), &value.r))
        {
            viewAndEditResult.isEdited = true;
        }

        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(Color& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(RGBColor& value, const std::string& variableName)
    {
        ViewAndEditResult viewAndEditResult;

        if (ImGui::ColorEdit3(variableName.c_str(), &value.r))
        {
            viewAndEditResult.isEdited = true;
        }

        viewAndEditResult.isActive = ImGui::IsItemActive();
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(RGBColor& value, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(value, memberData ? memberData->customName : "");
    }

    ViewAndEditResult ViewAndEditValue(Transform& value, const DataTypeMemberVariable* memberData)
    {
        memberData;
        ViewAndEditResult viewAndEditResult;

        {
            Point3f position = value.GetPosition();

            const ViewAndEditResult viewAndEditPosition = CustomDragFloat3("Position", position);
            viewAndEditResult.isEdited |= viewAndEditPosition.isEdited;
            viewAndEditResult.isActive |= viewAndEditPosition.isActive;

            if (viewAndEditPosition.isEdited)
            {
                viewAndEditResult.isEdited = true;
                value.SetPosition(position);
            }
        }

        {
            Rotatorf rotation = ToRotator(value.GetRotation());

            const ViewAndEditResult viewAndEditRotation = CustomDragFloat3("Rotation", rotation);
            viewAndEditResult.isEdited |= viewAndEditRotation.isEdited;
            viewAndEditResult.isActive |= viewAndEditRotation.isActive;

            if (viewAndEditRotation.isEdited)
            {
                value.SetRotation(ToRotationMatrix(rotation));
            }
        }

        {
            Vector3f scale = value.GetScale();

            const ViewAndEditResult viewAndEditScale = CustomDragFloat3("Scale", scale, 0.01f, 0.1f, 0.f);
            viewAndEditResult.isEdited |= viewAndEditScale.isEdited;
            viewAndEditResult.isActive |= viewAndEditScale.isActive;

            if (viewAndEditScale.isEdited)
            {
                scale = Max(scale, Vector3f(0.001f)); // Prevent scale from being zero or negative

                value.SetScale(scale);
                std::println("{}", scale);
            }
        }

        return viewAndEditResult;
    }

    static ViewAndEditResult ViewAndEditValue(EntityID& entityID, const ECS& ecs, const std::string& variableName)
    {
        ImGui::Text(variableName.c_str());
        ImGui::SameLine();
        std::string btnStr;
        if (entityID == InvalidEntityID)
        {
            btnStr = "None";
        }
        else
        {
            if (ecs.IsEntityValid(entityID))
            {
                const NameComponent* nameComponent = ecs.GetComponent<NameComponent>(entityID);
                ASSERT_NEW(nameComponent, "Entity does not have a NameComponent");
                btnStr = nameComponent->name + " (" + std::to_string(entityID.id) + ")";
            }
            else
            {
                btnStr = "Invalid Entity (" + std::to_string(entityID.id) + ")";
            }
        }
        ImGui::Button(btnStr.c_str());
        ViewAndEditResult viewAndEditResult;

        if (entityID != InvalidEntityID)
        {
            ObjectSource(entityID, variableName.c_str());
        }
        if (auto result = ObjectTarget<EntityID>())
        {
            viewAndEditResult.isEdited = true;
            entityID = result.value();
        }
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(EntityID& entityID, const Blackboard& blackboard, const DataTypeMemberVariable* memberData)
    {
        const ECS& ecs = blackboard.Get<Key_CurrentECS>();
        return ViewAndEditValue(entityID, ecs, memberData ? memberData->customName : "");
    }


    ViewAndEditResult ViewAndEditValue(PointLight& pointLight, const DataTypeMemberVariable* memberData)
    {
        memberData;
        ViewAndEditResult viewAndEditResult;
        ViewAndEditResult viewAndEditResultColor = ViewAndEditValue(pointLight.color, "Color");
        viewAndEditResult.isEdited |= viewAndEditResultColor.isEdited;
        viewAndEditResult.isActive |= viewAndEditResultColor.isActive;
        ViewAndEditResult viewAndEditResultIntensity = ViewAndEditValue(pointLight.intensity, "Intensity", 0.01f, 0.0f, 10.f);
        viewAndEditResult.isEdited |= viewAndEditResultIntensity.isEdited;
        viewAndEditResult.isActive |= viewAndEditResultIntensity.isActive;
        ViewAndEditResult viewAndEditResultRange = ViewAndEditValue(pointLight.range, "Radius", 0.01f, 0.01f, FLT_MAX);
        viewAndEditResult.isEdited |= viewAndEditResultRange.isEdited;
        viewAndEditResult.isActive |= viewAndEditResultRange.isActive;
        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(DirectionalLight& directionalLight, const Blackboard& blackboard)
    {
        ViewAndEditResult viewAndEditResult;
        ViewAndEditResult viewAndEditResultDirection = ViewAndEditValue(directionalLight.direction, "Direction", blackboard);
        viewAndEditResult.isEdited |= viewAndEditResultDirection.isEdited;
        viewAndEditResult.isActive |= viewAndEditResultDirection.isActive;
        ViewAndEditResult viewAndEditResultColor = ViewAndEditValue(directionalLight.color, "Color");
        viewAndEditResult.isEdited |= viewAndEditResultColor.isEdited;
        viewAndEditResult.isActive |= viewAndEditResultColor.isActive;
        ViewAndEditResult viewAndEditResultIntensity = ViewAndEditValue(directionalLight.intensity, "Intensity", 0.01f, 0.0f, 1.f);
        viewAndEditResult.isEdited |= viewAndEditResultIntensity.isEdited;
        viewAndEditResult.isActive |= viewAndEditResultIntensity.isActive;
        return viewAndEditResult;
    }

    static ViewAndEditResult ViewAndEditCamera(Camera& camera, const Dimension2u resolution)
    {
        ViewAndEditResult viewAndEditResult;

        /*Degreesf horizontalFoV = ToDegrees(camera.GetHorizontalFOV());
        float nearPlane = camera.GetNearPlane();
        float farPlane = camera.GetFarPlane();

        if (ImGui::DragFloat("HorizontalFoV", &horizontalFoV.Value(), 0.1f, 0.f, 179.f))
        {
            camera.SetHorizontalFOV(ToRadians(horizontalFoV), resolution);
            viewAndEditResult.isEdited = true;
            viewAndEditResult.isActive = ImGui::IsItemActive();
        }

        if (ImGui::DragFloat("NearPlane", &nearPlane, 0.1f, 0.f, 10.f))
        {
            camera.SetNearPlane(nearPlane, resolution);
            viewAndEditResult.isEdited = true;
            viewAndEditResult.isActive = ImGui::IsItemActive();
        }

        if (ImGui::DragFloat("FarPlane", &farPlane, 0.1f, 0.5f, 30000.f))
        {
            camera.SetFarPlane(farPlane, resolution);
            viewAndEditResult.isEdited = true;
            viewAndEditResult.isActive = ImGui::IsItemActive();
        }*/

        const float aspectRatio = ToAspectRatio(resolution);

        Degreesf horizontalFoV = ToDegrees(camera.GetHorizontalFOV());
        float nearPlane = camera.GetNearPlane();
        float farPlane = camera.GetFarPlane();

        if (ImGui::DragFloat("HorizontalFoV", &horizontalFoV.Value(), 0.1f, 0.f, 179.f))
        {
            camera.SetHorizontalFOV(ToRadians(horizontalFoV));
            camera.SetAspectRatio(aspectRatio);
            viewAndEditResult.isEdited = true;
            viewAndEditResult.isActive = ImGui::IsItemActive();
        }

        if (ImGui::DragFloat("NearPlane", &nearPlane, 0.1f, 0.f, 10.f))
        {
            camera.SetNearPlane(nearPlane);
            camera.SetAspectRatio(aspectRatio);
            viewAndEditResult.isEdited = true;
            viewAndEditResult.isActive = ImGui::IsItemActive();
        }

        if (ImGui::DragFloat("FarPlane", &farPlane, 0.1f, 0.5f, 30000.f))
        {
            camera.SetFarPlane(farPlane);
            camera.SetAspectRatio(aspectRatio);
            viewAndEditResult.isEdited = true;
            viewAndEditResult.isActive = ImGui::IsItemActive();
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(Camera& camera, const Blackboard& blackboard)
    {
        return ViewAndEditCamera(camera, blackboard.Get<Key_WindowView>().GetClientSize());
    }

    template<typename T, typename... Args>
    void ShowVariantSelectable(auto& variant, const DataTypeRegistry& dataTypeRegistry)
    {
        DataTypeID dataTypeID = GetDataTypeID<T>();
        const bool isSelected = std::holds_alternative<T>(variant);
        if (ImGui::Selectable(dataTypeRegistry.Find(dataTypeID)->name.c_str(), isSelected))
        {
            variant = T{};
        }

        if constexpr (sizeof...(Args) > 0)
        {
            ShowVariantSelectable<Args...>(variant, dataTypeRegistry);
        }
    }

    template<typename... Args>
    void ShowVariantCombo(std::variant<Args...>& variant, const DataTypeRegistry& dataTypeRegistry)
    {
        std::pair<DataTypeID, void*> variantResult = GetVariantInfo(variant);

        const DataType* currentDataType = dataTypeRegistry.Find(variantResult.first);
        ASSERT(currentDataType != nullptr);

        if (ImGui::BeginCombo("Shape Type", currentDataType->name.c_str()))
        {
            ShowVariantSelectable<Args...>(variant, dataTypeRegistry);
            ImGui::EndCombo();
        }
    }

    ViewAndEditResult ViewAndEditValue(Shape& shape, const DataTypeRegistry& dataTypeRegistry, const Blackboard& blackboard)
    {
        ShowVariantCombo(shape, dataTypeRegistry);
        const std::pair<DataTypeID, void*> variantResult = GetVariantInfo(shape);
        dataTypeRegistry.ViewAndEditData(variantResult.first, variantResult.second, blackboard);

        return {};
    }

    ViewAndEditResult ViewAndEditValue(Shape& shape, const Blackboard& blackboard)
    {
        return ViewAndEditValue(shape, blackboard.Get<Key_DataTypeRegistry>(), blackboard);
    }

    ViewAndEditResult ViewAndEditValue(AABB3f& aabb, const DataTypeMemberVariable* memberData)
    {
        memberData;
        ViewAndEditResult viewAndEditResult;

        {
            Point3f center = aabb.GetCenter();
            ViewAndEditResult centerViewAndEditResult = ViewAndEditValue(center, "Center");
            viewAndEditResult.isEdited |= centerViewAndEditResult.isEdited;
            viewAndEditResult.isActive |= centerViewAndEditResult.isActive;
            if (centerViewAndEditResult.isEdited)
            {
                aabb.SetCenter(center);
            }
        }
        {
            Vector3f extent = aabb.GetExtent();
            ViewAndEditResult extentViewAndEditResult = ViewAndEditValue(extent, "Extent");
            viewAndEditResult.isEdited |= extentViewAndEditResult.isEdited;
            viewAndEditResult.isActive |= extentViewAndEditResult.isActive;
            if (extentViewAndEditResult.isEdited)
            {
                aabb.SetExtent(extent);
            }
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(Ray3f& ray, const Blackboard& blackboard)
    {
        ViewAndEditResult viewAndEditResult;

        {
            Point3f origin = ray.GetOrigin();
            ViewAndEditResult originViewAndEditResult = ViewAndEditValue(origin, "Origin");
            viewAndEditResult.isEdited |= originViewAndEditResult.isEdited;
            viewAndEditResult.isActive |= originViewAndEditResult.isActive;
            if (originViewAndEditResult.isEdited)
            {
                ray.SetOrigin(origin);
            }
        }
        {
            UnitVector3f direction = ray.GetDirection();
            Blackboard newBlackboard = blackboard;
            newBlackboard.Insert<Key_VariableName>("Direction");
            ViewAndEditResult directionViewAndEditResult = ViewAndEditValue(direction, newBlackboard, nullptr);
            viewAndEditResult.isEdited |= directionViewAndEditResult.isEdited;
            viewAndEditResult.isActive |= directionViewAndEditResult.isActive;
            if (directionViewAndEditResult.isEdited)
            {
                ray.SetDirection(direction);
            }
        }

        return viewAndEditResult;
    }

    //ViewAndEditResult ViewAndEditValue(Sprite2D& aSprite, const std::string& /*variableName*/)
    //{
    //	ViewAndEditResult viewAndEditResult;
    //	{
    //		Point2f position = aSprite.GetPosition();
    //		ViewAndEditResult positionViewAndEditResult = ViewAndEditValue(position, "Position");
    //		viewAndEditResult |= positionViewAndEditResult;
    //		if (positionViewAndEditResult.isEdited)
    //		{
    //			aSprite.SetPosition(position);
    //		}
    //	}
    //	{
    //		Vector2f scale = aSprite.GetScale();
    //		ViewAndEditResult scaleViewAndEditResult = ViewAndEditValue(scale, "Scale");
    //		viewAndEditResult |= scaleViewAndEditResult;
    //		if (scaleViewAndEditResult.isEdited)
    //		{
    //			aSprite.SetScale(scale);
    //		}
    //	}
    //	return viewAndEditResult;
    //}

    static ViewAndEditResult ViewAndEditValue(MeshAssetHandle& meshAsset, AssetManager& assetManager)
    {
        ViewAndEditResult viewAndEditResult;
        std::filesystem::path meshFilePath;

        if (meshAsset)
        {
            meshFilePath = meshAsset->GetName();
        }

        ImGui::AlignTextToFramePadding();

        ImGui::Text("Mesh:");
        ImGui::SameLine();
        ImGui::BeginDisabled();
        ImGui::InputText("", meshFilePath.string().data(), meshFilePath.string().size());
        ImGui::EndDisabled();

        if (const ImGuiPayload* currentPayload = ImGui::GetDragDropPayload())
        {
            const std::filesystem::path path = std::filesystem::path(reinterpret_cast<const char*>(currentPayload->Data));
            const std::filesystem::path extension = path.extension();

            if (extension.string() == ".fbx")
            {
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset"))
                    {
                        meshAsset = assetManager.GetMesh(path);
                        viewAndEditResult.isEdited = true;
                        viewAndEditResult.isActive = true;
                    }
                    ImGui::EndDragDropTarget();
                }
            }
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(MeshAssetHandle& mesh, const Blackboard& blackboard)
    {
        return ViewAndEditValue(mesh, blackboard.Get<Key_AssetManager>());
    }

    static ViewAndEditResult ViewAndEditValue(ModelAssetHandle& modelAsset, AssetManager& assetManager)
    {
        ViewAndEditResult viewAndEditResult;
        std::filesystem::path path;

        if (modelAsset)
        {
            path = modelAsset->GetPath();
        }

        ImGui::AlignTextToFramePadding();

        ImGui::Text("Model:");
        ImGui::SameLine();
        ImGui::BeginDisabled();
        ImGui::InputText("", path.string().data(), path.string().size());
        ImGui::EndDisabled();

        if (const ImGuiPayload* currentPayload = ImGui::GetDragDropPayload())
        {
            const std::filesystem::path droppedPath = std::filesystem::path(reinterpret_cast<const char*>(currentPayload->Data));
            const std::filesystem::path extension = droppedPath.extension();

            if (extension.string() == ".fbx")
            {
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset"))
                    {
                        modelAsset = assetManager.GetModel(droppedPath);
                        viewAndEditResult.isEdited = true;
                        viewAndEditResult.isActive = true;
                    }
                    ImGui::EndDragDropTarget();
                }
            }
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(ModelAssetHandle& model, const Blackboard& blackboard)
    {
        return ViewAndEditValue(model, blackboard.Get<Key_AssetManager>());
    }

    static ViewAndEditResult ViewAndEditValue(AnimatedModelAssetHandle& animatedModelAsset, AssetManager& assetManager)
    {
        ViewAndEditResult viewAndEditResult;
        std::filesystem::path path;

        if (animatedModelAsset)
        {
            path = animatedModelAsset->GetPath();
        }

        ImGui::AlignTextToFramePadding();

        ImGui::Text("Model:");
        ImGui::SameLine();
        ImGui::BeginDisabled();
        ImGui::InputText("", path.string().data(), path.string().size());
        ImGui::EndDisabled();

        if (const ImGuiPayload* currentPayload = ImGui::GetDragDropPayload())
        {
            const std::filesystem::path droppedPath = std::filesystem::path(reinterpret_cast<const char*>(currentPayload->Data));
            const std::filesystem::path extension = droppedPath.extension();

            if (extension.string() == ".fbx")
            {
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset"))
                    {
                        animatedModelAsset = assetManager.GetAnimatedModel(droppedPath);
                        viewAndEditResult.isEdited = true;
                        viewAndEditResult.isActive = true;
                    }
                    ImGui::EndDragDropTarget();
                }
            }
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(AnimatedModelAssetHandle& model, const Blackboard& blackboard)
    {
        return ViewAndEditValue(model, blackboard.Get<Key_AssetManager>());
    }

    static ViewAndEditResult ViewAndEditValue(TextureAssetHandle& textureAsset, AssetManager& assetManager)
    {
        ViewAndEditResult viewAndEditResult;

        std::string textureName = textureAsset ? textureAsset.GetRelativePath().stem().string() : "None";

        ImGui::AlignTextToFramePadding();

        ImGui::Text("Texture:");

        ImGui::SameLine();
        ImGui::BeginDisabled();
        ImGui::InputText("", textureName.data(), textureName.size());
        ImGui::EndDisabled();

        if (const ImGuiPayload* currentPayload = ImGui::GetDragDropPayload())
        {
            const std::filesystem::path path = std::filesystem::path(reinterpret_cast<const char*>(currentPayload->Data));
            const std::filesystem::path extension = path.extension();

            if (extension == ".dds")
            {
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset"))
                    {
                        textureAsset = assetManager.GetTexture(path);
                        viewAndEditResult.isEdited = true;
                        viewAndEditResult.isActive = true;
                    }
                    ImGui::EndDragDropTarget();
                }
            }
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(TextureAssetHandle& texture, const Blackboard& blackboard)
    {
        return ViewAndEditValue(texture, blackboard.Get<Key_AssetManager>());
    }

    [[nodiscard]] static bool IsPixelShader(const std::filesystem::path& path)
    {
        return path.string().find("PS") != std::string::npos;
    }

    [[nodiscard]] static bool IsVertexShader(const std::filesystem::path& path)
    {
        return path.string().find("VS") != std::string::npos;
    }

    static ViewAndEditResult ViewAndEditValue(PixelShaderAssetHandle& shader, AssetManager& assetManager)
    {
        ViewAndEditResult viewAndEditResult;

        std::filesystem::path currentPath;

        std::vector<std::filesystem::path> shaderFilePaths = FileUtility::GetPathsFromDirectory(std::filesystem::absolute(SIMPLE_DIR_SHADERS));
        auto pixelShaderFilePaths = shaderFilePaths | std::views::filter(IsPixelShader);

        if (shader)
        {
            currentPath = shader.GetRelativePath();
        }

        for (std::filesystem::path& shaderFilePath : shaderFilePaths)
        {
            shaderFilePath = std::filesystem::path("Shaders") / shaderFilePath;
        }


        ImGui::AlignTextToFramePadding();

        ImGui::Text("VertexShader:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);

        if (ImGui::BeginCombo("##PixelShaderECSEditorFunction", currentPath.string().c_str()))
        {
            for (const auto& path : pixelShaderFilePaths)
            {
                const bool isSelected = path == currentPath;

                if (ImGui::Selectable(path.string().c_str(), isSelected))
                {
                    currentPath = path;
                }
            }
        }

        if (viewAndEditResult.isEdited)
        {
            currentPath = std::filesystem::path("Shaders") / currentPath;

            PixelShaderAssetHandle foundShader = assetManager.GetPixelShader(currentPath);

            if (foundShader)
            {
                shader = foundShader;
                viewAndEditResult.isEdited = true;
                viewAndEditResult.isActive = true;
            }
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(PixelShaderAssetHandle& shaderAsset, const Blackboard& blackboard)
    {
        return ViewAndEditValue(shaderAsset, blackboard.Get<Key_AssetManager>());
    }

    static ViewAndEditResult ViewAndEditValue(VertexShaderAssetHandle& shaderAsset, AssetManager& assetManager)
    {
        ViewAndEditResult viewAndEditResult;

        std::filesystem::path currentPath;

        std::vector<std::filesystem::path> shaderFilePaths = FileUtility::GetPathsFromDirectory(std::filesystem::absolute(SIMPLE_DIR_SHADERS));
        auto vertexShaderFilePaths = shaderFilePaths | std::views::filter(IsVertexShader);

        if (shaderAsset)
        {
            currentPath = shaderAsset.GetRelativePath();
        }

        for (std::filesystem::path& shaderFilePath : shaderFilePaths)
        {
            shaderFilePath = std::filesystem::path("Shaders") / shaderFilePath;
        }


        ImGui::AlignTextToFramePadding();

        ImGui::Text("VertexShader:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);

        if (ImGui::BeginCombo("##VertexShaderECSEditorFunction", currentPath.string().c_str()))
        {
            for (const auto& vertexShaderPath : vertexShaderFilePaths)
            {
                const bool isSelected = vertexShaderPath == currentPath;

                if (ImGui::Selectable(vertexShaderPath.string().c_str(), isSelected))
                {
                    currentPath = vertexShaderPath;
                }
            }
        }

        if (viewAndEditResult.isEdited)
        {
            currentPath = std::filesystem::path("Shaders") / currentPath;

            VertexShaderAssetHandle foundShader = assetManager.GetVertexShader(currentPath);

            if (foundShader)
            {
                shaderAsset = foundShader;
                viewAndEditResult.isEdited = true;
                viewAndEditResult.isActive = true;
            }
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(VertexShaderAssetHandle& shaderAsset, const Blackboard& blackboard)
    {
        return ViewAndEditValue(shaderAsset, blackboard.Get<Key_AssetManager>());
    }

    static ViewAndEditResult ViewAndEditValue(AnimationAssetHandle& animationAsset, AssetManager& assetManager)
    {
        ViewAndEditResult viewAndEditResult;
        std::filesystem::path filePath;

        if (animationAsset)
        {
            filePath = animationAsset.GetRelativePath();
        }

        ImGui::AlignTextToFramePadding();

        ImGui::Text("Animation:");
        ImGui::SameLine();
        ImGui::BeginDisabled();
        ImGui::InputText("", filePath.string().data(), filePath.string().size());
        ImGui::EndDisabled();

        if (const ImGuiPayload* currentPayload = ImGui::GetDragDropPayload())
        {
            const std::filesystem::path path = std::filesystem::path(reinterpret_cast<const char*>(currentPayload->Data));
            const std::filesystem::path extension = path.extension();

            if (extension.string() == ".fbx")
            {
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset"))
                    {
                        animationAsset = assetManager.GetAnimation(path);
                        viewAndEditResult.isEdited = true;
                        viewAndEditResult.isActive = true;
                    }
                    ImGui::EndDragDropTarget();
                }
            }
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(AnimationAssetHandle& animationAsset, const Blackboard& blackboard)
    {
        return ViewAndEditValue(animationAsset, blackboard.Get<Key_AssetManager>());
    }

    static ViewAndEditResult ViewAndEditValue(SceneAssetHandle& sceneAsset, const std::string* variableName, AssetManager& assetManager)
    {
        ViewAndEditResult viewAndEditResult;
        const std::filesystem::path filePath = sceneAsset ? sceneAsset.GetRelativePath() : std::filesystem::path();

        ImGui::AlignTextToFramePadding();

        if (variableName)
        {
            ImGui::Text(variableName->c_str());
        }
        ImGui::Text("Scene:");
        ImGui::SameLine();
        ImGui::BeginDisabled();
        ImGui::InputText("", filePath.string().data(), filePath.string().size());
        ImGui::EndDisabled();

        if (const ImGuiPayload* currentPayload = ImGui::GetDragDropPayload())
        {
            const std::filesystem::path path = std::filesystem::path(reinterpret_cast<const char*>(currentPayload->Data));
            const std::filesystem::path extension = path.extension();

            if (std::wstring_view(extension.c_str()) == AssetExtensions::Scene)
            {
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset"))
                    {
                        sceneAsset = assetManager.GetScene(path);
                        viewAndEditResult.isEdited = true;
                        viewAndEditResult.isActive = true;
                    }
                    ImGui::EndDragDropTarget();
                }
            }
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(SceneAssetHandle& sceneAsset, const Blackboard& blackboard, const DataTypeMemberVariable* memberData)
    {
        return ViewAndEditValue(sceneAsset, memberData ? &memberData->customName : nullptr, blackboard.Get<Key_AssetManager>());
    }

    /*ViewAndEditResult ViewAndEditValue(Fly::ClassInstanceProxy& aClassInstance, [[maybe_unused]] const std::string&)
    {

        ViewAndEditResult viewAndEditResult;

        Fly::DataTypeProxy entityDataType(Fly::GetDataTypeID<EntityID>());
        auto entityClasses = Fly::GetClassesByTargetDataType(entityDataType);

        std::string_view preview = aClassInstance ? aClassInstance.GetName() : "None";
        Combo classCombo("Entity Class", std::string(preview).c_str(), [&]() -> void
            {

                for (auto& entityClass : entityClasses)
                {
                    const bool isSelected = aClassInstance ? aClassInstance.GetClassInstance().mClassID == entityClass.GetID() : false;
                    if (ImGui::Selectable(std::string(entityClass.GetName()).c_str(), isSelected))
                    {
                        if (isSelected)
                        {
                            continue;
                        }

                        if (aClassInstance)
                        {
                            aClassInstance.Destroy();
                        }

                        aClassInstance = entityClass.CreateClassInstance();

                        viewAndEditResult.isEdited = true;
                        viewAndEditResult.isActive = true;
                    }
                }
            });

        if (!aClassInstance)
        {
            return viewAndEditResult;
        }

        aClassInstance.ViewAndEditVariableDefaultValues(nullptr);

        return viewAndEditResult;
    }*/

    /*static bool IsCubeMap(ID3D11ShaderResourceView* shaderResourceView)
    {
        Microsoft::WRL::ComPtr<ID3D11Resource> resource;
        shaderResourceView->GetResource(&resource);

        if (resource)
        {
            Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
            if (SUCCEEDED(resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture2D)))
            {
                D3D11_TEXTURE2D_DESC desc;
                texture2D->GetDesc(&desc);

                return desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE;
            }
        }

        return false;
    }*/

    ViewAndEditResult ViewAndEditValue(std::array<TextureAssetHandle, 3>& textureAssets, AssetManager& assetManager)
    {
        ViewAndEditResult viewAndEditResult;

        ImGui::AlignTextToFramePadding();

        ImTextureID albedoTextureID = textureAssets[TextureSlots::Albedo] ? textureAssets[TextureSlots::Albedo]->GetShaderResourceView() : nullptr;
        ImTextureID normalTextureID = textureAssets[TextureSlots::Normal] ? textureAssets[TextureSlots::Normal]->GetShaderResourceView() : nullptr;
        ImTextureID materialTextureID = textureAssets[TextureSlots::Material] ? textureAssets[TextureSlots::Material]->GetShaderResourceView() : nullptr;

        ImGui::Dummy(ImVec2(0, 10));

        const ImVec2 windowSize = ImGui::GetWindowSize();

        constexpr float imageWidth = 64.0f;
        constexpr float imageHeight = 64.0f;
        const float totalSpacing = windowSize.x - (imageWidth * 3);
        const float spacing = totalSpacing / 4.0f;

        for (size_t i = 0; i < textureAssets.size(); ++i)
        {
            std::string textureName;

            if (textureAssets[i])
            {
                textureName = textureAssets[i].GetRelativePath().stem().string();
            }

            ImTextureID textureID = nullptr;
            const char* labelText = "";
            float xPosition = 0.0f;

            switch (i)
            {
            case TextureSlots::Albedo:
                textureID = albedoTextureID;
                labelText = "Albedo";
                xPosition = spacing;
                break;
            case TextureSlots::Normal:
                textureID = normalTextureID;
                labelText = "Normal";
                xPosition = spacing * 2 + imageWidth;
                break;
            case TextureSlots::Material:
                textureID = materialTextureID;
                labelText = "Material";
                xPosition = windowSize.x - imageWidth - spacing;
                break;
            }

            ImGui::SetCursorPos(ImVec2(xPosition, ImGui::GetCursorPosY()));
            const ImVec2 imagePos = ImGui::GetCursorScreenPos();

            ImGui::BeginGroup();
            ImGui::Image(textureID, ImVec2(imageWidth, imageHeight));

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            const ImVec2 imageMin = imagePos;
            const ImVec2 imageMax = ImVec2(imagePos.x + imageWidth, imagePos.y + imageHeight);
            drawList->AddRect(imageMin, imageMax, IM_COL32(0, 0, 0, 255), 0.0f, ImDrawFlags_None, 2.0f);

            if (const ImGuiPayload* currentPayload = ImGui::GetDragDropPayload())
            {
                const std::filesystem::path path = std::filesystem::path(reinterpret_cast<const char*>(currentPayload->Data));
                const std::filesystem::path extension = path.extension();

                if (extension.string() == ".dds")
                {
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset"))
                        {
                            TextureAssetHandle textureAsset = assetManager.GetTexture(path);

                            void* shaderResourceView = textureAsset->GetShaderResourceView();

                            switch (i)
                            {
                            case TextureSlots::Albedo:
                                albedoTextureID = shaderResourceView;
                                break;
                            case TextureSlots::Normal:
                                normalTextureID = shaderResourceView;
                                break;
                            case TextureSlots::Material:
                                materialTextureID = shaderResourceView;
                                break;
                            }



                            bool isCubeMap = false/*IsCubeMap()*/;

                            if (!isCubeMap)
                            {
                                textureAssets[i] = textureAsset;
                                viewAndEditResult.isEdited = true;
                                viewAndEditResult.isActive = true;
                            }
                        }

                        ImGui::EndDragDropTarget();
                    }

                }
            }

            ImGui::Text("%s", labelText);
            ImGui::EndGroup();

            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip(textureName.c_str());
            }

            if (i < textureAssets.size() - 1)
            {
                ImGui::SameLine();
            }
        }

        return viewAndEditResult;
    }

    ViewAndEditResult ViewAndEditValue(std::array<TextureAssetHandle, 3>& textureAssets, const Blackboard& blackboard)
    {
        return ViewAndEditValue(textureAssets, blackboard.Get<Key_AssetManager>());
    }
}