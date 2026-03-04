#pragma once
#include <vector>
#include <type_traits>

namespace Simple
{
	struct DynamicMemoryArenaHandle final
	{
		size_t id = std::numeric_limits<size_t>::max();

		[[nodiscard]] explicit operator size_t() const
		{
			return id;
		}
	};

	class DynamicMemoryArena final
	{
	public:

		template<typename, typename...>
		class FunctionPtrWrapper;

		template<typename Tag, typename Ret, typename... Args>
		class FunctionPtrWrapper<Ret(Args...), Tag> final
		{
			using FunctionType = Ret(Args...);
		public:

			constexpr FunctionPtrWrapper(FunctionType* functionPtr)
				: mFunctionPtr(functionPtr)
			{
			}

			constexpr Ret operator()(Args... aArgs) const
			{
				return mFunctionPtr(std::forward<Args>(aArgs)...);
			}

			[[nodiscard]] explicit operator bool() const
			{
				return mFunctionPtr != nullptr;
			}

		private:

			FunctionType* mFunctionPtr = nullptr;
		};

		using InplaceAllocateFunction = FunctionPtrWrapper<void(void*, const void*), struct InplaceFTag>;
		using DestructFunction = FunctionPtrWrapper<void(void*), struct DestructFTag>;
		using CopyFunction = FunctionPtrWrapper<void(void*, const void*), struct CopyFTag>;

	private:

		template<typename T>
		[[nodiscard]] constexpr InplaceAllocateFunction CreateInplaceAllocateFunction()
		{
			return [](void* aPtr, const void* aDefaultValuePtr) -> void
				{
					if (aDefaultValuePtr)
					{
						const T& defaultValue = *reinterpret_cast<const T*>(aDefaultValuePtr);
						new(aPtr)T(defaultValue);
					}
					else
					{
						new(aPtr)T();
					}
				};
		}

		template<typename T>
		[[nodiscard]] constexpr DestructFunction CreateDestructFunction()
		{
			return [](void* aPtr) -> void
				{
					T& value = *reinterpret_cast<T*>(aPtr);
					value.~T();
				};
		}

		template<typename T>
		[[nodiscard]] constexpr CopyFunction CreateCopyFunction()
		{
			return [](void* aDestination, const void* aSource) -> void
				{
					const T& source = *reinterpret_cast<const T*>(aSource);
					if constexpr (std::is_trivially_copyable_v<T>)
					{
						T& dest = *reinterpret_cast<T*>(aDestination);
						std::memcpy(&dest, &source, sizeof(T));
					}
					else
					{
						new(aDestination)T(source);
					}
				};
		}

		struct MemoryObject
		{
			DynamicMemoryArenaHandle handle;
			DestructFunction destroy = nullptr;
			CopyFunction copy = nullptr;
#ifdef _DEBUG
			const std::type_info* typeInfo = nullptr;
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

		[[nodiscard]] DynamicMemoryArenaHandle AllocateUnsafe(size_t size, InplaceAllocateFunction inplaceAllocateFunction, DestructFunction destructFunction, CopyFunction copyFunction);

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

		void AllocateSize(size_t aSize);
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