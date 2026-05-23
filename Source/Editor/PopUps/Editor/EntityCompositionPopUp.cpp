#include "Editor/Precompiled/EditorPch.hpp"
#include "EntityCompositionPopUp.hpp"
#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Editor/Functions/EditorFunctions.hpp"
#include "Editor/EditorSceneSettings.hpp"
#include "Engine/Utility/DebugShapes.hpp"
#include "Engine/OperatingSystem/OperatingSystem.hpp"
#include "Engine/ECSEngine/Utility/ECSUtilityFunctions.hpp"
#include "Engine/ECS/ECSSerializer.hpp"
#include "Engine/Asset/AssetManager.hpp"

namespace CLX
{

    std::string GetEntityCompositionName(EntityCompositionAssetHandle handle)
    {
        ASSERT(handle.IsValid());
        return handle.GetRelativePath().stem().string();
    }

    static void ShowInspector(std::set<EntityID>& selectedEntityIDs, EditorCommandTracker& commandTracker,
        EntityCompositionAssetHandle entityCompositionAsset, EntityCompositionInstantiationManager& entityInstantiations,
        ComponentBufferData& componentBufferData, EntityID& copyEntityID, uint32_t& selectedComponentPopupIndex,
        std::string& componentSearchBuffer, JsonAny& copiedComponent, const InputState& input, Blackboard& newBlackboard)
    {
        if (ImGui::Begin((GetEntityCompositionName(entityCompositionAsset) + " Inspector").c_str()))
        {
            if (!entityCompositionAsset.IsValid())
            {
                ImGui::End();
                return;
            }

            const EntityID selectedEntityID = selectedEntityIDs.size() == 1 ? *selectedEntityIDs.begin() : InvalidEntityID;
            if (selectedEntityID != InvalidEntityID)
            {
                const Transform transform = GetEntityWorldTransform(entityCompositionAsset->GetECS(), selectedEntityID);
                newBlackboard.Insert<Key_ReferenceTransform>(transform);
            }

            const bool canChangeName = entityCompositionAsset ? entityCompositionAsset->GetRootEntity() != selectedEntityID : false;
            auto entityNameAction = ShowEntityName(entityCompositionAsset->GetECS(), selectedEntityID, input, canChangeName);

            auto editorActions = ShowEntityInspector(ShowEntityInspectorData
                {
                    entityCompositionAsset->GetECS(),
                    selectedEntityID,
                    componentBufferData,
                    copyEntityID,
                    selectedComponentPopupIndex,
                    componentSearchBuffer,
                    copiedComponent,
                    entityCompositionAsset,
                    entityInstantiations,
                    newBlackboard
                }
            );

            if (entityNameAction)
            {
                entityNameAction.value()(commandTracker);
            }
            for (auto& action : editorActions)
            {
                action(commandTracker);
            }
        }
        ImGui::End();
    }

    EntityCompositionPopUp::EntityCompositionPopUp(EntityCompositionAssetHandle assetHandle, RenderContext&& renderContext)
        : mRenderState(std::move(renderContext))
    {
        SetCompositionAsset(assetHandle);
        sGuizmoIDCounter++;
        mGuizmoID = sGuizmoIDCounter;
    }

    EntityCompositionPopUp::EntityCompositionPopUp(RenderContext&& renderContext)
        : mRenderState(std::move(renderContext))
    {
        sGuizmoIDCounter++;
        mGuizmoID = sGuizmoIDCounter;
    }

