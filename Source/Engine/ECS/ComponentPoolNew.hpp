#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <array>
#include <cstddef>
#include <typeindex>
#include <span>
#include <typeinfo>
#include <type_traits>
#include "EntityID.hpp"
#include "Engine/Utility/ByteOffset.hpp"
#include "Engine/Utility/Assert.hpp"

namespace CLX
{

    class ComponentPoolNew final
    {
    public:

        static constexpr uint32_t InvalidComponentIndex = std::numeric_limits<uint32_t>::max();

        template<typename T>
        constexpr explicit ComponentPoolNew(std::type_identity<T>)
            : mType(std::type_index(typeid(T)))
        {
            using M = Model<T>;
            static_assert(sizeof(M) == BufferSize);
            static_assert(alignof(M) <= BufferAlignment);
            static_assert(std::is_nothrow_move_constructible_v<M>, "Model<T> must be nothrow move constructible");
            static_assert(std::is_nothrow_move_assignable_v<M>, "Model<T> must be nothrow move assignable");
            std::construct_at(reinterpret_cast<M*>(GetConcept()));
        }

        template<typename T>
        [[nodiscard]] static ComponentPoolNew Create()
        {
            return ComponentPoolNew(std::type_identity<T>{});
        }

        ~ComponentPoolNew()
        {
            GetConcept()->~Concept();
        }

        ComponentPoolNew(const ComponentPoolNew& other)
            : mType(other.mType)
        {
            other.GetConcept()->Clone(GetConcept());
        }

        ComponentPoolNew& operator=(const ComponentPoolNew& other)
        {
            ComponentPoolNew temp(other);
            mBuffer.swap(temp.mBuffer);
            mType = other.mType;
            return *this;
        }

        ComponentPoolNew(ComponentPoolNew&& other) noexcept
            : mType(std::exchange(other.mType, typeid(void)))
        {
            other.GetConcept()->Move(GetConcept());
        }

        ComponentPoolNew& operator=(ComponentPoolNew&& other) noexcept
        {
            mType = std::exchange(other.mType, typeid(void));
            GetConcept()->~Concept(); // destroy current
            ComponentPoolNew temp(std::move(other));
            temp.GetConcept()->Move(GetConcept());

            return *this;
        }

        template<typename T>
        T& AddComponent(const EntityID entityID)
        {
            ASSERT(mType == std::type_index(typeid(T)), "ComponentPoolNew::GetComponent: Type mismatch");
            void* componentPtr = GetConcept()->AddComponent(entityID);
            return *reinterpret_cast<T*>(componentPtr);
        }

        template<typename T>
        T& AddComponent(const EntityID entityID, const T& defaultValuePtr)
        {
            ASSERT(mType == std::type_index(typeid(T)), "ComponentPoolNew::GetComponent: Type mismatch");
            void* componentPtr = GetConcept()->AddComponent(entityID, &defaultValuePtr);
            return *reinterpret_cast<T*>(componentPtr);
        }

        template<typename T>
        T& AddComponent(const EntityID entityID, T&& defaultValuePtr)
        {
            ASSERT(mType == std::type_index(typeid(T)), "ComponentPoolNew::GetComponent: Type mismatch");
            void* componentPtr = GetConcept()->AddComponentMoved(entityID, &defaultValuePtr);
            return *reinterpret_cast<T*>(componentPtr);
        }

        template<typename T, typename... Args> requires std::is_default_constructible_v<T>
        T& AddComponent(const EntityID entityID, Args&&... args)
        {
            ASSERT(mType == std::type_index(typeid(T)), "ComponentPoolNew::GetComponent: Type mismatch");
            return AddComponent(entityID, T{ std::forward<Args>(args)... });
        }

        void* AddComponentMoved(const EntityID entityID, void* defaultValuePtr)
        {
            return GetConcept()->AddComponentMoved(entityID, defaultValuePtr);
        }

        template<typename T>
        [[nodiscard]] T* GetComponent(const EntityID entityID)
        {
            ASSERT(mType == std::type_index(typeid(T)), "ComponentPoolNew::GetComponent: Type mismatch");
            void* componentPtr = GetConcept()->GetComponent(entityID);
            return reinterpret_cast<T*>(componentPtr);
        }

