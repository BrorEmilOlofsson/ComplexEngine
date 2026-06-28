#pragma once
#include <cstdint>

namespace CLX
{


    class ref_counter
    {
    public:

        ref_counter() = default;

        [[nodiscard]] uint64_t strong_count() const
        {
            return mStrongCount;
        }

        [[nodiscard]] uint64_t total_count() const
        {
            return mStrongCount + mWeakCount;
        }

        ref_counter& operator++()
        {
            ++mStrongCount;
            return *this;
        }

        ref_counter& operator--()
        {
            --mStrongCount;
            return *this;
        }

    private:

        uint64_t mStrongCount = 1;
        uint64_t mWeakCount = 0;

    };

    template<typename T>
    class queen_ptr final
    {
    public:

        queen_ptr()
            : mObject(new T())
            , mRefCounter(new ref_counter())
        {
        }

        ~queen_ptr()
        {
            if (mRefCounter)
            {
                --(*mRefCounter);
                if ((*mRefCounter).strong_count() == 0)
                {
                    delete mObject;
                }
                else if ((*mRefCounter).total_count() == 0)
                {
                    delete mRefCounter;
                }
            }
        }

        queen_ptr(const queen_ptr& other)
            : mObject(new T(*other.mObject))
            , mRefCounter(new ref_counter())
        {
        }

        queen_ptr(queen_ptr&& other) noexcept
            : mObject(std::exchange(other.mObject, nullptr))
            , mRefCounter(std::exchange(other.mRefCounter, nullptr))
        {}

        queen_ptr& operator=(const queen_ptr& other)
        {
            if (this != &other)
            {
                if (mRefCounter)
                {
                    --(*mRefCounter);
                    if ((*mRefCounter).strong_count() == 0)
                    {
                        delete mObject;
                    }
                    if ((*mRefCounter).total_count() == 0)
                    {
                        delete mRefCounter;
                    }
                }
                mObject = new T(*other.mObject);
                mRefCounter = new ref_counter();
            }
            return *this;
        }

        queen_ptr& operator=(queen_ptr&& other) noexcept
        {
            if (this != &other)
            {
                if (mRefCounter)
                {
                    --(*mRefCounter);
                    if ((*mRefCounter).strong_count() == 0)
                    {
                        delete mObject;
                    }
                    if ((*mRefCounter).total_count() == 0)
                    {
                        delete mRefCounter;
                    }
                }
                mObject = std::exchange(other.mObject, nullptr);
                mRefCounter = std::exchange(other.mRefCounter, nullptr);
            }
            return *this;
        }

        explicit queen_ptr(T* object)
            : mObject(object)
            , mRefCounter(new ref_counter())
        {
        }

        [[nodiscard]] uint64_t use_count() const
        {
            return mRefCounter ? (*mRefCounter).strong_count() : 0;
        }

        [[nodiscard]] T* get()
        {
            return mObject;
        }

        [[nodiscard]] const T* get() const
        {
            return mObject;
        }

        T& operator*()
        {
            return *mObject;
        }

        const T& operator*() const
        {
            return *mObject;
        }

        T* operator->()
        {
            return mObject;
        }

        const T* operator->() const
        {
            return mObject;
        }

        operator bool() const
        {
            return mObject != nullptr;
        }

    private:

        T* mObject = nullptr;
        ref_counter* mRefCounter = nullptr;

    };

    [[nodiscard]] decltype(auto) make_queen_ptr(auto&&... args)
    {
        return queen_ptr<std::decay_t<decltype(args)...>>(new std::decay_t<decltype(args)...>(std::forward<decltype(args)>(args)...));
    }

    // Weak pointer model
    template<typename T>
    class bee_ptr final
    {
    public:

        bee_ptr() = default;


    private:

        T* mObject = nullptr;
        ref_counter* mRefCounter = nullptr;
    };
}