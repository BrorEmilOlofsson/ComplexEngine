#pragma once
#include <algorithm>
#include <memory>
#include <type_traits>
#include <cstring>
#include <utility>
#include <concepts>

#include "Engine/Utility/FunctionPtrWrapper.hpp"

namespace CLX
{

    using InplaceConstructFunction = FunctionPtrWrapper<void(void*, const void*), struct InplaceFTag>;
    using DestroyFunction = FunctionPtrWrapper<void(void*), struct DestructFTag>;
    using CopyConstructFunction = FunctionPtrWrapper<void(void*, const void*), struct CopyConstructFTag>;
    using CopyFunction = FunctionPtrWrapper<void(void*, const void*), struct CopyFTag>;
    using MoveConstructFunction = FunctionPtrWrapper<void(void*, void*), struct MoveConstructFTag>;
    using MoveFunction = FunctionPtrWrapper<void(void*, void*), struct MoveFTag>;
    using SwapFunction = FunctionPtrWrapper<void(void*, void*), struct SwapFTag>;
    using EqualsFunction = FunctionPtrWrapper<bool(const void*, const void*), struct EqualsFTag>;

    template<typename T>
    [[nodiscard]] constexpr InplaceConstructFunction CreateInplaceConstructFunction()
    {
        return InplaceConstructFunction(
            [](void* ptr, const void* defaultValuePtr) -> void
            {
                T* constructAtPtr = reinterpret_cast<T*>(ptr);
                if (defaultValuePtr)
                {
                    const T& defaultValue = *reinterpret_cast<const T*>(defaultValuePtr);
                    std::construct_at(constructAtPtr, defaultValue);
                }
                else
                {
                    std::construct_at(constructAtPtr);
                }
            });
    }

    template<typename T>
    [[nodiscard]] constexpr DestroyFunction CreateDestroyFunction()
    {
        return DestroyFunction(
            [](void* ptr) -> void
            {
                T* destroyAtPtr = reinterpret_cast<T*>(ptr);
                std::destroy_at(destroyAtPtr);
            });
    }

    template<typename T>
    [[nodiscard]] constexpr CopyConstructFunction CreateCopyConstructFunction()
    {
        return CopyConstructFunction(
            [](void* destinationPtr, const void* sourcePtr) -> void
            {
                const T& sourceValue = *reinterpret_cast<const T*>(sourcePtr);
                std::construct_at(reinterpret_cast<T*>(destinationPtr), sourceValue);
            });
    }

    template<typename T>
    [[nodiscard]] constexpr CopyFunction CreateCopyFunction()
    {
        return CopyFunction(
            [](void* destinationPtr, const void* sourcePtr) -> void
            {
                if constexpr (std::is_trivially_copyable_v<T>)
                {
                    std::memcpy(destinationPtr, sourcePtr, sizeof(T));
                }
                else
                {
                    const T& sourceValue = *reinterpret_cast<const T*>(sourcePtr);
                    T& destValue = *reinterpret_cast<T*>(destinationPtr);
                    destValue = sourceValue;
                }
            });
    }

    template<typename T>
    [[nodiscard]] constexpr MoveConstructFunction CreateMoveConstructFunction()
    {
        return MoveConstructFunction(
            [](void* destinationPtr, void* sourcePtr) -> void
            {
                T& sourceValue = *reinterpret_cast<T*>(sourcePtr);
                std::construct_at(reinterpret_cast<T*>(destinationPtr), std::move(sourceValue));
            });
    }

    template<typename T>
    [[nodiscard]] constexpr MoveFunction CreateMoveFunction()
    {
        return MoveFunction(
            [](void* destinationPtr, void* sourcePtr) -> void
            {
                T& sourceValue = *reinterpret_cast<T*>(sourcePtr);
                T& destValue = *reinterpret_cast<T*>(destinationPtr);
                destValue = std::move(sourceValue);
            });
    }

    template<typename T>
    [[nodiscard]] constexpr SwapFunction CreateSwapFunction()
    {
        return SwapFunction(
            [](void* dataPtr1, void* dataPtr2) -> void
            {
                T& value1 = *reinterpret_cast<T*>(dataPtr1);
                T& value2 = *reinterpret_cast<T*>(dataPtr2);
                using std::swap;

                swap(value1, value2);
            });
    }

    template<std::equality_comparable T>
    [[nodiscard]] constexpr EqualsFunction CreateEqualsFunction()
    {
        return EqualsFunction(
            [](const void* dataPtr1, const void* dataPtr2) -> bool
            {
                const T& value1 = *reinterpret_cast<const T*>(dataPtr1);
                const T& value2 = *reinterpret_cast<const T*>(dataPtr2);
                return value1 == value2;
            });
    }
}