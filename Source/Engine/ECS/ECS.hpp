#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <span>
#include <queue>

#include "ECSRegistry.hpp"
#include "EntityID.hpp"
#include "ComponentPool.hpp"
#include "ECSSystem.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/Utility/TypeList.hpp"

namespace CLX
{

    template<typename T>
    struct FunctionTraits;

    // Function pointer
    template<typename R, typename... Args>
    struct FunctionTraits<R(*)(Args...)>
    {
        using Return_Type = R;
        using Args_Tuple = std::tuple<Args...>;
        using Args_TypeList = TypeList<Args...>;
        static constexpr std::size_t arity = sizeof...(Args);

        template<std::size_t N>
        struct Arg
        {
            static_assert(N < arity, "Invalid parameter index.");
            using Type = typename std::tuple_element<N, Args_Tuple>::type;
        };
    };

    // Lambda / functor (operator())
    template<typename C>
    struct FunctionTraits
    {
    private:
        using Call_Type = FunctionTraits<decltype(&C::operator())>;
    public:
        using Return_Type = typename Call_Type::Return_Type;
        using Args_Tuple = typename Call_Type::Args_Tuple;
        using Args_TypeList = typename Call_Type::Args_TypeList;
        static constexpr std::size_t arity = Call_Type::arity;

        template<std::size_t N>
        struct Arg
        {
            using Type = typename Call_Type::template Arg<N>::Type;
        };
    };

    // Member function pointer
    template<typename C, typename R, typename... Args>
    struct FunctionTraits<R(C::*)(Args...) const>
    {
        using Return_Type = R;
        using Args_Tuple = std::tuple<Args...>;
        using Args_TypeList = TypeList<Args...>;
        static constexpr std::size_t arity = sizeof...(Args);

        template<std::size_t N>
        struct Arg
        {
            using Type = typename std::tuple_element<N, Args_Tuple>::type;
        };
    };



    [[nodiscard]] constexpr std::vector<std::size_t> BitMaskToIndices(std::size_t mask)
    {
        std::vector<std::size_t> result;

        for (std::size_t i = 0; mask != 0; ++i, mask >>= 1)
        {
            if (mask & 1)
            {
                result.push_back(i);
            }
        }
        return result;
    }

    struct ECSRegisterComponentHelper
    {
        template<typename T>
        inline static std::size_t ComponentPoolID{ 1 };
    };

    class EntityIDConverter final
    {
    public:

        explicit EntityIDConverter(std::vector<EntityID>&& entityIDConverter)
            : mEntityIDConverter(std::move(entityIDConverter))
        {}

        [[nodiscard]] EntityID operator[](const EntityID entityID) const
        {
            return mEntityIDConverter[entityID.id];
        }

    private:

        std::vector<EntityID> mEntityIDConverter;
    };


    class ECS final
    {
    public:
        using ComponentMask = std::size_t;

        static EntityID FindFirstValidEntityID(const ECS& ecs, const EntityID startID = EntityID{ 0 })
        {
            EntityID entityID = startID;
            while (entityID.id < size(ecs.mEntityData) && !ecs.mEntityData[entityID.id].isActive)
            {
                entityID.id++;
            }
            return entityID;
        }
#pragma region Views

        template<bool IncludeEntityID, typename... Ts>
        class ComponentView final
        {
            class ComponentIterator final
            {
            public:

                constexpr ComponentIterator(ECS* ecs, std::span<const EntityID> entityIDs, const std::size_t entityIndex = 0)
                    : mECS(ecs)
                    , mEntityIDs(entityIDs)
                    , mEntityIndex(entityIndex)
                {}

                ComponentIterator& operator++()
                {
                    mEntityIndex++;
                    return *this;
                }

                [[nodiscard]] auto operator*() requires (sizeof...(Ts) > 1)
                {
                    const EntityID entityID = mEntityIDs[mEntityIndex];

                    if constexpr (IncludeEntityID)
                    {
                        return std::tuple<EntityID, Ts&...>{ entityID, (*mECS->GetComponent<Ts>(entityID))... };
                    }
                    else
                    {
                        if constexpr (sizeof...(Ts) == 1)
                        {
                            return *mECS->GetComponent<std::tuple_element_t<0, std::tuple<Ts...>>>(entityID);
                        }
                        else
                        {
                            return std::tuple<Ts&...>{ (*mECS->GetComponent<Ts>(entityID))... };
                        }
                    }
                }


                [[nodiscard]] decltype(auto) operator*() requires (sizeof...(Ts) == 1)
                {
                    const EntityID entityID = mEntityIDs[mEntityIndex];

                    if constexpr (IncludeEntityID)
                    {
                        return std::tuple<EntityID, Ts&...>{ entityID, (*mECS->GetComponent<Ts>(entityID))... };
                    }
                    else
                    {
                        return std::tuple<Ts&...>{ (*mECS->GetComponent<Ts>(entityID))... };
                    }
                }