        template<typename T>
        [[nodiscard]] const T* GetComponent(const EntityID entityID) const
        {
            ASSERT(mType == std::type_index(typeid(T)), "ComponentPoolNew::GetComponent: Type mismatch");
            const void* componentPtr = GetConcept()->GetComponent(entityID);
            return reinterpret_cast<const T*>(componentPtr);
        }

        [[nodiscard]] void* GetComponent(const EntityID entityID)
        {
            return GetConcept()->GetComponent(entityID);
        }

        [[nodiscard]] const void* GetComponent(const EntityID entityID) const
        {
            return GetConcept()->GetComponent(entityID);
        }

        [[nodiscard]] bool HasComponent(const EntityID entityID) const
        {
            return GetComponent(entityID) != nullptr;
        }

        void RemoveComponent(const EntityID entityID)
        {
            GetConcept()->RemoveComponent(entityID);
        }

        [[nodiscard]] std::size_t GetComponentCount() const
        {
            return GetConcept()->GetComponentCount();
        }

        void ReserveComponents(size_t size)
        {
            GetConcept()->ReserveComponents(size);
        }

        template<typename T>
        [[nodiscard]] const std::vector<T>& GetComponents() const
        {
            ASSERT(mType == std::type_index(typeid(T)), "ComponentPoolNew::GetComponents: Type mismatch");
            static_assert(std::same_as<ComponentRangeT<T>, std::vector<T>>, "ComponentRangeT<T> must be std::vector<T>");
            return *reinterpret_cast<const std::vector<T>*>(GetConcept()->GetComponentRangePtr());
        }

        [[nodiscard]] std::span<const uint32_t> GetEntityIDsToComponentIndexList() const
        {
            return GetConcept()->GetEntityIDsToComponentIndexList();
        }

        [[nodiscard]] std::span<const EntityID> GetComponentIndexToEntityIDList() const
        {
            return GetConcept()->GetComponentIndexToEntityIDList();
        }

        [[nodiscard]] std::type_index GetType() const
        {
            return mType;
        }

        [[nodiscard]] bool IsMovedFrom() const
        {
            return mType == std::type_index(typeid(void));
        }

        [[nodiscard]] bool operator==(const ComponentPoolNew& other) const
        {
            if (mType != other.mType)
            {
                return false;
            }
            return GetConcept()->Equals(other.GetConcept());
        }

        template<typename T>
        using ComponentRangeT = std::vector<T>;


        struct ComponentSentinel
        {};

        template<typename T>
        class ComponentIterator
        {
        public:

            ComponentIterator() = default;
            ComponentIterator(void* beginPtr, size_t componentCount, size_t componentSize)
                : mStartPtr(beginPtr)
                , mCurrentPtr(beginPtr)
                , mEndPtr(beginPtr + ByteOffset{ componentCount * componentSize })
                , mComponentSize(componentSize)
            {}

            T& operator*() const
            {
                return *reinterpret_cast<T*>(mCurrentPtr);
            }

            ComponentIterator& operator++()
            {
                mCurrentPtr += ByteOffset{ mComponentSize };
                return *this;
            }

            [[nodiscard]] friend bool operator==(const ComponentIterator& a, const ComponentIterator& b)
            {
                return a.mCurrentPtr == b.mCurrentPtr;
            }

            [[nodiscard]] friend bool operator==(const ComponentIterator& it, const ComponentSentinel&)
            {
                return  it.mCurrentPtr == it.mEndPtr;
            }

            [[nodiscard]] size_t GetIndex() const
            {
                return (static_cast<char*>(mCurrentPtr) - static_cast<char*>(mStartPtr)) / mComponentSize;
            }

        private:

            void* mStartPtr = nullptr;
            void* mCurrentPtr = nullptr;
            void* mEndPtr = nullptr;
            size_t mComponentSize = 0;
        };


        template<typename T>
        class ComponentIteratorHelper
        {
        public:

            ComponentIteratorHelper(void* dataPtr, size_t componentCount, size_t componentSize)
                : mDataPtr(dataPtr)
                , mComponentCount(componentCount)
                , mComponentSize(componentSize)
            {}

