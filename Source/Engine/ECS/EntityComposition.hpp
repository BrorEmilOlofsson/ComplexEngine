#pragma once
#include "Engine/ECS/ECSManager.hpp"
#include "Engine/ECS/ECSHandle.hpp"

namespace CLX
{

	class EntityComposition final
	{
	public:

		explicit EntityComposition(ECSManager& ecsManager, ECSID ecsID);

		[[nodiscard]] constexpr ECSHandle GetECSHandle() noexcept;
		[[nodiscard]] ECS& GetECS() noexcept;
		[[nodiscard]] const ECS& GetECS() const noexcept;

		[[nodiscard]] EntityID GetRootEntity() const noexcept;

		[[nodiscard]] bool HasChanged() const noexcept
		{
			return mHasChanged;
		}

		void SetHasChanged(const bool hasChanged) noexcept
		{
			mHasChanged = hasChanged;
		}

	private:

		ECSOwningHandle mECSHandle;
		EntityID mRootEntityID = InvalidEntityID;

		bool mHasChanged = false;
	};

	constexpr ECSHandle EntityComposition::GetECSHandle() noexcept
	{
		return ECSHandle(mECSHandle.GetManager(), mECSHandle.GetID());
    }

	inline ECS& EntityComposition::GetECS() noexcept
	{
		return mECSHandle.Get();
	}

	inline  const ECS& EntityComposition::GetECS() const noexcept
	{
		return mECSHandle.Get();
	}

}