                [[nodiscard]] friend constexpr bool operator==(const ComponentIterator& a, const ComponentIterator& b) noexcept
                {
                    return a.mEntityIndex == b.mEntityIndex;
                }

            private:

                std::span<const EntityID> mEntityIDs;
                ECS* mECS = nullptr;
                std::size_t mEntityIndex = 0;
            };

        public:
            constexpr ComponentView(ECS* ecs, std::span<const EntityID> entityIDs)
                : mECS(ecs)
                , mEntityIDs(entityIDs)
            {}

            [[nodiscard]] ComponentIterator begin() const
            {
                return ComponentIterator(mECS, mEntityIDs);
            }

            [[nodiscard]] ComponentIterator end() const
            {
                return ComponentIterator(mECS, mEntityIDs, size(mEntityIDs));
            }

            [[nodiscard]] std::size_t GetSize() const
            {
                return mEntityIDs.size();
            }

            [[nodiscard]] bool IsEmpty() const
            {
                return GetSize() == 0;
            }

        private:

            std::span<const EntityID> mEntityIDs;
            ECS* mECS = nullptr;
        };

        //template<bool IncludeEntityID, typename... Ts>
        //class ConstComponentView final
        //{
        //    class ConstComponentIterator final
        //    {
        //    public:

        //        constexpr ConstComponentIterator(const ECS* ecs, std::span<const EntityID> entityIDs, uint32_t entityIndex = 0)
        //            : mECS(ecs)
        //            , mEntityIDs(entityIDs)
        //            , mEntityIndex(entityIndex)
        //        {
        //        }

        //        ConstComponentIterator& operator++()
        //        {
        //            mEntityIndex++;
        //            return *this;
        //        }

        //        [[nodiscard]] decltype(auto) operator*() /*requires (sizeof...(Ts) > 1)*/
        //        {
        //            const EntityID entityID = mEntityIDs[mEntityIndex];

        //            if constexpr (IncludeEntityID)
        //            {
        //                return std::tuple<EntityID, const Ts&...>{ entityID, (*mECS->GetComponent<Ts>(entityID))... };
        //            }
        //            else
        //            {
        //                if constexpr (sizeof...(Ts) == 1)
        //                {
        //                    return *mECS->GetComponent<std::tuple_element_t<0, std::tuple<Ts...>>>(entityID);
        //                }
        //                else
        //                {
        //                    return std::tuple<const Ts&...>{ (*mECS->GetComponent<Ts>(entityID))... };
        //                }
        //            }
        //        }


        //      /*  [[nodiscard]] decltype(auto) operator*() requires (sizeof...(Ts) == 1)
        //        {
        //            const auto& list = mEntityIDs[mListIndex];
        //            const EntityID entityID = list[mEntityIndex];

        //            if constexpr (IncludeEntityID)
        //            {
        //                return std::tuple<EntityID, const Ts&...>{ entityID, (*mECS->GetComponent<Ts>(entityID))... };
        //            }
        //            else
        //            {
        //                return *mECS->GetComponent<std::tuple_element_t<0, std::tuple<Ts...>>>(entityID);
        //            }
        //        }*/

        //        [[nodiscard]] friend bool operator==(const ConstComponentIterator& a, const ConstComponentIterator& b)
        //        {
        //            return a.mListIndex == b.mListIndex && a.mEntityIndex == b.mEntityIndex;
        //        }

        //    private:

        //        uint32_t mEntityIndex = 0;
        //        std::span<const EntityID> mEntityIDs;
        //        const ECS* mECS = nullptr;
        //    };

        //public:
        //    constexpr ConstComponentView(const ECS* ecs, std::vector<EntityID>&& list)
        //        : mECS(ecs)
        //        , mEntityIDs(std::move(list))
        //    {
        //    }

        //    [[nodiscard]] ConstComponentIterator begin() const
        //    {
        //        return ConstComponentIterator(mECS, mEntityIDs);
        //    }

        //    [[nodiscard]] ConstComponentIterator end() const
        //    {
        //        return ConstComponentIterator(mECS, mEntityIDs, static_cast<uint32_t>(mEntityIDs.size()));
        //    }

        //    [[nodiscard]] std::size_t GetSize() const
        //    {
        //        return size(mEntityIDs);
        //    }

        //    [[nodiscard]] bool IsEmpty() const
        //    {
        //        return GetSize() == 0;
        //    }

        //private:

        //    std::vector<EntityID> mEntityIDs;
        //    const ECS* mECS = nullptr;
        //};

        class EntityView final
        {
            class TypeErasedComponentIterator final
            {
            public:

