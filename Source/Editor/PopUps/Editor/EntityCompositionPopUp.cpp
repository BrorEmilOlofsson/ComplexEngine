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

    static void ShowInspector(std::set<EntityID>& selectedEntityIDs, ECS& ecsBuffer, EditorCommandTracker& commandTracker,
        EntityCompositionAssetHandle entityCompositionAsset, bool& anyItemActiveLastFrame, EntityID& copyEntityID, uint32_t& selectedComponentPopupIndex,
        std::string& componentSearchBuffer, JsonAny& copiedComponent, const InputState& input, Blackboard& newBlackboard)
    {
        if (ImGui::Begin("Entity Composition Inspector"))
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

            auto entityNameAction = ShowEntityName(entityCompositionAsset->GetECS(), selectedEntityID, input);

            auto editorActions = ShowEntityInspector(
                entityCompositionAsset->GetECS(),
                selectedEntityID,
                anyItemActiveLastFrame,
                ecsBuffer,
                copyEntityID,
                selectedComponentPopupIndex,
                componentSearchBuffer,
                copiedComponent,
                entityCompositionAsset,
                newBlackboard
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

    EntityCompositionPopUp::EntityCompositionPopUp(RenderContext&& renderContext)
    {
        mRenderState.SetRenderContext(std::move(renderContext));
    }

    void EntityCompositionPopUp::Update(const Blackboard& blackboard)
    {

        mIsOpen = ImGui::Begin(PopUpName);
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
            FreeFlyCameraSettings& cameraSettings = blackboard.Get<Key_FreeFlyCameraSettings>();
            const float deltaTime = blackboard.Get<Key_DeltaTime>();
            WindowView window = blackboard.Get<Key_WindowView>();
            UpdateEditorCamera(mCamera, cameraSettings, deltaTime, window, input, os);


            constexpr Spheref drawSphere = Spheref::FromCenterAndRadius(Point3f(0, 0, 10), Radiusf(5.f));
            mRenderState.GetRenderList().AddSphere(DrawSphere{ drawSphere, Colors::Salmon }, true);

            if (blackboard.Get<Key_EditorSceneSettings>().showGrid)
            {
                const PrimitiveGrid3 grid
                {
                    .minPos = Point3f::Zero(),
                    .gridSize = Vector3u(50, 0, 50),
                    .cellSize = Vector3f(10, 0, 10),
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


            if (input.IsKeyPressed(eInputKey::LMB))
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
        ECS& ecsBuffer = blackboard.Get<Key_ECSBuffer>();
        EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
        const WindowView windowView = blackboard.Get<Key_WindowView>();
        OperatingSystem& os = blackboard.Get<Key_OperatingSystem>();
        const DataTypeRegistry& dataTypeRegistry = blackboard.Get<Key_DataTypeRegistry>();
        EditorSceneSettings& editorSceneSettings = blackboard.Get<Key_EditorSceneSettings>();
        const InputState& input = blackboard.Get<Key_InputState>();

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

        if (ImGui::Begin("Entity Composition Hierarchy"))
        {

            if (hasValidEntityComposition)
            {
                if (ImGui::Button("Save"))
                {
                    SaveEntityCompositionAsset(mEntityCompositionAsset, blackboard.Get<Key_DataTypeRegistry>());
                }
                ShowEntityHierarchyWithAddButtons(
                    mEntityCompositionAsset->GetECS(),
                    ecsBuffer,
                    mRootEntities,
                    commandTracker,
                    "##EntityComposition",
                    mSelectedEntityIDs,
                    mEntityCompositionAsset->GetRootEntity(),
                    { mEntityCompositionAsset->GetRootEntity() },
                    mEntitySearchBuffer,
                    dataTypeRegistry
                );
            }
        }

        ImGui::End();

        ShowInspector(
            mSelectedEntityIDs,
            ecsBuffer,
            commandTracker,
            mEntityCompositionAsset,
            mAnyItemActiveLastFrame,
            mCopyEntityID,
            mSelectedComponentPopupIndex,
            mComponentSearchBuffer,
            mCopiedComponent,
            input,
            newBlackboard
        );

        if (ImGui::Begin(PopUpName))
        {
            if (hasValidEntityComposition)
            {

                const AABB2i renderRect = RenderImage(mRenderState.GetRenderContext()->GetOutputSRV());

                //mRenderState.SetRenderRect(renderRect);*/

                if (mSelectedEntityIDs.size() == 1)
                {
                    const EntityID selectedEntityID = *mSelectedEntityIDs.begin();
                    mTransformEntityTool.ShowEntityImGuizmo(
                        mEntityCompositionAsset->GetECS(),
                        selectedEntityID,
                        editorSceneSettings.transformMode,
                        renderRect,
                        editorSceneSettings.useSnap,
                        editorSceneSettings.snapValue,
                        mCamera,
                        input,
                        os.IsCursorVisible(),
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