            ComponentIterator<T> begin()
            {
                return ComponentIterator<T>(mDataPtr, mComponentCount, mComponentSize);
            }

            ComponentSentinel end()
            {
                return ComponentSentinel();
            }
        private:

            void* mDataPtr = nullptr;
            size_t mComponentCount = 0;
            size_t mComponentSize = 0;
        };

        template<typename T>
        ComponentIteratorHelper<T> Iterate()
        {
            ASSERT(mType == std::type_index(typeid(T)), "ComponentPoolSBO::Iterate: Type mismatch");
            auto c = GetConcept();
            return ComponentIteratorHelper<T>(c->Data(), c->GetComponentCount(), sizeof(T));
        }

    private:

        class Concept
        {
        public:

            Concept() = default;
            virtual ~Concept() = default;

            virtual void* AddComponent(EntityID entityID) = 0;
            virtual void* AddComponent(EntityID entityID, const void* defaultValuePtr) = 0;
            virtual void* AddComponentMoved(EntityID entityID, void* defaultValuePtr) = 0;
            [[nodiscard]] virtual void* GetComponent(EntityID entityID) = 0;
            [[nodiscard]] virtual const void* GetComponent(EntityID entityID) const = 0;
            virtual void RemoveComponent(const EntityID entityID) = 0;
            virtual void ReserveComponents(const size_t size) = 0;
            [[nodiscard]] virtual void* Data() = 0;
            [[nodiscard]] virtual const void* Data() const = 0;
            [[nodiscard]] virtual size_t GetComponentSize() const = 0;
            [[nodiscard]] virtual size_t GetComponentCount() const = 0;
            [[nodiscard]] virtual const void* GetComponentRangePtr() const = 0;
            [[nodiscard]] virtual const std::vector<uint32_t>& GetEntityIDsToComponentIndexList() const = 0;
            [[nodiscard]] virtual const std::vector<EntityID>& GetComponentIndexToEntityIDList() const = 0;

            virtual void Clone(Concept* memory) const = 0;
            virtual void Move(Concept* memory) noexcept = 0;
            virtual bool Equals(const Concept* other) const = 0;
            virtual std::type_index GetType() const = 0;
        };

        template<typename T>
        class Model final : public Concept
        {
        public:

            Model() = default;

            void* AddComponent(const EntityID entityID) override
            {
                return &AddComponentInternal(entityID, T{});
            }

            void* AddComponent(const EntityID entityID, const void* defaultValuePtr) override
            {
                ASSERT(defaultValuePtr != nullptr, "ComponentPoolSBO::AddComponent: defaultValuePtr is nullptr");
                return &AddComponentInternal(entityID, *reinterpret_cast<const T*>(defaultValuePtr));
            }

            void* AddComponentMoved(const EntityID entityID, void* defaultValuePtr) override
            {
                ASSERT(defaultValuePtr != nullptr, "ComponentPoolSBO::AddComponentMoved: defaultValuePtr is nullptr");
                T& defaultValue = *reinterpret_cast<T*>(defaultValuePtr);
                return &AddComponentInternal(entityID, std::move(defaultValue));
            }

            template<typename U>
            T& AddComponentInternal(const EntityID entityID, U&& defaultValue)
            {
                if (mEntityIDToComponentIndex.size() <= entityID.id)
                {
                    mEntityIDToComponentIndex.resize(static_cast<size_t>(entityID.id) + 1, InvalidComponentIndex);
                }
                const uint32_t previousComponentIndex = mEntityIDToComponentIndex[entityID.id];
                if (previousComponentIndex != InvalidComponentIndex)
                {
                    mComponents[previousComponentIndex] = std::forward<U>(defaultValue);
                    return mComponents[previousComponentIndex];
                }

                const uint32_t componentIndex = static_cast<uint32_t>(mComponents.size());
                mEntityIDToComponentIndex[entityID.id] = componentIndex;

                mComponents.emplace_back(std::forward<U>(defaultValue));

                ASSERT(componentIndex <= mComponentIndexToEntityID.size(), "Component index should be less than or equal to the size of mComponentIndexToEntityID");
                if (componentIndex == mComponentIndexToEntityID.size())
                {
                    mComponentIndexToEntityID.emplace_back(entityID);
                }
                else
                {
                    mComponentIndexToEntityID[componentIndex] = entityID;
                }
                return mComponents.back();
            }