                TypeErasedComponentIterator(ECS* ecs, EntityID entityID)
                    : mECS(ecs)
                    , mEntityID(entityID)
                    , mComponentTypeIndices(BitMaskToIndices(ecs->mEntityData[entityID.id].mask))
                {}

                TypeErasedComponentIterator(ECS* ecs, EntityID entityID, std::size_t endIndex)
                    : mECS(ecs)
                    , mEntityID(entityID)
                    , mComponentTypeIndices(BitMaskToIndices(ecs->mEntityData[entityID.id].mask))
                    , mCurrentIndex(endIndex)
                {}

                TypeErasedComponentIterator& operator++()
                {
                    mCurrentIndex++;
                    return *this;
                }

                [[nodiscard]] std::pair<const std::type_info&, void*> operator*()
                {
                    auto& componentPool = mECS->GetComponentPool(mComponentTypeIndices[mCurrentIndex]);
                    return std::pair<const std::type_info&, void*>{ componentPool.GetTypeInfo(), componentPool.GetComponent(mEntityID) };
                }

                [[nodiscard]] friend bool operator==(const TypeErasedComponentIterator& a, const TypeErasedComponentIterator& b)
                {
                    return a.mCurrentIndex == b.mCurrentIndex && a.mEntityID == b.mEntityID;
                }

            private:

                ECS* mECS = nullptr;
                EntityID mEntityID;
                std::vector<std::size_t> mComponentTypeIndices;
                std::size_t mCurrentIndex = 0;
            };
        public:

            EntityView(ECS* ecs, EntityID entityID)
                : mECS(ecs)
                , mEntityID(entityID)
            {
            }

            [[nodiscard]] TypeErasedComponentIterator begin() const
            {
                return TypeErasedComponentIterator(mECS, mEntityID);
            }

            [[nodiscard]] TypeErasedComponentIterator end() const
            {
                return TypeErasedComponentIterator(mECS, mEntityID, BitMaskToIndices(mECS->mEntityData[mEntityID.id].mask).size());
            }

            [[nodiscard]] EntityID GetEntityID() const
            {
                return mEntityID;
            }

            template<typename T>
            [[nodiscard]] T* GetComponent()
            {
                return mECS->GetComponent<T>(mEntityID);
            }

            /*void SetIsActive(const bool isActive)
            {
                mECS->mEntityData[mEntityID.id].isActive = isActive;
            }*/

        private:

            ECS* mECS = nullptr;
            EntityID mEntityID;
        };

        class ConstEntityView final
        {
            class ConstTypeErasedComponentIterator final
            {
            public:

                ConstTypeErasedComponentIterator(const ECS* ecs, EntityID entityID)
                    : mECS(ecs)
                    , mEntityID(entityID)
                    , mComponentTypeIndices(BitMaskToIndices(ecs->mEntityData[entityID.id].mask))
                {}

                ConstTypeErasedComponentIterator(const ECS* ecs, EntityID entityID, std::size_t endIndex)
                    : mECS(ecs)
                    , mEntityID(entityID)
                    , mComponentTypeIndices(BitMaskToIndices(ecs->mEntityData[entityID.id].mask))
                    , mCurrentIndex(endIndex)
                {}

                ConstTypeErasedComponentIterator& operator++()
                {
                    mCurrentIndex++;
                    return *this;
                }

                [[nodiscard]] std::pair<const std::type_info&, const void*> operator*()
                {
                    const auto& componentPool = mECS->GetComponentPool(mComponentTypeIndices[mCurrentIndex]);
                    return std::pair<const std::type_info&, const void*>{ componentPool.GetTypeInfo(), componentPool.GetComponent(mEntityID) };
                }

                [[nodiscard]] friend bool operator==(const ConstTypeErasedComponentIterator& a, const ConstTypeErasedComponentIterator& b)
                {
                    return a.mCurrentIndex == b.mCurrentIndex && a.mEntityID == b.mEntityID;
                }

            private:

                const ECS* mECS = nullptr;
                EntityID mEntityID;
                std::vector<std::size_t> mComponentTypeIndices;
                std::size_t mCurrentIndex = 0;
            };
        public:

            ConstEntityView(const ECS* ecs, const EntityID entityID)
                : mECS(ecs)
                , mEntityID(entityID)
            {}

            [[nodiscard]] ConstTypeErasedComponentIterator begin() const
            {
                return ConstTypeErasedComponentIterator(mECS, mEntityID);
            }

            [[nodiscard]] ConstTypeErasedComponentIterator end() const
            {
                return ConstTypeErasedComponentIterator(mECS, mEntityID, BitMaskToIndices(mECS->mEntityData[mEntityID.id].mask).size());
            }

            [[nodiscard]] EntityID GetEntityID() const
            {
                return mEntityID;
            }

        private:

