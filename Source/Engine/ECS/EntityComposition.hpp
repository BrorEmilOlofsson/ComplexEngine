#pragma once
#include "Engine/ECS/ECS.hpp"

namespace CLX
{

	class EntityComposition final
	{
	public:

		explicit EntityComposition(const ECSRegistry& ecsRegistry);

		[[nodiscard]] constexpr ECS& GetECS() noexcept;
		[[nodiscard]] constexpr const ECS& GetECS() const noexcept;

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

		ECS mECS;
		EntityID mRootEntityID = InvalidEntityID;

		bool mHasChanged = false;
	};

	constexpr ECS& EntityComposition::GetECS() noexcept
	{
		return mECS;
	}

	constexpr const ECS& EntityComposition::GetECS() const noexcept
	{
		return mECS;
	}

}