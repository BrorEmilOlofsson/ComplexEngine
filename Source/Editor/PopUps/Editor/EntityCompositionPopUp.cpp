#include "Editor/Precompiled/EditorPch.hpp"
#include "EntityCompositionPopUp.hpp"
#include "Editor/Functions/ECSEditorFunctions.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Editor/Functions/EditorFunctions.hpp"
#include "Editor/EditorSceneSettings.hpp"
#include "Engine/Utility/DebugShapes.hpp"
#include "Engine/OperatingSystem/OperatingSystem.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformUtility.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformHierarchyUtility.hpp"
#include "Engine/ECS/ECSSerializer.hpp"
#include "Engine/Asset/AssetManager.hpp"

namespace CLX
{

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
            std::shared_ptr<EntityComposition> ec = std::make_shared<EntityComposition>(blackboard.Get<Key_ECSRegistry>());
            const std::filesystem::path defaultPath = std::filesystem::absolute(SIMPLE_DIR_ASSETS) / "EntityCompositions" / "Test.json";
            blackboard.Get<Key_AssetManager>().AddEntityComposition(defaultPath, EntityCompositionAsset(ec));

            mEntityCompositionAsset = blackboard.Get<Key_AssetManager>().GetEntityComposition(defaultPath);
            mRootEntities = GetRootEntities(mEntityCompositionAsset->GetECS());
            mEntityCompositionAsset->GetECS().GetComponent<NameComponent>(mEntityCompositionAsset->GetRootEntity())->name = "Root";
        }

        Blackboard newBlackboard = blackboard;
        newBlackboard.Insert<Key_CurrentCamera>(mCamera);
        newBlackboard.Insert<Key_CurrentRenderState>(mRenderState);
        const InputState& input = blackboard.Get<Key_InputState>();
        EditorCommandTracker& commandTracker = blackboard.Get<Key_CommandTracker>();
        OperatingSystem& os = blackboard.Get<Key_OperatingSystem>();
        mEntityCompositionAsset->GetECS().EditorUpdate(newBlackboard);


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
                    .gridSize = Vector3ui(50, 0, 50),
                    .cellSize = Vector3f(10, 0, 10),
                    .offset = Vector3f(250.f, 0.f, 250.f),
                };
                RenderGrid3(grid, Colors::Gray, mRenderState.GetRenderList());
            }

            mRenderState.SetRenderRect(renderRect);
            mCamera.SetResolution(Vector2ui(renderRect.GetExtent()));


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
                        SelectEntity(entityID, mSelectedEntityID, commandTracker);
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
        EditorSceneSettings& editorSceneSettings = blackboard.Get<Key_EditorSceneSettings>();
        const InputState& input = blackboard.Get<Key_InputState>();
        input;
        Blackboard newBlackboard = blackboard;
        newBlackboard.Insert<Key_CurrentCamera>(mCamera);
        newBlackboard.Insert<Key_CurrentRenderState>(mRenderState);

        if (mIsOpen)
        {
            mEntityCompositionAsset->GetECS().Render(newBlackboard);
            blackboard.Get<Key_OperatingSystem>().GetGraphicsFoundation().Render(mRenderState);
        }

        if (ImGui::Begin("Entity Composition Hierarchy"))
        {
            if (ImGui::Button("Save"))
            {
                mEntityCompositionAsset->SetPath(std::filesystem::path(SIMPLE_DIR_ASSETS) / "EntityCompositions/Test.json");
                SaveEntityCompositionAsset(mEntityCompositionAsset, blackboard.Get<Key_DataTypeRegistry>());
            }
            ShowEntityHierarchyWithAddButtons(
                mEntityCompositionAsset->GetECS(),
                ecsBuffer,
                mRootEntities,
                commandTracker,
                "##EntityComposition",
                mSelectedEntityID,
                mEntityCompositionAsset->GetRootEntity(),
                { mEntityCompositionAsset->GetRootEntity() }
            );
        }

        ImGui::End();

        if (ImGui::Begin("Entity Composition Inspector"))
        {
            if (mSelectedEntityID != InvalidEntityID)
            {
                const Transform transform = GetWorldTransform(mEntityCompositionAsset->GetECS(), mSelectedEntityID);
                newBlackboard.Insert<Key_ReferenceTransform>(transform);
            }

            ShowEntityName(mEntityCompositionAsset->GetECS(), mSelectedEntityID, input);

            ShowEntityInspector(
                mEntityCompositionAsset->GetECS(),
                mSelectedEntityID,
                mAnyItemActiveLastFrame,
                ecsBuffer,
                mCopyEntityID,
                newBlackboard,
                commandTracker
            );
        }
        ImGui::End();

        if (ImGui::Begin(PopUpName))
        {
            const AABB2i renderRect = RenderImage(mRenderState.GetRenderContext()->GetOutputSRV());

            //mRenderState.SetRenderRect(renderRect);*/

            mTransformEntityTool.ShowEntityImGuizmo(
                mEntityCompositionAsset->GetECS(),
                mSelectedEntityID,
                editorSceneSettings.transformMode,
                renderRect,
                editorSceneSettings.useSnap,
                editorSceneSettings.snapValue,
                mCamera,
                input,
                os.IsCursorVisible(),
                commandTracker
            );

            ShowSceneSettingsPopUp(editorSceneSettings);
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
        mSelectedEntityID = mRootEntities.front();
    }

}