            const ECS* mECS = nullptr;
            EntityID mEntityID;
        };

        class EntityIterator final
        {
        public:

            EntityIterator(EntityID entityID, ECS* ecs)
                : mEntityID(entityID)
                , mECS(ecs)
            {}

            EntityIterator& operator++()
            {
                ++mEntityID.id;
                mEntityID = FindFirstValidEntityID(*mECS, mEntityID);
                return *this;
            }

            [[nodiscard]] EntityView operator*() const
            {
                return EntityView(mECS, mEntityID);
            }

            [[nodiscard]] friend bool operator==(const EntityIterator& a, const EntityIterator& b)
            {
                return a.mEntityID == b.mEntityID;
            }

        private:

            EntityID mEntityID;
            ECS* mECS = nullptr;
        };

        class ConstEntityIterator final
        {
        public:

            ConstEntityIterator(EntityID entityID, const ECS* ecs)
                : mEntityID(entityID)
                , mECS(ecs)
            {}

            ConstEntityIterator& operator++()
            {
                ++mEntityID.id;
                mEntityID = FindFirstValidEntityID(*mECS, mEntityID);
                return *this;
            }

            [[nodiscard]] ConstEntityView operator*() const
            {
                return ConstEntityView(mECS, mEntityID);
            }

            [[nodiscard]] friend bool operator==(const ConstEntityIterator& a, const ConstEntityIterator& b)
            {
                return a.mEntityID == b.mEntityID;
            }

        private:

            EntityID mEntityID;
            const ECS* mECS = nullptr;
        };

        class EntityCollectionView final
        {
        public:

            EntityCollectionView(ECS* ecs)
                : mECS(ecs)
            {}

            [[nodiscard]] EntityIterator begin() const
            {
                return EntityIterator(FindFirstValidEntityID(*mECS), mECS);
            }

            [[nodiscard]] EntityIterator end() const
            {
                return EntityIterator(EntityID(static_cast<unsigned int>(size(mECS->mEntityData))), mECS);
            }

            [[nodiscard]] bool IsEmpty() const
            {
                return begin() == end();
            }

            [[nodiscard]] std::size_t GetCount() const
            {
                return mECS->GetEntityCount();
            }

        private:

            ECS* mECS = nullptr;
        };

        class ConstEntityCollectionView final
        {
        public:

            ConstEntityCollectionView(const ECS* ecs)
                : mECS(ecs)
            {}

            [[nodiscard]] ConstEntityIterator begin() const
            {
                return ConstEntityIterator(FindFirstValidEntityID(*mECS), mECS);
            }

            [[nodiscard]] ConstEntityIterator end() const
            {
                return ConstEntityIterator(EntityID(static_cast<unsigned int>(size(mECS->mEntityData))), mECS);
            }

            [[nodiscard]] bool IsEmpty() const
            {
                return begin() == end();
            }

            [[nodiscard]] std::size_t GetCount() const
            {
                return mECS->GetEntityCount();
            }
        private:

            const ECS* mECS = nullptr;
        };
#pragma endregion

        struct EntityData
        {
            ComponentMask mask;
            uint32_t generation = 0;
            bool isActive = true;
        };

    public:

        ECS() = delete;
        explicit ECS(const ECSRegistry& ecsRegistry);

        EntityID CreateEntity();
        void DestroyEntity(const EntityID entityID);

        EntityID CopyEntity(const EntityID entityID, ECS& targetECS) const;
        EntityID DuplicateEntity(const EntityID entityID);
        void ReplaceEntity(const EntityID replaceEntityID, const ECS& sourceECS, const EntityID sourceEntityID);

        void ActivateEntity(const EntityID entityID);
        void DeactivateEntity(const EntityID entityID);

        [[nodiscard]] constexpr bool IsEntityValid(const EntityID entityID) const;

        template<typename T>
        T& AddComponent(const EntityID entityID);

        template<typename T>
        T& AddComponent(const EntityID entityID, const T& defaultValue);

        template<typename T>
        T& InsertComponent(const EntityID entityID);

        template<typename T>
        void RemoveComponent(const EntityID entityID);

        void RemoveComponent(const EntityID entityID, const std::type_index& typeIndex);

        template<typename T>
        [[nodiscard]] T* GetComponent(const EntityID entityID);

        template<typename T>
        [[nodiscard]] const T* GetComponent(const EntityID entityID) const;

        template<typename T>
        [[nodiscard]] bool HasComponent(const EntityID entityID) const;

        [[nodiscard]] bool HasComponent(const EntityID entityID, const std::type_index& typeIndex) const;

        template<typename T>
        [[nodiscard]] bool HasComponent2(const EntityID entityID) const;

        template<typename T>
        [[nodiscard]] std::size_t GetComponentTypeIndex() const;

        template<typename T>
        [[nodiscard]] std::size_t GetComponentCount() const;

