#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorWindowManager.hpp"

namespace CLX
{

    void EditorWindowManager::UpdateWindows(const Blackboard& blackboard)
    {
        for (const auto& window : mEntityCompositionWindows)
        {
            window->Update(blackboard);
        }
    }

    void EditorWindowManager::RenderWindows(const Blackboard& blackboard)
    {
        for (const auto& window : mEntityCompositionWindows)
        {
            window->Render(blackboard);
        }
    }

    void EditorWindowManager::CreateWindow(const EntityCompositionAssetHandle assetHandle, RenderContext&& renderContext)
    {
        ASSERT(assetHandle.IsValid());
        if (std::ranges::find_if(mEntityCompositionWindows, [&](const auto& window) { return window->GetCompositionAsset() == assetHandle; }) != mEntityCompositionWindows.end())
        {
            return;
        }
        mEntityCompositionWindows.push_back(std::make_unique<EntityCompositionPopUp>(assetHandle, std::move(renderContext)));
    }
}