            [[nodiscard]] void* GetComponent(const EntityID entityID) override
            {
                const uint32_t componentIndex = mEntityIDToComponentIndex[entityID.id];
                if (componentIndex == InvalidComponentIndex)
                {
                    return nullptr;
                }
                return &mComponents[componentIndex];
            }

            [[nodiscard]] const void* GetComponent(const EntityID entityID) const override
            {
                const uint32_t componentIndex = mEntityIDToComponentIndex[entityID.id];
                if (componentIndex == InvalidComponentIndex)
                {
                    return nullptr;
                }
                return &mComponents[componentIndex];
            }

            void RemoveComponent(const EntityID entityID) override
            {
                const uint32_t componentIndex = mEntityIDToComponentIndex[entityID.id];
                const std::size_t lastIndex = mComponents.size() - 1;
                mComponents[componentIndex] = std::move(mComponents[lastIndex]);
                mComponents.pop_back();
                const EntityID lastIndexEntityID = mComponentIndexToEntityID[lastIndex];
                mEntityIDToComponentIndex[lastIndexEntityID.id] = componentIndex;
                mEntityIDToComponentIndex[entityID.id] = InvalidComponentIndex;
                mComponentIndexToEntityID[componentIndex] = lastIndexEntityID;
                mComponentIndexToEntityID.pop_back();
            }

            void ReserveComponents(const std::size_t size) override
            {
                mComponents.reserve(size);
                mEntityIDToComponentIndex.resize(size, InvalidComponentIndex);
            }

            [[nodiscard]] void* Data() override
            {
                return mComponents.data();
            }

            [[nodiscard]] const void* Data() const override
            {
                return mComponents.data();
            }

            [[nodiscard]] size_t GetComponentSize() const override
            {
                return sizeof(T);
            }

            [[nodiscard]] size_t GetComponentCount() const override
            {
                return mComponents.size();
            }

            [[nodiscard]] const void* GetComponentRangePtr() const override
            {
                return &mComponents;
            }

            [[nodiscard]] const std::vector<uint32_t>& GetEntityIDsToComponentIndexList() const override
            {
                return mEntityIDToComponentIndex;
            }

            [[nodiscard]] const std::vector<EntityID>& GetComponentIndexToEntityIDList() const override
            {
                return mComponentIndexToEntityID;
            }

            void Clone(Concept* memory) const override
            {
                std::construct_at(reinterpret_cast<Model*>(memory), *this);
            }

            void Move(Concept* memory) noexcept override
            {
                std::construct_at(reinterpret_cast<Model*>(memory), std::move(*this));
            }

            [[nodiscard]] bool Equals(const Concept* other) const override
            {
                if (typeid(T) != other->GetType())
                {
                    return false;
                }
                const Model* otherModel = reinterpret_cast<const Model*>(other);
                if constexpr (std::equality_comparable<T>)
                {
                    return mComponents == otherModel->mComponents &&
                        mEntityIDToComponentIndex == otherModel->mEntityIDToComponentIndex &&
                        mComponentIndexToEntityID == otherModel->mComponentIndexToEntityID;
                }
                else
                {
                    throw std::runtime_error("ComponentPoolNew::Model<T>::Equals: Type T is not equality comparable");
                }
            }

            [[nodiscard]] std::type_index GetType() const override
            {
                return std::type_index(typeid(T));
            }

        private:

            ComponentRangeT<T> mComponents;
            std::vector<uint32_t> mEntityIDToComponentIndex;
            std::vector<EntityID> mComponentIndexToEntityID;
        };

    private:

        [[nodiscard]] Concept* GetConcept()
        {
            return reinterpret_cast<Concept*>(mBuffer.data());
        }

        [[nodiscard]] const Concept* GetConcept() const
        {
            return reinterpret_cast<const Concept*>(mBuffer.data());
        }


    private:

        static constexpr std::size_t BufferSize = sizeof(Model<std::byte>);
        static constexpr std::size_t BufferAlignment = 16;

        alignas(BufferAlignment) std::array<std::byte, BufferSize> mBuffer = {};
        std::type_index mType;
    };
}