#pragma once
#include "Engine/ECS/ECS.hpp"

namespace Simple
{

	class EntityComposition final
	{
	public:

		explicit EntityComposition(const ECSRegistry& ecsRegistry);

		[[nodiscard]] constexpr ECS& GetECS() noexcept;
		[[nodiscard]] constexpr const ECS& GetECS() const noexcept;

	private:

		ECS mECS;
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