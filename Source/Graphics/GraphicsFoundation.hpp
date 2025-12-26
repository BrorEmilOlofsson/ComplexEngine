#pragma once
#include "Graphics/RenderContext.hpp"
#include "Utility/Math/Vector2.hpp"
#include <memory>


namespace Simple
{
    
    class GraphicsFoundation final
    {
    public:

        template<typename T>
        explicit GraphicsFoundation(T&& object)
            : mConcept(std::make_unique<Model<T>>(std::move(object)))
        {

        }



        [[nodiscard]] RenderContext CreateRenderContext(const Vector2ui& size);

    private:

        class Concept
        {
        public:

            virtual ~Concept() = default;

            [[nodiscard]] virtual RenderContext CreateRenderContext(const Vector2ui& size) = 0;
        };

        template<typename T>
        class Model final : public Concept
        {
        public:


            template<typename T>
            explicit Model(T&& object)
                : mObject(object)
            {
            }

            [[nodiscard]] RenderContext CreateRenderContext(const Vector2ui& size) override
            {
                return mObject.CreateRenderContext(size);
            }


        private:

            T mObject;
        };

    private:

        std::unique_ptr<Concept> mConcept;
    };
}