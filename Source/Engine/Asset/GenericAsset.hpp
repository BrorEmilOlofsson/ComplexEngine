#pragma once
#include <memory>
#include "Engine/Utility/IDWrapper.hpp"

namespace CLX
{

    using GenericAssetID = IDWrapper<std::size_t, struct GenericAssetTag>;

    class GenericAsset
    {
    public:

        GenericAsset() = default;

        template<typename T>
        GenericAsset(T asset)
            : mConcept(std::make_unique<Model<T>>(std::move(asset)))
        {
        }

        template<typename T>
        [[nodiscard]] T* Cast()
        {
            if (auto model = dynamic_cast<Model<T>*>(mConcept.get()))
            {
                return &model->Get();
            }
            return nullptr;
        }

        template<typename T>
        [[nodiscard]] const T* Cast() const
        {
            if (auto model = dynamic_cast<const Model<T>*>(mConcept.get()))
            {
                return &model->Get();
            }
            return nullptr;
        }

        template<typename T>
        [[nodiscard]] T& UnsafeCast()
        {
            return static_cast<Model<T>*>(mConcept.get())->Get();
        }

        template<typename T>
        [[nodiscard]] const T& UnsafeCast() const
        {
            return static_cast<const Model<T>*>(mConcept.get())->Get();
        }



    private:

        class Concept
        {
        public:

            virtual ~Concept() = default;
        };

        template<typename T>
        class Model : public Concept
        {
        public:
            Model(T&& object)
                : mObject(std::move(object))
            {
            }

            [[nodiscard]] T& Get()
            {
                return mObject;
            }

            [[nodiscard]] const T& Get() const
            {
                return mObject;
            }

        private:

            T mObject;
        };

    private:

        std::unique_ptr<Concept> mConcept;
    };


}