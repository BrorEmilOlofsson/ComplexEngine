#include "Utility/Precompiled/UtilityPch.hpp"
#include "DynamicMemoryArena.hpp"

namespace Simple
{

	DynamicMemoryArena::DynamicMemoryArena(const size_t aDefaultCapacity)
		: mStartPtr(nullptr)
		, mCapacity(aDefaultCapacity)
		, mSize(0)
	{
		if (aDefaultCapacity == 0)
		{
			return;
		}
		mStartPtr = new std::byte[aDefaultCapacity];
	}

	DynamicMemoryArena::~DynamicMemoryArena()
	{
		if (mStartPtr == nullptr)
		{
			return;
		}

		for (size_t i = 0; i < mObjects.size(); ++i)
		{
			MemoryObject& obj = mObjects[i];
			obj.destroy(MemoryAt(obj.handle));
		}

		delete[] mStartPtr;
	}

	DynamicMemoryArena::DynamicMemoryArena(const DynamicMemoryArena& aOther)
	{
		mStartPtr = new std::byte[aOther.mCapacity];
		mCapacity = aOther.mCapacity;
		mSize = aOther.mSize;

		for (const MemoryObject& memoryObject : aOther.mObjects)
		{
			std::byte* newAddress = &mStartPtr[memoryObject.handle.id];
			const std::byte* oldAddress = aOther.MemoryAt(memoryObject.handle);
			memoryObject.copy(newAddress, oldAddress);

			mObjects.push_back(memoryObject);
		}
	}

	DynamicMemoryArena::DynamicMemoryArena(DynamicMemoryArena&& aOther) noexcept
		: mStartPtr(std::exchange(aOther.mStartPtr, nullptr))
		, mCapacity(std::exchange(aOther.mCapacity, 0))
		, mSize(std::exchange(aOther.mSize, 0))
		, mObjects(std::move(aOther.mObjects))
	{
	}

	DynamicMemoryArena& DynamicMemoryArena::operator=(const DynamicMemoryArena& aOther)
	{
		for (const MemoryObject& memoryObject : mObjects)
		{
			memoryObject.destroy(MemoryAt(memoryObject.handle));
		}
		delete[] mStartPtr;

		mStartPtr = new std::byte[aOther.mCapacity];
		mCapacity = aOther.mCapacity;
		mSize = aOther.mSize;

		for (const MemoryObject& memoryObject : aOther.mObjects)
		{
			std::byte* newAddress = &mStartPtr[memoryObject.handle.id];
			const std::byte* oldAddress = aOther.MemoryAt(memoryObject.handle);
			memoryObject.copy(newAddress, oldAddress);

			mObjects.push_back(memoryObject);
		}

		return *this;
	}

	DynamicMemoryArena& DynamicMemoryArena::operator=(DynamicMemoryArena&& aOther) noexcept
	{
		mStartPtr = aOther.mStartPtr;
		mCapacity = aOther.mCapacity;
		mSize = aOther.mSize;

		mObjects = std::move(aOther.mObjects);

		aOther.mStartPtr = nullptr;
		aOther.mCapacity = 0;
		aOther.mSize = 0;

		return *this;
	}

	DynamicMemoryArenaHandle DynamicMemoryArena::AllocateUnsafe(const size_t size, InplaceAllocateFunction inplaceAllocateFunction, DestructFunction destructorFunction, CopyFunction copyFunction)
	{
		assert(inplaceAllocateFunction);
		assert(destructorFunction);
		assert(copyFunction);

		AllocateSize(size);

		DynamicMemoryArenaHandle memoryHandle{ GetSize() };
		mSize += size;
		inplaceAllocateFunction(MemoryAt(memoryHandle), nullptr);

		mObjects.push_back(
			MemoryObject
			{
				.handle = memoryHandle,
				.destroy = destructorFunction,
				.copy = copyFunction,
#ifdef _DEBUG
				.typeInfo = nullptr
#endif
			}
		);

		return memoryHandle;
	}

	size_t DynamicMemoryArena::GetCapacity() const
	{
		return mCapacity;
	}

	size_t DynamicMemoryArena::GetSize() const
	{
		return mSize;
	}

	size_t DynamicMemoryArena::GetSizeLeft() const
	{
		return mCapacity - mSize;
	}

	void DynamicMemoryArena::AllocateSize(const size_t aSize)
	{
		if (mStartPtr == nullptr)
		{
			*this = DynamicMemoryArena(aSize * 2);
		}
		while (aSize > GetSizeLeft())
		{
			Reallocate();
		}
	}

	void DynamicMemoryArena::Reallocate()
	{
		const std::byte* oldStart = mStartPtr;
		const size_t oldCapacity = GetCapacity();
		const size_t newCapacity = oldCapacity * 2;
		const size_t size = GetSize();

		std::byte* newStart = new std::byte[newCapacity];
		for (const MemoryObject& memoryObject : mObjects)
		{
			std::byte* newAddress = &newStart[memoryObject.handle.id];
			const std::byte* oldAddress = MemoryAt(memoryObject.handle);
			memoryObject.copy(newAddress, oldAddress);
		}
		delete[] oldStart;

		mStartPtr = newStart;
		mCapacity = newCapacity;
		mSize = size;
	}

	void DynamicMemoryArena::Clear()
	{
		if (mStartPtr == nullptr)
		{
			return;
		}

		for (size_t i = 0; i < mObjects.size(); ++i)
		{
			const MemoryObject& obj = mObjects[i];
			obj.destroy(MemoryAt(obj.handle));
		}

		delete[] mStartPtr;

		mStartPtr = nullptr;
		mCapacity = 0;
		mSize = 0;

		mObjects.clear();
	}
}