#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include <array>
#include <typeindex>
#include <type_traits>
#include <exception>
#include <cassert>

#include "Engine/Utility/Memory/HeapObject.hpp"
#include "Engine/Utility/FundamentalFunctionDefinitions.hpp"

namespace CLX
{

    constexpr std::size_t AlignUp(std::size_t n, std::size_t a)
    {
        return (n + (a - 1)) & ~(a - 1);
    }

    struct MemoryObject final
    {
        template<typename T>
        explicit MemoryObject(T* value)
            : mDataPtr(value)
            , mRelease(CreateDestroyFunction<T>())
            , mCopy(CreateCopyFunction<T>())
        {
#ifdef FLY_DEBUG
            mTypeInfo = &typeid(T);
#endif
        }

        MemoryObject(void* dataPtr, const DestroyFunction destroyFunction, CopyFunction copyFunction, [[maybe_unused]] const std::type_index type);

        ~MemoryObject();

        MemoryObject(const MemoryObject&) = delete;

        MemoryObject(const MemoryObject& other, void* memory);

        MemoryObject(MemoryObject&&) noexcept;
        MemoryObject& operator=(const MemoryObject&) = delete;
        MemoryObject& operator=(MemoryObject&&) noexcept = default;

    public:
        void* mDataPtr = nullptr;
        DestroyFunction mRelease;
        CopyFunction mCopy;
#ifdef _DEBUG
        std::type_index mType;
#endif
    };

    [[nodiscard]] constexpr std::size_t GetPointerDiff(const void* const ptr1, const void* const ptr2)
    {
        return static_cast<const std::byte*>(ptr1) - static_cast<const std::byte*>(ptr2);
    }

    template<typename T, size_t Size>
    concept MemSizeLessEqual = sizeof(T) <= Size;

    template<size_t Capacity>
    class MemoryBuffer final
    {
    public:

        MemoryBuffer()
            : mBuffer{}
            , mSize(0)
        {
        }

        ~MemoryBuffer()
        {}

        MemoryBuffer(const MemoryBuffer& other)
            : mBuffer(other.mBuffer)
            , mSize(other.mSize)
        {
            mMemoryObjects.reserve(other.mMemoryObjects.size());
            for (const MemoryObject& memoryObject : other.mMemoryObjects)
            {
                const size_t ptrDiff = GetPointerDiff(memoryObject.mDataPtr, &other.mBuffer[0]);
                void* newMemory = &mBuffer[0] + ptrDiff;
                mMemoryObjects.emplace_back(memoryObject, newMemory);
            }
        }

        MemoryBuffer(MemoryBuffer&&) noexcept = default;

        MemoryBuffer& operator=(const MemoryBuffer& other)
        {
            mBuffer = other.mBuffer;
            mSize = other.mSize;

            mMemoryObjects.clear();

            for (const MemoryObject& memoryObject : other.mMemoryObjects)
            {
                const size_t ptrDiff = GetPointerDiff(memoryObject.mDataPtr, &other.mBuffer[0]);
                void* newMemory = &mBuffer[0] + ptrDiff;
                mMemoryObjects.emplace_back(memoryObject, newMemory);
            }

            return *this;
        }

        MemoryBuffer& operator=(MemoryBuffer&&) noexcept = default;

        template<MemSizeLessEqual<Capacity> T, typename... Args>
        T& Allocate(Args&&... args)
        {
            void* currentMemory = AllocateSizeAligned(sizeof(T), alignof(T));

            T* valuePtr = static_cast<T*>(currentMemory);

            std::construct_at(valuePtr, std::forward<Args>(args)...);

            if constexpr (!std::is_trivially_copyable_v<T>)
            {
                mMemoryObjects.emplace_back(valuePtr);
            }
            return *valuePtr;
        }

        void* AllocateSizeAligned(const std::size_t size, const std::size_t alignment)
        {
            // alignment must be power of two for the bit trick
            assert((alignment & (alignment - 1)) == 0);

            std::size_t alignedOffset = AlignUp(mSize, alignment);
            if (alignedOffset + size > Capacity)
            {
                throw std::bad_alloc();
            }

            void* p = mBuffer.data() + alignedOffset;
            mSize = alignedOffset + size;
            return p;
        }

        void RegisterMemoryObject(void* dataPtr, const DestroyFunction destroyFuncton, const CopyFunction copyFunction, std::type_index type)
        {
            assert(GetPointerDiff(dataPtr, GetDataPtr()) < Capacity);

            mMemoryObjects.emplace_back(MemoryObject(dataPtr, destroyFuncton, copyFunction, type));
        }

        [[nodiscard]] size_t GetSizeLeft() const
        {
            return Capacity - mSize;
        }

        [[nodiscard]] void* GetDataPtr()
        {
            return &mBuffer[0];
        }

        [[nodiscard]] const void* GetDataPtr() const
        {
            return &mBuffer[0];
        }

        [[nodiscard]] void* GetCurrentPtr()
        {
            return (&mBuffer[0]) + mSize;
        }

        [[nodiscard]] const void* GetCurrentPtr() const
        {
            return (&mBuffer[0]) + mSize;
        }

    private:

        std::array<std::byte, Capacity> mBuffer;
        size_t mSize = 0;

        std::vector<MemoryObject> mMemoryObjects;
    };



    template<size_t BufferCapacity>
    class GenericBufferArena final
    {
        using MemoryBuffer = MemoryBuffer<BufferCapacity>;

    public:

