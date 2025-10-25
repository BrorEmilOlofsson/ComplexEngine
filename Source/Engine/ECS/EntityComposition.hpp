#pragma once
#include "Engine/ECS/ECS.hpp"
#include <filesystem>

namespace Simple
{

	class EntityComposition final
	{
	public:

		explicit EntityComposition(const ECSRegistry& ecsRegistry);

		[[nodiscard]] constexpr ECS& GetECS() noexcept;
		[[nodiscard]] constexpr const ECS& GetECS() const noexcept;

		[[nodiscard]] EntityID GetRootEntity() const noexcept;

		[[nodiscard]] const std::filesystem::path& GetPath() const noexcept;

		[[nodiscard]] bool HasChanged() const noexcept
		{
			return mHasChanged;
		}

		void SetHasChanged(const bool hasChanged) noexcept
		{
			mHasChanged = hasChanged;
		}

		void SetPath(const std::filesystem::path& path)
		{
			mPath = path;
		}

	private:

		ECS mECS;
		EntityID mRootEntityID = InvalidEntityID;
		
		std::filesystem::path mPath;
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