        //template<typename... Ts>
        //[[nodiscard]] ComponentView<false, Ts...> View();

        //template<typename... Ts>
        //[[nodiscard]] ConstComponentView<false, Ts...> View() const;

        //template<typename... Ts>
        //[[nodiscard]] ComponentView<true, Ts...> ViewUsingEntityID();

        //template<typename... Ts>
        //[[nodiscard]] ConstComponentView<true, Ts...> ViewUsingEntityID() const;

        template<typename Func>
        void ForEach(Func&& func);

        template<typename Func>
        void ForEach(Func&& func) const;

        [[nodiscard]] EntityCollectionView ViewEntities();
        [[nodiscard]] ConstEntityCollectionView ViewEntities() const;
        [[nodiscard]] EntityView ViewEntity(const EntityID entityID);

        [[nodiscard]] std::size_t GetEntityCount() const;

        void BeginPlay(const Blackboard& blackboard);
        void EndPlay(const Blackboard& blackboard);
        void EarlyUpdate(const float deltaTime, const Blackboard& blackboard);
        void Update(const float deltaTime, const Blackboard& blackboard);
        void EditorUpdate(const Blackboard& blackboard);
        void Render(const Blackboard& blackboard) const;

        [[nodiscard]] const ECSRegistry& GetRegistry() const;

    private:

        [[nodiscard]] ComponentPoolSBO& GetComponentPool(const std::size_t componentTypeIndex);
        [[nodiscard]] const ComponentPoolSBO& GetComponentPool(const std::size_t componentTypeIndex) const;

        template<typename... Ts>
        [[nodiscard]] ComponentMask CreateComponentMask() const;

        template<typename Func>
        [[nodiscard]] ComponentMask CreateComponentMaskFromFunction() const;

        template<typename... Args>
        [[nodiscard]] ComponentMask CreateComponentMaskFromTypeList(TypeList<Args...>) const;

        void UpdateEntityMask(const EntityID entityID, const bool setValue, const std::type_index& typeIndex);

        template<typename T>
        void UpdateEntityMask(const EntityID entityID, const bool setValue);

        void UpdateEntityMask(const EntityID entityID, const bool setValue, const std::size_t componentTypeIndex);

        void UpdateEntityMask(const EntityID entityID, const ComponentMask newMask);

        template<typename Func, typename... Args> requires not std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
        void CallWithComponentsImpl(EntityID id, const Func& func, TypeList<Args...>);

        template<typename Func> requires not std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
        void CallWithComponents(EntityID id, const Func& func);

        template<typename Func, typename... Args> requires std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
        void CallWithComponentsImpl(EntityID id, const Func& func, TypeList<Args...>);

        template<typename Func> requires std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
        void CallWithComponents(EntityID id, const Func& func);

        template<typename Func, typename... Args> requires not std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
        void CallWithComponentsImpl(EntityID id, const Func& func, TypeList<Args...>) const;

        template<typename Func> requires not std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
        void CallWithComponents(EntityID id, const Func& func) const;

        template<typename Func, typename... Args> requires std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
        void CallWithComponentsImpl(EntityID id, const Func& func, TypeList<Args...>) const;

        template<typename Func> requires std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
        void CallWithComponents(EntityID id, const Func& func) const;


    private:

        std::vector<ComponentPoolSBO> mComponentPools;
        std::unordered_map<ComponentMask, std::vector<uint32_t>> mEntitiesByMask;
        std::vector<EntityData> mEntityData;
        std::vector<System> mSystems;
        std::queue<uint32_t> mFreeEntityIDs;
        ECSRegistry mRegistry;
        std::vector<EntityID> mIterationList;
    };

    constexpr bool ECS::IsEntityValid(const EntityID entityID) const
    {
        return entityID.id < size(mEntityData) && mEntityData[entityID.id].generation == entityID.generation;
    }

    template<typename T>
    T& ECS::AddComponent(const EntityID entityID)
    {
        const std::size_t componentTypeIndex = GetComponentTypeIndex<T>();
        T& component = GetComponentPool(componentTypeIndex).AddComponent<T>(entityID, nullptr);
        UpdateEntityMask<T>(entityID, true);
        if constexpr (requires { Initialize(component, mRegistry.GetBlackboard()); })
        {
            Initialize(component, mRegistry.GetBlackboard());
        }
        return component;
    }

    template<typename T>
    T& ECS::AddComponent(const EntityID entityID, const T& defaultValue)
    {
        const std::size_t componentTypeIndex = GetComponentTypeIndex<T>();
        T& component = GetComponentPool(componentTypeIndex).AddComponent(entityID, &defaultValue);
        UpdateEntityMask<T>(entityID, true);
        return component;
    }

