#pragma once
#include <vector>
#include <memory>
#include "Editor/PopUps/Editor/EntityCompositionPopUp.hpp"

namespace CLX
{
    
    class EditorWindowManager final
    {
    public:

        EditorWindowManager() = default;

        void UpdateWindows(const Blackboard& blackboard);
        void RenderWindows(const Blackboard& blackboard);

        void CreateWindow(EntityCompositionAssetHandle assetHandle, RenderContext&& renderContext);


    private:

        std::vector<std::unique_ptr<EntityCompositionPopUp>> mEntityCompositionWindows;
    };
}