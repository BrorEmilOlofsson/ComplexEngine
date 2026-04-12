#pragma once
#include "../FlyDefines.hpp"
#include "FlyHeapObject.hpp"
#include <vector>
#include <array>
#include <cassert>

namespace FLY_NAMESPACE
{

    using Byte = unsigned char;
    using MemoryPoolID = size_t;

    class MemoryPool final
    {

        using ConstructorInterface = void(*)(void*);
        using DestructorInterface = void(*)(void*);
        using CopyInterface = void(*)(void*, const void*);

        template<typename T>
        inline ConstructorInterface CreateConstructorInterface()
        {
            return [](void* aPtr) -> void
                {
                    new(aPtr)T();
                };
        }

        template<typename T>
        inline DestructorInterface CreateDestructorInterface()
        {
            return [](void* aPtr) -> void
                {
                    T& ptr = *reinterpret_cast<T*>(aPtr);
                    ptr.~T();
                };
        }

        template<typename T>
        inline CopyInterface CreateCopyInterface()
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
            const MemoryPoolID mID;
            const DestructorInterface mDestroy;
            const CopyInterface mCopy;
#ifdef FLY_DEBUG
            const std::type_info* mTypeInfo;
#endif
        };
    public:

        MemoryPool(size_t aDefaultCapacity = 0);
        ~MemoryPool();

        MemoryPool(const MemoryPool&);
        MemoryPool(MemoryPool&&) noexcept;
        MemoryPool& operator=(const MemoryPool&);
        MemoryPool& operator=(MemoryPool&&) noexcept;

        template<typename T, typename... Args>
        MemoryPoolID Allocate(Args&&... args);

        template<typename T>
        [[nodiscard]] T& At(MemoryPoolID id);

        template<typename T>
        [[nodiscard]] const T& At(MemoryPoolID id) const;

        [[nodiscard]] size_t GetCapacity() const;
        [[nodiscard]] size_t GetSize() const;
        [[nodiscard]] size_t GetSizeLeft() const;

        [[nodiscard]] Byte* MemoryAt(MemoryPoolID id);
        [[nodiscard]] const Byte* MemoryAt(MemoryPoolID id) const;

        void Clear();

    private:
        void Reallocate();
    private:
        Byte* mStartPtr;
        size_t mCapacity;
        size_t mSize;

