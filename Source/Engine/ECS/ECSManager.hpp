#pragma once
#include <map>
#include <cstdint>
#include <limits>

#include "Engine/ECS/ECS.hpp"

namespace CLX
{

    using ECSID = uint32_t;

    class ECSManager
    {
        friend class ECSOwningHandle;
    public:

        ECSManager() = default;

        [[nodiscard]] ECS& GetECS(const ECSID ecsID)
        {
            ASSERT(mECSMap.contains(ecsID));
            return mECSMap.at(ecsID);
        }

        [[nodiscard]] const ECS& GetECS(const ECSID ecsID) const
        {
            ASSERT(mECSMap.contains(ecsID));
            return mECSMap.at(ecsID);
        }

        [[nodiscard]] ECSID CreateECS(const ECSRegistry& registry)
        {
            const ECSID ecsID = mECSIDCounter++;
            ASSERT(!mECSMap.contains(ecsID));
            mECSMap.emplace(ecsID, ECS(registry));
            return ecsID;
        }

        [[nodiscard]] ECSID CopyECS(const ECSID& other)
        {
            const ECSID ecsID = mECSIDCounter++;
            ECS& otherECS = GetECS(other);
            ASSERT(!mECSMap.contains(ecsID));
            mECSMap.emplace(ecsID, ECS(otherECS));
            return ecsID;
        }

        [[nodiscard]] std::size_t GetECSCount() const
        {
            return mECSMap.size();
        }

    private:

        std::map<ECSID, ECS> mECSMap;
        ECSID mECSIDCounter = 0;
    };

    /*
    * Handle with value semantics that manages the lifetime of an ECS.
    When the handle is destroyed, the associated ECS is also destroyed. 
    Copying the handle creates a new ECS with the same data.
    */
    class ECSOwningHandle final
    {
    public:

        constexpr ECSOwningHandle(ECSManager& ecsManager, const ECSID ecsID)
            : mECSManager(&ecsManager)
            , mECSID(ecsID)
        {}

        ECSOwningHandle(const ECSOwningHandle& other)
            : mECSManager(other.mECSManager)
        {
            mECSID = mECSManager->CopyECS(other.mECSID);
        }

        ECSOwningHandle(ECSOwningHandle&& other) noexcept
            : mECSManager(std::exchange(other.mECSManager, nullptr))
            , mECSID(std::exchange(other.mECSID, std::numeric_limits<ECSID>::max()))
        {
        }

        ECSOwningHandle& operator=(const ECSOwningHandle& other)
        {
            if (this != &other)
            {
                mECSManager = other.mECSManager;
                mECSID = other.mECSManager->CopyECS(other.mECSID);
            }
            return *this;
        }

        ECSOwningHandle& operator=(ECSOwningHandle&& other) noexcept
        {
            if (this != &other)
            {
                mECSManager->mECSMap.erase(mECSID);
                mECSManager = std::exchange(other.mECSManager, nullptr);
                mECSID = std::exchange(other.mECSID, std::numeric_limits<ECSID>::max());
            }
            return *this;
        }

        ~ECSOwningHandle()
        {
            if (mECSID != std::numeric_limits<ECSID>::max())
            {
                mECSManager->mECSMap.erase(mECSID);
            }
            mECSID = std::numeric_limits<ECSID>::max();
            mECSManager = nullptr;
        }

        [[nodiscard]] ECS& Get()
        {
            return mECSManager->GetECS(mECSID);
        }

        [[nodiscard]] const ECS& Get() const
        {
            return mECSManager->GetECS(mECSID);
        }

        [[nodiscard]] ECSID GetID() const
        {
            return mECSID;
        }

    private:

        ECSManager* mECSManager;
        ECSID mECSID = std::numeric_limits<ECSID>::max();
    };


    inline ECSOwningHandle CreateECS(ECSManager& ecsManager, const ECSRegistry& registry)
    {
        return ECSOwningHandle(ecsManager, ecsManager.CreateECS(registry));
    }
}