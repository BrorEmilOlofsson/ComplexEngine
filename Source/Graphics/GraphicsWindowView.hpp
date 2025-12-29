#pragma once
#include <memory>

namespace Simple
{

    class RenderContext;

    class GraphicsWindowView final
    {
    public:

        template<typename T> requires (not std::same_as<T, GraphicsWindowView>)
        explicit GraphicsWindowView(T& window)
            : mWindow(std::addressof(window))
            , mInitFunc([](void* window) { static_cast<T*>(window)->Init(); })
            , mBeginFrameFunc([](void* window) { static_cast<T*>(window)->BeginFrame(); })
            , mEndFrameFunc([](void* window, RenderContext* renderContext) { static_cast<T*>(window)->EndFrame(renderContext); })
            , mOnResizeFunc([](void* window) { static_cast<T*>(window)->OnResize(); })
            , mBindBackBufferFunc([](void* window) { static_cast<T*>(window)->BindBackBuffer(); })
        {
        }

        void Init()
        {
            mInitFunc(mWindow);
        }

        void BeginFrame()
        {
            mBeginFrameFunc(mWindow);
        }

        void EndFrame(RenderContext* renderContext)
        {
            mEndFrameFunc(mWindow, renderContext);
        }

        void OnResize()
        {
            mOnResizeFunc(mWindow);
        }

        void BindBackBuffer()
        {
            mBindBackBufferFunc(mWindow);
        }

    private:

        void* mWindow = nullptr;
        void (*mInitFunc)(void*) = nullptr;
        void (*mBeginFrameFunc)(void*) = nullptr;
        void (*mEndFrameFunc)(void*, RenderContext*) = nullptr;
        void (*mOnResizeFunc)(void*) = nullptr;
        void (*mBindBackBufferFunc)(void*) = nullptr;
    };
}