        std::vector<MemoryObject> mObjects;
    };

    template<typename T, typename... Args>
    inline MemoryPoolID MemoryPool::Allocate(Args&&... args)
    {
        constexpr size_t newAllocSize = sizeof(T);
        if (mStartPtr == nullptr)
        {
            *this = MemoryPool(newAllocSize * 2);
        }
        while (newAllocSize > GetSizeLeft())
        {
            Reallocate();
        }

        const MemoryPoolID id = GetSize();
        Byte* currentMemory = mStartPtr + id;
        new(currentMemory)T(std::forward<Args>(args)...);
        mSize += newAllocSize;
        MemoryObject memoryObject
        {
            .mID = id,
            .mDestroy = CreateDestructorInterface<T>(),
            .mCopy = CreateCopyInterface<T>(),
#ifdef FLY_DEBUG
            .mTypeInfo = &typeid(T)
#endif
        };
        mObjects.push_back(memoryObject);
        return id;
    }

    template<typename T>
    inline T& MemoryPool::At(MemoryPoolID id)
    {
        Byte* byte = MemoryAt(id);
        return *reinterpret_cast<T*>(byte);
    }

    template<typename T>
    inline const T& MemoryPool::At(MemoryPoolID id) const
    {
        const Byte* byte = MemoryAt(id);
        return *reinterpret_cast<const T*>(byte);
    }

    inline Byte* MemoryPool::MemoryAt(MemoryPoolID id)
    {
        return mStartPtr + id;
    }

    inline const Byte* MemoryPool::MemoryAt(MemoryPoolID id) const
    {
        return mStartPtr + id;
    }


    using ReleaseFunction = void(*)(void*);
    using CopyFunction = void(*)(void*, const void*);

    template<typename T>
    ReleaseFunction CreateReleaseFunction()
    {
        return [](void* aPtr) -> void
            {
                ((T*)aPtr)->~T();
            };
    }

    template<typename T>
    CopyFunction CreateCopyFunction()
    {
        return [](void* aDest, const void* aSource) -> void
            {
                const T& source = *(const T*)aSource;
                new(aDest)T(source);
            };
    }

    struct MemoryObject final
    {
        template<typename T>
        explicit MemoryObject(T* value)
            : mDataPtr(value)
            , mRelease(CreateReleaseFunction<T>())
            , mCopy(CreateCopyFunction<T>())
        {
#ifdef FLY_DEBUG
            mTypeInfo = &typeid(T);
#endif
        }

        MemoryObject(void* dataPtr, DataTypeID dataTypeID);

        ~MemoryObject();

        MemoryObject(const MemoryObject&) = delete;

        MemoryObject(const MemoryObject& other, void* memory);

        MemoryObject(MemoryObject&&) noexcept;
        MemoryObject& operator=(const MemoryObject&) = delete;
        MemoryObject& operator=(MemoryObject&&) noexcept = default;

    public:
        void* mDataPtr = nullptr;
        ReleaseFunction mRelease = nullptr;
        CopyFunction mCopy = nullptr;
#ifdef FLY_DEBUG
        const std::type_info* mTypeInfo = nullptr;
#endif
    };

    [[nodiscard]] constexpr std::size_t GetPointerDiff(const void* const ptr1, const void* const ptr2)
    {
        return static_cast<const Byte*>(ptr1) - static_cast<const Byte*>(ptr2);
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
        {
        }

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

        void RegisterMemoryObject(void* dataPtr, DataTypeID dataTypeID)
        {
            assert(GetPointerDiff(dataPtr, GetDataPtr()) < Capacity);

            mMemoryObjects.emplace_back(MemoryObject(dataPtr, dataTypeID));
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

        static constexpr std::size_t AlignUp(std::size_t n, std::size_t a)
        {
            return (n + (a - 1)) & ~(a - 1);
        }

    private:

        std::array<std::byte, Capacity> mBuffer;
        size_t mSize = 0;

        std::vector<MemoryObject> mMemoryObjects;
    };



    template<size_t BufferCapacity>
    class MemoryArena final
    {
        using MemoryBuffer = MemoryBuffer<BufferCapacity>;

    public:

        MemoryArena();
        ~MemoryArena() = default;
        MemoryArena(const MemoryArena& other);
        MemoryArena(MemoryArena&&) noexcept = default;
        MemoryArena& operator=(const MemoryArena& other);
        MemoryArena& operator=(MemoryArena&&) noexcept = default;


        template<MemSizeLessEqual<BufferCapacity> T, typename... Args>
        T& Allocate(Args&&... args);

        // Returns a raw pointer to a memory block at the start of the allocated size.
        void* AllocateSizeAligned(std::size_t size, std::size_t alignment);

        void RegisterMemoryObject(void* dataPtr, DataTypeID dataTypeID);

        [[nodiscard]] void* GetRenewedPointer(const void* dataPtr, const MemoryArena& previous) const;

        template<std::integral T>
        [[nodiscard]] T GetID(const void* ptr) const;

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
    inline MemoryArena<BufferCapacity>::MemoryArena()
        : mCurrentBufferIndex(0)
    {
        AllocateNewBuffer();
    }

    template<size_t BufferCapacity>
    inline MemoryArena<BufferCapacity>::MemoryArena(const MemoryArena& other)
        : mCurrentBufferIndex(other.mCurrentBufferIndex)
        , mMemoryBuffers(other.mMemoryBuffers)
    {
    }

    template<size_t BufferCapacity>
    inline MemoryArena<BufferCapacity>& MemoryArena<BufferCapacity>::operator=(const MemoryArena<BufferCapacity>& other)
    {
        mMemoryBuffers = other.mMemoryBuffers;
        mCurrentBufferIndex = other.mCurrentBufferIndex;
        return *this;
    }

    template<size_t BufferCapacity>
    template<MemSizeLessEqual<BufferCapacity> T, typename ...Args>
    inline T& MemoryArena<BufferCapacity>::Allocate(Args&&... args)
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
    inline void* MemoryArena<BufferCapacity>::AllocateSizeAligned(const std::size_t size, const std::size_t alignment)
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
    inline void MemoryArena<BufferCapacity>::RegisterMemoryObject(void* const dataPtr, const DataTypeID dataTypeID)
    {
        MemoryBuffer& buffer = GetCurrentBuffer();
        buffer.RegisterMemoryObject(dataPtr, dataTypeID);
    }

    template<size_t BufferCapacity>
    inline void* MemoryArena<BufferCapacity>::GetRenewedPointer(const void* dataPtr, const MemoryArena& previous) const
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
    template<std::integral T>
    inline T MemoryArena<BufferCapacity>::GetID(const void* dataPtr) const
    {
        for (size_t i = 0; i < mMemoryBuffers.size(); ++i)
        {
            const auto& buffer = mMemoryBuffers[i];
            const size_t ptrDiff = GetPointerDiff(dataPtr, buffer->GetDataPtr());
            if (ptrDiff < BufferCapacity)
            {
                return static_cast<T>(i * BufferCapacity + (static_cast<size_t>(mMemoryBuffers[i]->GetDataPtr()) + ptrDiff));
            }
        }

        return InvalidID<T>();
    }

    template<size_t BufferCapacity>
    inline void MemoryArena<BufferCapacity>::Clear()
    {
        *this = MemoryArena();
    }

    template<size_t BufferCapacity>
    inline void MemoryArena<BufferCapacity>::AllocateNewBuffer()
    {
        mCurrentBufferIndex = mMemoryBuffers.size();
        mMemoryBuffers.emplace_back(HeapObject<MemoryBuffer>());
    }


    template<size_t BufferCapacity>
    inline MemoryBuffer<BufferCapacity>& MemoryArena<BufferCapacity>::GetCurrentBuffer()
    {
        return *mMemoryBuffers.back();
    }

    template<size_t BufferCapacity>
    inline MemoryBuffer<BufferCapacity>& MemoryArena<BufferCapacity>::GetMemoryBuffer(const void* const dataPtr)
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


    template<size_t Size>
    class TestStackAllocator
    {
    public:


        [[nodiscard]] void* allocate(const size_t numObjects)
        {

        }

    private:

        std::array<std::byte, Size> mBytes;
    };


}