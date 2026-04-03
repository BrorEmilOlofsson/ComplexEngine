#pragma once

namespace CLX
{

    class GenericAsset
    {
    public:

        GenericAsset() = default;

        template<typename T>
        GenericAsset(Asset<T> asset)
            : mConcept(std::make_unique<Model<T>>(std::move(asset)))
        {
            std::construct_at()
        }

        template<typename T>
        T* Get()
        {
            if (auto model = dynamic_cast<Model<T>*>(mConcept.get()))
            {
                return &model->mAsset;
            }
            return nullptr;
        }

        template<typename T>
        const T* Get() const
        {
            if (auto model = dynamic_cast<const Model<T>*>(mConcept.get()))
            {
                return &model->mAsset;
            }
            return nullptr;
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
            Model();



        private:

            Asset<T> mAsset;
        };

    private:

        std::array<std::byte, 64> mStorage;
    };


}