    template<typename T>
    T& ECS::InsertComponent(const EntityID entityID)
    {
        if (GetComponent<T>(entityID) == nullptr)
        {
            return AddComponent(entityID, T{});
        }
        const std::size_t componentTypeIndex = GetComponentTypeIndex<T>();
        return *GetComponentPool(componentTypeIndex).GetComponent<T>(entityID);
    }

    template<typename T>
    void ECS::RemoveComponent(const EntityID entityID)
    {
        const std::size_t componentTypeIndex = GetComponentTypeIndex<T>();
        GetComponentPool(componentTypeIndex).RemoveComponent(entityID);
        UpdateEntityMask<T>(entityID, false);
    }

    template<typename T>
    T* ECS::GetComponent(const EntityID entityID)
    {
        if (!IsEntityValid(entityID))
        {
            return nullptr;
        }
        const std::size_t componentTypeIndex = GetComponentTypeIndex<T>();
        return GetComponentPool(componentTypeIndex).GetComponent<T>(entityID);
    }

    template<typename T>
    const T* ECS::GetComponent(const EntityID entityID) const
    {
        if (!IsEntityValid(entityID))
        {
            return nullptr;
        }
        const std::size_t componentTypeIndex = GetComponentTypeIndex<T>();
        return GetComponentPool(componentTypeIndex).GetComponent<T>(entityID);
    }

    template<typename T>
    bool ECS::HasComponent(const EntityID entityID) const
    {
        const std::size_t componentTypeIndex = GetComponentTypeIndex<T>();
        return GetComponentPool(componentTypeIndex).HasComponent(entityID);
    }

    inline bool ECS::HasComponent(const EntityID entityID, const std::type_index& typeIndex) const
    {
        const std::size_t componentTypeIndex = mRegistry.GetComponentTypeIndex(typeIndex);
        if (componentTypeIndex == 0)
        {
            return false;
        }
        return GetComponentPool(componentTypeIndex).HasComponent(entityID);
    }

    template<typename T>
    bool ECS::HasComponent2(const EntityID entityID) const
    {
        const std::size_t componentTypeIndex = GetComponentTypeIndex<T>();
        return mEntityData[entityID.id].mask & (1ULL << componentTypeIndex);
    }

    template<typename T>
    std::size_t ECS::GetComponentTypeIndex() const
    {
        const std::size_t typeIndex = ECSRegisterComponentHelper::ComponentPoolID<T>;
        const std::size_t registeredTypeIndex = mRegistry.GetComponentTypeIndex(std::type_index(typeid(T)));
        if (typeIndex != registeredTypeIndex)
        {
            return registeredTypeIndex;
        }
        return typeIndex;
    }

    template<typename T>
    std::size_t ECS::GetComponentCount() const
    {
        const std::size_t componentTypeIndex = GetComponentTypeIndex<T>();
        return GetComponentPool(componentTypeIndex).GetComponentCount();
    }

    inline ComponentPoolSBO& ECS::GetComponentPool(const std::size_t componentTypeIndex)
    {
        return mComponentPools[componentTypeIndex - 1];
    }

    inline const ComponentPoolSBO& ECS::GetComponentPool(const std::size_t componentTypeIndex) const
    {
        return mComponentPools[componentTypeIndex - 1];
    }

    template<typename... Ts>
    typename ECS::ComponentMask ECS::CreateComponentMask() const
    {
        std::size_t mask{};

        ((mask |= 1ULL << GetComponentTypeIndex<Ts>()), ...);
        return mask;
    }

    template<typename... Ts>
    struct RemoveReference {};

    template<template<typename...> typename TupleType, typename... Ts>
    struct RemoveReference<TupleType<Ts...>>
    {
        using Type = TupleType<std::remove_cvref_t<Ts>...>;
    };

    template<typename... Args>
    typename ECS::ComponentMask ECS::CreateComponentMaskFromTypeList(TypeList<Args...>) const
    {
        std::size_t mask{};
        ((mask |= 1ULL << GetComponentTypeIndex<std::remove_cvref_t<Args>>()), ...);
        return mask;
    }

    template<typename Func>
    typename ECS::ComponentMask ECS::CreateComponentMaskFromFunction() const
    {
        using Args_TypeList = typename FunctionTraits<Func>::Args_TypeList;

        if constexpr (std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>)
        {
            return CreateComponentMaskFromTypeList(typename Args_TypeList::template Split<1>::Right::TypeList{});
        }
        else
        {
            return CreateComponentMaskFromTypeList(Args_TypeList{});
        }
    }

    //template<typename... Ts>
    //typename ECS::ComponentView<false, Ts...> ECS::View()
    //{
    //    const ComponentMask targetMask = CreateComponentMask<Ts...>();

