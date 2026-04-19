#pragma once
#include "Engine/ECS/ECSManager.hpp"
#include <limits>

namespace CLX
{

    class ECSHandle final
    {
    public:

        constexpr ECSHandle(ECSManager& ecsManager, const ECSID ecsID)
            : mECSManager(&ecsManager)
            , mECSID(ecsID)
        {}

        [[nodiscard]] ECS* Get() const
        {
            return mECSManager->GetECS(mECSID);
        }

        [[nodiscard]] ECSID GetID() const
        {
            return mECSID;
        }

        [[nodiscard]] ECS* operator->() const
        {
            return Get();
        }

        [[nodiscard]] bool IsValid() const
        {
            return Get() != nullptr;
        }

        [[nodiscard]] explicit operator bool() const
        {
            return Get() != nullptr;
        }

        [[nodiscard]] constexpr bool operator==(const ECSHandle& other) const
        {
            return mECSManager == other.mECSManager && mECSID == other.mECSID;
        }

    private:

        ECSManager* mECSManager = nullptr;
        ECSID mECSID = std::numeric_limits<ECSID>::max();
    };

    constexpr ECSHandle ToHandle(ECSOwningHandle& handle)
    {
        return ECSHandle(handle.GetManager(), handle.GetID());
    }

}