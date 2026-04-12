#pragma once
#include <vector>
#include <typeindex>
#include <cassert>
#include <limits>

#include "Engine/Utility/FundamentalFunctionDefinitions.hpp"

namespace CLX
{
	struct DynamicMemoryArenaHandle final
	{
		size_t id = std::numeric_limits<size_t>::max();

		[[nodiscard]] explicit operator size_t() const
		{
			return id;
		}
	};

	constexpr std::size_t AlignUp(std::size_t n, std::size_t a)
	{
		return (n + (a - 1)) & ~(a - 1);
	}

	class DynamicMemoryArena final
	{
	private:

		struct MemoryObject
		{
			DynamicMemoryArenaHandle handle;
			DestroyFunction destroy = nullptr;
			CopyFunction copy = nullptr;
#ifdef _DEBUG
			std::type_index type;
#endif
		};
	public:

		DynamicMemoryArena(size_t defaultCapacity = 0);
		~DynamicMemoryArena();

		DynamicMemoryArena(const DynamicMemoryArena&);
		DynamicMemoryArena(DynamicMemoryArena&&) noexcept;
		DynamicMemoryArena& operator=(const DynamicMemoryArena&);
		DynamicMemoryArena& operator=(DynamicMemoryArena&&) noexcept;

		template<typename T, typename... Args>
		[[nodiscard]] DynamicMemoryArenaHandle Allocate(Args&&... args);

		[[nodiscard]] DynamicMemoryArenaHandle AllocateUnsafe(size_t size, InplaceConstructFunction inplaceAllocateFunction, DestroyFunction destructFunction, CopyFunction copyFunction, std::type_index type);

		template<typename T>
		[[nodiscard]] T& At(DynamicMemoryArenaHandle handle);

		template<typename T>
		[[nodiscard]] const T& At(DynamicMemoryArenaHandle handle) const;

		[[nodiscard]] size_t GetCapacity() const;
		[[nodiscard]] size_t GetSize() const;
		[[nodiscard]] size_t GetSizeLeft() const;

		[[nodiscard]] std::byte* MemoryAt(DynamicMemoryArenaHandle handle);
		[[nodiscard]] const std::byte* MemoryAt(DynamicMemoryArenaHandle handle) const;

		void Clear();

	private:

		void AllocateSize(size_t size);
		void Reallocate();

	private:
		std::byte* mStartPtr = nullptr;
		size_t mCapacity = 0;
		size_t mSize = 0;

		std::vector<MemoryObject> mObjects;
	};

	template<typename T, typename... Args>
	inline DynamicMemoryArenaHandle DynamicMemoryArena::Allocate(Args&&... args)
	{
		const size_t allocSize = sizeof(T);
		AllocateSize(allocSize);

		const size_t currentSize = GetSize();
		const DynamicMemoryArenaHandle memoryHandle{ currentSize };
		std::byte* currentMemory = mStartPtr + currentSize;
		new(currentMemory)T(std::forward<Args>(args)...);
		mSize += allocSize;
		MemoryObject memoryObject
		{
			.handle = memoryHandle,
			.destroy = CreateDestructorInterface<T>(),
			.copy = CreateCopyInterface<T>(),
#ifdef _DEBUG
			.typeInfo = &typeid(T)
#endif
		};
		mObjects.push_back(memoryObject);
		return memoryHandle;
	}

	template<typename T>
	inline T& DynamicMemoryArena::At(DynamicMemoryArenaHandle handle)
	{
		std::byte* byte = MemoryAt(handle);
		return *reinterpret_cast<T*>(byte);
	}

	template<typename T>
	inline const T& DynamicMemoryArena::At(DynamicMemoryArenaHandle handle) const
	{
		const std::byte* byte = MemoryAt(handle);
		return *reinterpret_cast<const T*>(byte);
	}

	inline std::byte* DynamicMemoryArena::MemoryAt(DynamicMemoryArenaHandle handle)
	{
		assert(GetCapacity() > handle.id);
		return mStartPtr + handle.id;
	}

	inline const std::byte* DynamicMemoryArena::MemoryAt(DynamicMemoryArenaHandle handle) const
	{
		assert(GetCapacity() > handle.id);
		return mStartPtr + handle.id;
	}

}