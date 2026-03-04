#pragma once
#include <memory>

#include "Engine/Graphics/RenderContext.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Graphics/GraphicsWindowView.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"
#include "Engine/Utility/GraphicsBufferData.hpp"

namespace CLX
{
    class RenderState;
    class AssetManager;
    class GraphicsSettings;
    
    class GraphicsFoundation final
    {
    public:

        template<typename T>
        explicit GraphicsFoundation(T&& object)
            : mConcept(std::make_unique<Model<T>>(std::move(object)))
        {

        }

        GraphicsFoundation(const GraphicsFoundation&) = delete;
        GraphicsFoundation& operator=(const GraphicsFoundation&) = delete;
        GraphicsFoundation(GraphicsFoundation&&) = default;
        GraphicsFoundation& operator=(GraphicsFoundation&&) = default;

        void BeginFrame(const GraphicsBufferData& bufferData);
        void EndFrame();
        void Init();
        void Shutdown();
        void Render(RenderState& renderState);

        [[nodiscard]] RenderContext CreateRenderContext(const Vector2ui& size);
        GraphicsWindowView MakeWindow(WindowView windowView);

        void SetAssetManager(std::shared_ptr<AssetManager> assetManager);
        void SetGraphicsSettings(std::shared_ptr<GraphicsSettings> graphicsSettings);

    private:

        class Concept
        {
        public:

            virtual ~Concept() = default;

            virtual void BeginFrame(const GraphicsBufferData& bufferData) = 0;
            virtual void EndFrame() = 0;
            virtual void Init() = 0;
            virtual void Shutdown() = 0;
            virtual void Render(RenderState& renderState) = 0;

            [[nodiscard]] virtual RenderContext CreateRenderContext(const Vector2ui& size) = 0;
            virtual GraphicsWindowView MakeWindow(WindowView windowView) = 0;

            virtual void SetAssetManager(std::shared_ptr<AssetManager> assetManager) = 0;
            virtual void SetGraphicsSettings(std::shared_ptr<GraphicsSettings> graphicsSettings) = 0;
        };

        template<typename T>
        class Model final : public Concept
        {
        public:

            template<typename T>
            explicit Model(T&& object)
                : mObject(std::move(object))
            {
            }

            void BeginFrame(const GraphicsBufferData& bufferData) override
            {
                mObject.BeginFrame(bufferData);
            }

            void EndFrame() override
            {
                mObject.EndFrame();
            }

            void Init() override
            {
                mObject.Init();
            }

            void Shutdown() override
            {
                mObject.Shutdown();
            }

            void Render(RenderState& renderState) override
            {
                mObject.Render(renderState);
            }

            [[nodiscard]] RenderContext CreateRenderContext(const Vector2ui& size) override
            {
                return mObject.CreateRenderContext(size);
            }

            GraphicsWindowView MakeWindow(WindowView windowView) override
            {
                return mObject.MakeWindow(windowView);
            }

            void SetAssetManager(std::shared_ptr<AssetManager> assetManager)
            {
                mObject.SetAssetManager(std::move(assetManager));
            }

            void SetGraphicsSettings(std::shared_ptr<GraphicsSettings> graphicsSettings)
            {
                mObject.SetGraphicsSettings(std::move(graphicsSettings));
            }

        private:

            T mObject;
        };

    private:

        std::unique_ptr<Concept> mConcept;
    };
}