        GenericBufferArena();
        ~GenericBufferArena() = default;
        GenericBufferArena(const GenericBufferArena& other);
        GenericBufferArena(GenericBufferArena&&) noexcept = default;
        GenericBufferArena& operator=(const GenericBufferArena& other);
        GenericBufferArena& operator=(GenericBufferArena&&) noexcept = default;


        template<MemSizeLessEqual<BufferCapacity> T, typename... Args>
        T& Allocate(Args&&... args);

        // Returns a raw pointer to a memory block at the start of the allocated size.
        void* AllocateSizeAligned(std::size_t size, std::size_t alignment);

        void RegisterMemoryObject(void* dataPtr, const DestroyFunction destroyFuncton, const CopyFunction copyFunction, std::type_index type);

        [[nodiscard]] void* GetRenewedPointer(const void* dataPtr, const GenericBufferArena& previous) const;

        void Clear();

        [[nodiscard]] consteval std::size_t GetBufferCapacity() const
        {
            return BufferCapacity;
        }

    private:


        void AllocateNewBuffer();
        [[nodiscard]] MemoryBuffer& GetCurrentBuffer();
        [[nodiscard]] MemoryBuffer& GetMemoryBuffer(const void* dataPtr);

    private:

        std::vector<HeapObject<MemoryBuffer>> mMemoryBuffers;
        size_t mCurrentBufferIndex = 0;
    };

    template<size_t BufferCapacity>
    inline GenericBufferArena<BufferCapacity>::GenericBufferArena()
        : mCurrentBufferIndex(0)
    {
        AllocateNewBuffer();
    }

    template<size_t BufferCapacity>
    inline GenericBufferArena<BufferCapacity>::GenericBufferArena(const GenericBufferArena& other)
        : mCurrentBufferIndex(other.mCurrentBufferIndex)
        , mMemoryBuffers(other.mMemoryBuffers)
    {}

    template<size_t BufferCapacity>
    inline GenericBufferArena<BufferCapacity>& GenericBufferArena<BufferCapacity>::operator=(const GenericBufferArena<BufferCapacity>& other)
    {
        mMemoryBuffers = other.mMemoryBuffers;
        mCurrentBufferIndex = other.mCurrentBufferIndex;
        return *this;
    }

    template<size_t BufferCapacity>
    template<MemSizeLessEqual<BufferCapacity> T, typename ...Args>
    inline T& GenericBufferArena<BufferCapacity>::Allocate(Args&&... args)
    {
        constexpr size_t allocSize = sizeof(T);
        if (GetCurrentBuffer().GetSizeLeft() < allocSize)
        {
            AllocateNewBuffer();
        }

        MemoryBuffer& currentBuffer = GetCurrentBuffer();
        T& value = currentBuffer.Allocate<T>(std::forward<Args>(args)...);


        return value;
    }


    template<size_t BufferCapacity>
    inline void* GenericBufferArena<BufferCapacity>::AllocateSizeAligned(const std::size_t size, const std::size_t alignment)
    {
        if (size > BufferCapacity)
        {
            throw std::runtime_error("Trying to allocate size that is larger than the set buffer capacity.");
        }
        if (GetCurrentBuffer().GetSizeLeft() < size)
        {
            AllocateNewBuffer();
        }
        MemoryBuffer& buffer = GetCurrentBuffer();
        void* dataPtr = buffer.AllocateSizeAligned(size, alignment);
        return dataPtr;
    }

    template<size_t BufferCapacity>
    inline void GenericBufferArena<BufferCapacity>::RegisterMemoryObject(void* const dataPtr, const DestroyFunction destroyFunction, const CopyFunction copyFunction, std::type_index type)
    {
        MemoryBuffer& buffer = GetCurrentBuffer();
        buffer.RegisterMemoryObject(dataPtr, destroyFunction, copyFunction, type);
    }

    template<size_t BufferCapacity>
    inline void* GenericBufferArena<BufferCapacity>::GetRenewedPointer(const void* dataPtr, const GenericBufferArena& previous) const
    {
        for (size_t i = 0; i < previous.mMemoryBuffers.size(); ++i)
        {
            const auto& buffer = previous.mMemoryBuffers[i];
            const size_t ptrDiff = GetPointerDiff(dataPtr, buffer->GetDataPtr());
            if (ptrDiff < BufferCapacity)
            {
                return reinterpret_cast<void*>(reinterpret_cast<size_t>(mMemoryBuffers[i]->GetDataPtr()) + ptrDiff);
            }
        }
        assert(false);
        return nullptr;
    }

    template<size_t BufferCapacity>
    inline void GenericBufferArena<BufferCapacity>::Clear()
    {
        *this = GenericBufferArena();
    }

    template<size_t BufferCapacity>
    inline void GenericBufferArena<BufferCapacity>::AllocateNewBuffer()
    {
        mCurrentBufferIndex = mMemoryBuffers.size();
        mMemoryBuffers.emplace_back(HeapObject<MemoryBuffer>());
    }


    template<size_t BufferCapacity>
    inline MemoryBuffer<BufferCapacity>& GenericBufferArena<BufferCapacity>::GetCurrentBuffer()
    {
        return *mMemoryBuffers.back();
    }

    template<size_t BufferCapacity>
    inline MemoryBuffer<BufferCapacity>& GenericBufferArena<BufferCapacity>::GetMemoryBuffer(const void* const dataPtr)
    {
        for (size_t i = 0; i < mMemoryBuffers.size(); ++i)
        {
            const auto& buffer = mMemoryBuffers[i];
            const size_t ptrDiff = GetPointerDiff(dataPtr, buffer->GetDataPtr());
            if (ptrDiff < BufferCapacity)
            {
                return *buffer;
            }
        }

        assert(false);

        return GetCurrentBuffer();

    }
}