    //    mIterationList.clear();
    //    for (auto& [mask, ids] : mEntitiesByMask)
    //    {
    //        if ((mask & targetMask) == targetMask)
    //        {
    //            if (!ids.empty())
    //            {
    //                mIterationList.insert(end(mIterationList), begin(ids), end(ids));
    //            }
    //        }
    //    }
    //    return ComponentView<false, Ts...>(this, mIterationList);
    //}

    //template<typename... Ts>
    //typename ECS::ConstComponentView<false, Ts...> ECS::View() const
    //{
    //    const ComponentMask targetMask = CreateComponentMask<Ts...>();

    //    std::vector<EntityID> list;
    //    for (auto& [mask, ids] : mEntitiesByMask)
    //    {
    //        if ((mask & targetMask) == targetMask)
    //        {
    //            if (!ids.empty())
    //            {
    //                std::ranges::transform(ids, std::back_inserter(list), [this](const uint32_t id)
    //                {
    //                    return EntityID{ id, mEntityData[id].generation };
    //                    });
    //                //list.insert(end(list), begin(ids), end(ids));
    //            }
    //        }
    //    }
    //    return ConstComponentView<false, Ts...>(this, std::move(list));
    //}

    /*template<typename... Ts>
    typename ECS::ComponentView<true, Ts...> ECS::ViewUsingEntityID()
    {
        const ComponentMask targetMask = CreateComponentMask<Ts...>();

        mIterationList.clear();
        for (auto& [mask, ids] : mEntitiesByMask)
        {
            if ((mask & targetMask) == targetMask)
            {
                if (!ids.empty())
                {
                    mIterationList.insert(end(mIterationList), begin(ids), end(ids));
                }
            }
        }

        return ComponentView<true, Ts...>(this, mIterationList);
    }

    template<typename... Ts>
    typename ECS::ConstComponentView<true, Ts...> ECS::ViewUsingEntityID() const
    {
        const ComponentMask targetMask = CreateComponentMask<Ts...>();

        std::vector<std::span<const EntityID>> list;
        for (auto& [mask, ids] : mEntitiesByMask)
        {
            if ((mask & targetMask) == targetMask)
            {
                if (!ids.empty())
                {
                    list.push_back(ids);
                }
            }
        }

        return ConstComponentView<true, Ts...>(this, std::move(list));
    }*/

    template<typename Func, typename... Args> requires not std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
    void ECS::CallWithComponentsImpl(EntityID id, const Func& func, TypeList<Args...>)
    {
        // Expand parameter pack: GetComponent<Arg>(id) for each Arg
        std::invoke(func, (*GetComponent<std::decay_t<Args>>(id))...);
    }

    template<typename Func> requires not std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
    void ECS::CallWithComponents(EntityID id, const Func& func)
    {
        using ArgsTypeList = typename FunctionTraits<std::decay_t<Func>>::Args_TypeList;

        CallWithComponentsImpl(id, func, ArgsTypeList{});
    }

    template<typename Func, typename... Args> requires std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
    void ECS::CallWithComponentsImpl(EntityID id, const Func& func, TypeList<Args...>)
    {
        std::apply(func, [&]<typename... As> (TypeList<As...>)
        {
            return std::tuple_cat(std::make_tuple(std::ref(id)), std::make_tuple((std::ref(*GetComponent<std::remove_cvref_t<As>>(id)))...));
        }(typename TypeList<Args...>::template Split<1>::Right::TypeList{}));
    }

    template<typename Func> requires std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
    void ECS::CallWithComponents(EntityID id, const Func& func)
    {
        using ArgsTypeList = typename FunctionTraits<std::decay_t<Func>>::Args_TypeList;

        CallWithComponentsImpl(id, func, ArgsTypeList{});
    }

    template<typename Func, typename... Args> requires not std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
    void ECS::CallWithComponentsImpl(EntityID id, const Func& func, TypeList<Args...>) const
    {
        // Expand parameter pack: GetComponent<Arg>(id) for each Arg
        std::invoke(func, (*GetComponent<std::decay_t<Args>>(id))...);
    }

    template<typename Func> requires not std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
    void ECS::CallWithComponents(EntityID id, const Func& func) const
    {
        using ArgsTypeList = typename FunctionTraits<std::decay_t<Func>>::Args_TypeList;

        CallWithComponentsImpl(id, func, ArgsTypeList{});
    }

    template<typename Func, typename... Args> requires std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
    void ECS::CallWithComponentsImpl(EntityID id, const Func& func, TypeList<Args...>) const
    {
        std::apply(func, [&]<typename... As> (TypeList<As...>)
        {
            return std::tuple_cat(std::make_tuple(std::ref(id)), std::make_tuple((std::ref(*GetComponent<std::remove_cvref_t<As>>(id)))...));
        }(typename TypeList<Args...>::template Split<1>::Right::TypeList{}));
    }

