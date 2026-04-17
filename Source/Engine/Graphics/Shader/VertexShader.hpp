#pragma once

namespace CLX
{

    class VertexShader final
    {
    public:

        template<typename T>
        VertexShader(T&& object)
            : mConcept(std::make_unique<VertexShaderModel<T>>(object))
        {}

        void Bind()
        {
            mConcept->Bind();
        }

        template<typename T>
        [[nodiscard]] T& GetUnsafe()
        {
            return static_cast<VertexShaderModel<T>*>(mConcept.get())->Get();
        }

        template<typename T>
        [[nodiscard]] const T& GetUnsafe() const
        {
            return static_cast<const VertexShaderModel<T>*>(mConcept.get())->Get();
        }

    private:

        class VertexShaderConcept
        {
        public:

            virtual ~VertexShaderConcept() = default;

            virtual void Bind() = 0;
        };

        template<typename T>
        class VertexShaderModel final : public VertexShaderConcept
        {
        public:

            template<typename T>
            VertexShaderModel(T&& object)
                : mObject(std::move(object))
            {}

            void Bind()
            {
                mObject.Bind();
            }

            T& Get()
            {
                return mObject;
            }

            const T& Get() const
            {
                return mObject;
            }

        private:

            T mObject;
        };

    private:


        std::unique_ptr<VertexShaderConcept> mConcept;
    };
}