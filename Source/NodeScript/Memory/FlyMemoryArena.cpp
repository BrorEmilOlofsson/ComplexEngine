#include "FlyMemoryArena.hpp"
#include "../DataType/FlyDataTypeManager.hpp"
#include "../Internal/FlyInternal.hpp"
#include <cassert>

namespace FLY_NAMESPACE
{

	MemoryPool::MemoryPool(const size_t defaultCapacity)
		: mStartPtr(nullptr)
		, mCapacity(defaultCapacity)
		, mSize(0)
	{
		if (defaultCapacity == 0)
		{
			return;
		}
		mStartPtr = new Byte[defaultCapacity];
	}

	MemoryPool::~MemoryPool()
	{
		if (mStartPtr == nullptr)
		{
			return;
		}

		for (int i = 0; i < mObjects.size(); ++i)
		{
			MemoryObject& obj = mObjects[i];
			obj.mDestroy(MemoryAt(obj.mID));
		}

		delete[] mStartPtr;
	}

	MemoryPool::MemoryPool(const MemoryPool& other)
	{
		mStartPtr = new Byte[other.mCapacity];
		mCapacity = other.mCapacity;
		mSize = other.mSize;

		for (const MemoryObject& memoryObject : other.mObjects)
		{
			Byte* newAddress = &mStartPtr[memoryObject.mID];
			const Byte* oldAddress = other.MemoryAt(memoryObject.mID);
			memoryObject.mCopy(newAddress, oldAddress);

			mObjects.push_back(memoryObject);
		}
	}

	MemoryPool::MemoryPool(MemoryPool&& other) noexcept
		: mStartPtr(other.mStartPtr)
		, mCapacity(other.mCapacity)
		, mSize(other.mSize)
		, mObjects(std::move(other.mObjects))
	{
		other.mStartPtr = nullptr;
		other.mCapacity = 0;
		other.mSize = 0;
	}

	MemoryPool& MemoryPool::operator=(const MemoryPool& other)
	{
		for (const MemoryObject& memoryObject : mObjects)
		{
			memoryObject.mDestroy(MemoryAt(memoryObject.mID));
		}
		delete[] mStartPtr;

		mStartPtr = new Byte[other.mCapacity];
		mCapacity = other.mCapacity;
		mSize = other.mSize;

		for (const MemoryObject& memoryObject : other.mObjects)
		{
			Byte* newAddress = &mStartPtr[memoryObject.mID];
			const Byte* oldAddress = other.MemoryAt(memoryObject.mID);
			memoryObject.mCopy(newAddress, oldAddress);

			mObjects.push_back(memoryObject);
		}

		return *this;
	}

	MemoryPool& MemoryPool::operator=(MemoryPool&& aOther) noexcept
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

	size_t MemoryPool::GetCapacity() const
	{
		return mCapacity;
	}

	size_t MemoryPool::GetSize() const
	{
		return mSize;
	}

	size_t MemoryPool::GetSizeLeft() const
	{
		return mCapacity - mSize;
	}

	void MemoryPool::Reallocate()
	{
		const Byte* oldStart = mStartPtr;
		const size_t oldCapacity = GetCapacity();
		const size_t newCapacity = oldCapacity * 2;
		const size_t size = GetSize();

		Byte* newStart = new Byte[newCapacity];
		for (const MemoryObject& memoryObject : mObjects)
		{
			Byte* newAddress = &newStart[memoryObject.mID];
			const Byte* oldAddress = MemoryAt(memoryObject.mID);
			memoryObject.mCopy(newAddress, oldAddress);
		}
		delete[] oldStart;

		mStartPtr = newStart;
		mCapacity = newCapacity;
		mSize = size;
	}

	void MemoryPool::Clear()
	{
		if (mStartPtr == nullptr)
		{
			return;
		}

		for (size_t i = 0; i < mObjects.size(); ++i)
		{
			const MemoryObject& obj = mObjects[i];
			obj.mDestroy(MemoryAt(obj.mID));
		}

		delete[] mStartPtr;

		mStartPtr = nullptr;
		mCapacity = 0;
		mSize = 0;

		mObjects.clear();
	}

	static DataType* GetDataType(DataTypeID aDataTypeID)
	{
		return Internal::GetDataTypeManager().Find(aDataTypeID);
	}

	MemoryObject::MemoryObject(void* const dataPtr, const DataTypeID dataTypeID)
		: mDataPtr(dataPtr)
	{
		DataType* dataType = GetDataType(dataTypeID);
		assert(dataType);

		mRelease = dataType->GetInterface().GetReleaseFunction().GetPtr();
		mCopy = dataType->GetInterface().GetCopyFunction().GetPtr();

#ifdef FLY_DEBUG
		mTypeInfo = dataType->GetTypeInfo();
#endif
	}

	MemoryObject::~MemoryObject()
	{
		if (mRelease)
		{
			mRelease(mDataPtr);
		}
	}

	MemoryObject::MemoryObject(const MemoryObject& other, void* const dataPtr)
		: mDataPtr(dataPtr)
		, mRelease(other.mRelease)
		, mCopy(other.mCopy)
	{
		mCopy(dataPtr, other.mDataPtr);

#ifdef FLY_DEBUG
		mTypeInfo = other.mTypeInfo;
#endif
	}

	MemoryObject::MemoryObject(MemoryObject&& other) noexcept
		: mDataPtr(std::exchange(other.mDataPtr, nullptr))
		, mRelease(std::exchange(other.mRelease, nullptr))
		, mCopy(std::exchange(other.mCopy, nullptr))
#ifdef FLY_DEBUG
		, mTypeInfo(std::exchange(other.mTypeInfo, nullptr))
#endif
	{
	}
}