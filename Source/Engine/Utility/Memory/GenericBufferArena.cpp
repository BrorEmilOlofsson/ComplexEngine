#include "Engine/Precompiled/EnginePch.hpp"
#include "GenericBufferArena.hpp"

namespace CLX
{

	MemoryObject::MemoryObject(void* const dataPtr, const DestroyFunction destroyFunction, CopyConstructFunction copyConstructFunction, [[maybe_unused]] const std::type_index type)
		: mDataPtr(dataPtr)
        , mRelease(destroyFunction)
        , mCopyConstruct(copyConstructFunction)
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
		, mCopyConstruct(other.mCopyConstruct)
#ifdef _DEBUG
		, mType(other.mType)
#endif
	{
		mCopyConstruct(dataPtr, other.mDataPtr);
	}

	MemoryObject::MemoryObject(MemoryObject&& other) noexcept
		: mDataPtr(std::exchange(other.mDataPtr, nullptr))
		, mRelease(std::exchange(other.mRelease, nullptr))
		, mCopyConstruct(std::exchange(other.mCopyConstruct, nullptr))
#ifdef _DEBUG
		, mType(other.mType)
#endif
	{}
}