#pragma once
#include <type_traits>
#include <memory>
#include <utility>

namespace CLX
{

    template<typename T>
    class HeapObject final
    {
    public:

        HeapObject()
            : mUniquePtr(std::make_unique<T>())
        {
        }


        template<typename... Args> requires std::is_trivially_constructible_v<T, Args...>
        HeapObject(Args&&... args)
            : mUniquePtr(std::make_unique<T>(std::forward<Args>(args)...))
        {}

        HeapObject(const HeapObject& other)
            : mUniquePtr(other.Get() != nullptr ? std::make_unique<T>(*other) : std::unique_ptr<T>())
        {
        }

        HeapObject(HeapObject&&) noexcept = default;

        HeapObject& operator=(const HeapObject& other)
        {
            mUniquePtr = other.Get() != nullptr ? std::make_unique<T>(*other) : std::unique_ptr<T>();

            return *this;
        }

        HeapObject& operator=(HeapObject&&) noexcept = default;

        [[nodiscard]] T* Get()
        {
            return mUniquePtr.get();
        }

        [[nodiscard]] const T* Get() const
        {
            return mUniquePtr.get();
        }

        [[nodiscard]] T* operator->()
        {
            return Get();
        }

        [[nodiscard]] const T* operator->() const
        {
            return Get();
        }

        [[nodiscard]] T& operator*()
        {
            return *mUniquePtr;
        }

        [[nodiscard]] const T& operator*() const
        {
            return *mUniquePtr;
        }

    private:

        std::unique_ptr<T> mUniquePtr;

    };
}