    template<typename Func> requires std::same_as<typename FunctionTraits<Func>::template Arg<0>::Type, EntityID>
    void ECS::CallWithComponents(EntityID id, const Func& func) const
    {
        using ArgsTypeList = typename FunctionTraits<std::decay_t<Func>>::Args_TypeList;

        CallWithComponentsImpl(id, func, ArgsTypeList{});
    }

    template<typename Func>
    void ECS::ForEach(Func&& func)
    {
        const ComponentMask targetMask = CreateComponentMaskFromFunction<Func>();

        std::vector<EntityID> entities;
        for (auto& [mask, ids] : mEntitiesByMask)
        {
            if ((mask & targetMask) == targetMask)
            {
                if (!ids.empty())
                {
                    entities.insert(end(entities), begin(ids), end(ids));
                }
            }
        }

        for (const EntityID entityID : entities)
        {
            CallWithComponents(entityID, func);
        }
    }

    template<typename Func>
    void ECS::ForEach(Func&& func) const
    {
        const ComponentMask targetMask = CreateComponentMaskFromFunction<Func>();

        std::vector<EntityID> entities;
        for (auto& [mask, ids] : mEntitiesByMask)
        {
            if ((mask & targetMask) == targetMask)
            {
                if (!ids.empty())
                {
                    entities.insert(end(entities), begin(ids), end(ids));
                }
            }
        }

        for (const EntityID entityID : entities)
        {
            CallWithComponents(entityID, func);
        }
    }

    inline void ECS::UpdateEntityMask(const EntityID entityID, const bool setValue, const std::type_index& typeIndex)
    {
        const std::size_t componentTypeIndex = mRegistry.GetComponentTypeIndex(typeIndex);
        UpdateEntityMask(entityID, setValue, componentTypeIndex);
    }

    template<typename T>
    void ECS::UpdateEntityMask(const EntityID entityID, const bool setValue)
    {
        const std::size_t componentTypeIndex = ECSRegisterComponentHelper::ComponentPoolID<T>;
        UpdateEntityMask(entityID, setValue, componentTypeIndex);
    }

    inline void ECS::UpdateEntityMask(const EntityID entityID, const bool setValue, const std::size_t componentTypeIndex)
    {
        ComponentMask newMask = mEntityData[entityID.id].mask;
        const std::size_t bit = (1ULL << (componentTypeIndex));
        if (setValue)
        {
            newMask |= bit;
        }
        else
        {
            newMask &= ~bit;
        }
        UpdateEntityMask(entityID, newMask);
    }

    inline void ECS::UpdateEntityMask(const EntityID entityID, const ComponentMask newMask)
    {
        EntityData& entityData = mEntityData[entityID.id];
        std::erase(mEntitiesByMask.at(entityData.mask), entityID.id);
        entityData.mask = newMask;
        mEntitiesByMask[entityData.mask].push_back(entityID.id);
    }

    inline std::size_t ECS::GetEntityCount() const
    {
        return size(mEntityData) - size(mFreeEntityIDs);
    }

    template<typename T>
    void ECSRegistry::RegisterComponentType(const bool isDefault)
    {
        if (mComponentTypeToIDMap.contains(std::type_index(typeid(T))))
        {
            throw std::runtime_error("Component type already registered.");
        }

        ECSComponentType componentType;

        componentType.isDefault = isDefault;

        componentType.createComponentPoolFunction = []() -> ComponentPoolSBO
            {
                return ComponentPoolSBO(std::type_identity<T>{});
            };

        componentType.copyComponentFunction = [](void* destinationPtr, const void* sourcePtr)
            {
                T& destination = *reinterpret_cast<T*>(destinationPtr);
                const T& source = *reinterpret_cast<const T*>(sourcePtr);
                destination = source;
            };

        componentType.addComponentFunction = [](ECS& ecs, const EntityID entityID, const void* defaultValuePtr) -> void*
            {
                if (defaultValuePtr != nullptr)
                {
                    const T& defaultValue = *reinterpret_cast<const T*>(defaultValuePtr);
                    T& component = ecs.AddComponent<T>(entityID, defaultValue);
                    return &component;
                }
                else
                {
                    T& component = ecs.AddComponent<T>(entityID);
                    return &component;
                }
            };

        componentType.removeComponentFunction = [](ECS& ecs, const EntityID entityID)
            {
                ecs.RemoveComponent<T>(entityID);
            };

        mComponentTypes.push_back(componentType);

        mComponentTypeToIDMap.emplace(std::type_index(typeid(T)), mCurrentID);

        ECSRegisterComponentHelper::ComponentPoolID<T> = mCurrentID;
        mCurrentID++;

        if (isDefault)
        {
            mDefaultComponentTypeIndices.push_back(std::type_index(typeid(T)));
        }
    }
}