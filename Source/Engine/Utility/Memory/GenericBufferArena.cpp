#include "Engine/Precompiled/EnginePch.hpp"
#include "GenericBufferArena.hpp"

namespace CLX
{

	MemoryObject::MemoryObject(void* const dataPtr, const DestroyFunction destroyFunction, CopyFunction copyFunction, [[maybe_unused]] const std::type_index type)
		: mDataPtr(dataPtr)
        , mRelease(destroyFunction)
        , mCopy(copyFunction)
#ifdef _DEBUG
		, mType(type)
#endif
	{
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
#ifdef _DEBUG
		, mType(other.mType)
#endif
	{
		mCopy(dataPtr, other.mDataPtr);
	}

	MemoryObject::MemoryObject(MemoryObject&& other) noexcept
		: mDataPtr(std::exchange(other.mDataPtr, nullptr))
		, mRelease(std::exchange(other.mRelease, nullptr))
		, mCopy(std::exchange(other.mCopy, nullptr))
#ifdef _DEBUG
		, mType(other.mType)
#endif
	{}
}