    void EntityCompositionPopUp::Update(const Blackboard& blackboard)
    {

        mIsOpen = ImGui::Begin(GetEntityCompositionName(mEntityCompositionAsset).c_str());
        ImGuiUtility::CheckForWindowFocus();
        mIsWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
        const AABB2i renderRect = GetImGuiRenderRect();
        ImGui::End();


        if (!mEntityCompositionAsset.IsValid())
        {
            /*std::shared_ptr<EntityComposition> ec = std::make_shared<EntityComposition>(blackboard.Get<Key_ECSRegistry>());
            const std::filesystem::path defaultPath = GetAbsoluteAssetPath(std::filesystem::path("EntityCompositions") / "Test.clxec");
            blackboard.Get<Key_AssetManager>().AddEntityComposition(EntityCompositionAsset(ec, defaultPath));

            mEntityCompositionAsset = blackboard.Get<Key_AssetManager>().GetEntityComposition(defaultPath);
            mRootEntities = GetRootEntities(mEntityCompositionAsset->GetECS());
            mEntityCompositionAsset->GetECS().GetComponent<NameComponent>(mEntityCompositionAsset->GetRootEntity())->name = "Root";*/
        }

        const bool hasValidEntityComposition = mEntityCompositionAsset.IsValid();

        Blackboard newBlackboard = blackboard;
        newBlackboard.Insert<Key_CurrentCamera>(mCamera);
        newBlackboard.Insert<Key_CurrentRenderState>(mRenderState);
        const InputState& input = blackboard.Get<Key_InputState>();
        EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
        OperatingSystem& os = blackboard.Get<Key_OperatingSystem>();

        if (hasValidEntityComposition)
        {
            mEntityCompositionAsset->GetECS().EditorUpdate(newBlackboard);
        }

        if (mIsOpen)
        {
            if (mIsWindowFocused)
            {
                FreeFlyCameraSettings& cameraSettings = blackboard.Get<Key_FreeFlyCameraSettings>();
                const float deltaTime = blackboard.Get<Key_DeltaTime>();
                WindowView window = blackboard.Get<Key_WindowView>();
                UpdateEditorCamera(mCamera, cameraSettings, deltaTime, window, input, os);
            }

            constexpr Spheref drawSphere = Spheref::FromCenterAndRadius(Point3f(0, 0, 10), Radiusf(5.f));
            mRenderState.GetRenderList().AddSphere(DrawSphere{ drawSphere, Colors::Salmon }, true);

            if (blackboard.Get<Key_EditorSceneSettings>().showGrid)
            {
                const PrimitiveGrid3 grid
                {
                    .minPos = Point3f::Zero(),
                    .gridSize = Dimension3u(50, 0, 50),
                    .cellSize = Dimension3f(10, 0, 10),
                    .offset = Vector3f(250.f, 0.f, 250.f),
                };
                RenderGrid3(grid, Colors::Gray, mRenderState.GetRenderList());
            }

            mRenderState.SetRenderRect(renderRect);
            if (renderRect.GetExtent().y == 0.f)
            {

            }
            else
            {

                mCamera.SetResolution(GetDimension(renderRect));
            }


            if (input.IsKeyReleased(eInputKey::LMB))
            {
                const Point2i mouseScreenPosition = os.GetCursorScreenPosition();

                if (IsInsideRenderRect(mouseScreenPosition, renderRect))
                {
                    const Point2i mappedPos = MapToRenderRect(mouseScreenPosition, renderRect);

                    const uint32_t id = mRenderState.GetRenderContext()->GetObjectIDAt(mappedPos);

                    const EntityID entityID{ id };
                    if (entityID != InvalidEntityID)
                    {
                        SetEntitySelection(entityID, mSelectedEntityIDs, commandTracker);
                    }
                }
            }

            mRenderState.SetCamera(mCamera);
        }
    }

