#include "Engine/Precompiled/EnginePch.hpp"
#include "DynamicMemoryArena.hpp"

namespace CLX
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

	DynamicMemoryArena::DynamicMemoryArena(const DynamicMemoryArena& other)
	{
		mStartPtr = new std::byte[other.mCapacity];
		mCapacity = other.mCapacity;
		mSize = other.mSize;

		for (const MemoryObject& memoryObject : other.mObjects)
		{
			std::byte* newAddress = &mStartPtr[memoryObject.handle.id];
			const std::byte* oldAddress = other.MemoryAt(memoryObject.handle);
			memoryObject.copyAssign(newAddress, oldAddress);

			mObjects.push_back(memoryObject);
		}
	}

	DynamicMemoryArena::DynamicMemoryArena(DynamicMemoryArena&& other) noexcept
		: mStartPtr(std::exchange(other.mStartPtr, nullptr))
		, mCapacity(std::exchange(other.mCapacity, 0))
		, mSize(std::exchange(other.mSize, 0))
		, mObjects(std::move(other.mObjects))
	{
	}

	DynamicMemoryArena& DynamicMemoryArena::operator=(const DynamicMemoryArena& other)
	{
		for (const MemoryObject& memoryObject : mObjects)
		{
			memoryObject.destroy(MemoryAt(memoryObject.handle));
		}
		delete[] mStartPtr;

		mStartPtr = new std::byte[other.mCapacity];
		mCapacity = other.mCapacity;
		mSize = other.mSize;

		for (const MemoryObject& memoryObject : other.mObjects)
		{
			std::byte* newAddress = &mStartPtr[memoryObject.handle.id];
			const std::byte* oldAddress = other.MemoryAt(memoryObject.handle);
			memoryObject.copyAssign(newAddress, oldAddress);

			mObjects.push_back(memoryObject);
		}

		return *this;
	}

	DynamicMemoryArena& DynamicMemoryArena::operator=(DynamicMemoryArena&& other) noexcept
	{
		mStartPtr = other.mStartPtr;
		mCapacity = other.mCapacity;
		mSize = other.mSize;

		mObjects = std::move(other.mObjects);

		other.mStartPtr = nullptr;
		other.mCapacity = 0;
		other.mSize = 0;

		return *this;
	}

	DynamicMemoryArenaHandle DynamicMemoryArena::AllocateUnsafe(const size_t size, InplaceConstructFunction inplaceAllocateFunction, DestroyFunction destructorFunction, CopyAssignFunction copyAssignFunction, [[maybe_unused]] std::type_index type)
	{
		assert(inplaceAllocateFunction);
		assert(destructorFunction);
		assert(copyAssignFunction);

		AllocateSize(size);

		DynamicMemoryArenaHandle memoryHandle{ GetSize() };
		mSize += size;
		inplaceAllocateFunction(MemoryAt(memoryHandle), nullptr);

		mObjects.push_back(
			MemoryObject
			{
				.handle = memoryHandle,
				.destroy = destructorFunction,
				.copyAssign = copyAssignFunction,
#ifdef _DEBUG
				.type = type
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

	void DynamicMemoryArena::AllocateSize(const size_t size)
	{
		if (mStartPtr == nullptr)
		{
			*this = DynamicMemoryArena(size * 2);
		}
		while (size > GetSizeLeft())
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
			memoryObject.copyAssign(newAddress, oldAddress);
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