    void EntityCompositionPopUp::Render(const Blackboard& blackboard)
    {
        EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
        const WindowView windowView = blackboard.Get<Key_WindowView>();
        //OperatingSystem& os = blackboard.Get<Key_OperatingSystem>();
        const DataTypeRegistry& dataTypeRegistry = blackboard.Get<Key_DataTypeRegistry>();
        EditorSceneSettings& editorSceneSettings = blackboard.Get<Key_EditorSceneSettings>();
        const InputState& input = blackboard.Get<Key_InputState>();
        AssetManager& assetManager = blackboard.Get<Key_AssetManager>();
        EntityCompositionInstantiationManager& compositionInstantiations = blackboard.Get<Key_EntityCompositionInstantiationManager>();
        JsonAny& copiedComponent = blackboard.Get<Key_CurrentCopiedComponent>();

        const bool hasValidEntityComposition = mEntityCompositionAsset.IsValid();

        Blackboard newBlackboard = blackboard;
        newBlackboard.Insert<Key_CurrentCamera>(mCamera);
        newBlackboard.Insert<Key_CurrentRenderState>(mRenderState);

        if (hasValidEntityComposition)
        {
            newBlackboard.Insert<Key_CurrentECS>(mEntityCompositionAsset->GetECS());
        }


        if (mIsOpen)
        {
            if (hasValidEntityComposition)
            {
                mEntityCompositionAsset->GetECS().Render(newBlackboard);
                blackboard.Get<Key_OperatingSystem>().GetGraphicsFoundation().Render(mRenderState);
            }
        }



        if (ImGui::Begin((GetEntityCompositionName(mEntityCompositionAsset) + " Hierarchy").c_str()))
        {

            if (hasValidEntityComposition)
            {
                if (ImGui::Button("Save"))
                {
                    SaveEntityCompositionAsset(mEntityCompositionAsset, blackboard.Get<Key_DataTypeRegistry>());
                }
                ShowEntityHierarchyWithAddButtons(
                    mEntityCompositionAsset->GetECSHandle(),
                    mRootEntities,
                    commandTracker,
                    std::string("##") + GetEntityCompositionName(mEntityCompositionAsset) + "EntityComposition",
                    mSelectedEntityIDs,
                    mEntityCompositionAsset->GetRootEntity(),
                    { mEntityCompositionAsset->GetRootEntity() },
                    mEntitySearchBuffer,
                    mEntityCompositionAsset,
                    compositionInstantiations,
                    dataTypeRegistry,
                    assetManager
                );
            }
        }

        ImGui::End();

        ShowInspector(
            mSelectedEntityIDs,
            commandTracker,
            mEntityCompositionAsset,
            compositionInstantiations,
            mComponentBufferData,
            mCopyEntityID,
            mSelectedComponentPopupIndex,
            mComponentSearchBuffer,
            copiedComponent,
            input,
            newBlackboard
        );

        if (ImGui::Begin(GetEntityCompositionName(mEntityCompositionAsset).c_str()))
        {
            if (hasValidEntityComposition)
            {

                const AABB2i renderRect = RenderImage(mRenderState.GetRenderContext()->GetOutputSRV());

                //mRenderState.SetRenderRect(renderRect);*/

                if (mSelectedEntityIDs.size() == 1)
                {
                    const EntityID selectedEntityID = *mSelectedEntityIDs.begin();
                    ShowEntityImGuizmo(
                        mEntityCompositionAsset->GetECS(),
                        selectedEntityID,
                        editorSceneSettings.transformMode,
                        editorSceneSettings.transformOperation,
                        mCamera,
                        renderRect,
                        editorSceneSettings.useSnap,
                        editorSceneSettings.snapValue,
                        mGuizmoID,
                        mIsWindowFocused,
                        mTransformEntityData,
                        commandTracker
                    );
                }

                ShowSceneSettingsPopUp(editorSceneSettings);
            }
        }

        ImGui::End();
        mRenderState.Reset();
    }


    void EntityCompositionPopUp::SetCompositionAsset(EntityCompositionAssetHandle asset)
    {
        if (!asset)
        {
            return;
        }
        if (asset == mEntityCompositionAsset)
        {
            return;
        }
        mEntityCompositionAsset = asset;
        mRootEntities = GetRootEntities(mEntityCompositionAsset->GetECS());
        mSelectedEntityIDs = { mRootEntities.front() };
    }

}