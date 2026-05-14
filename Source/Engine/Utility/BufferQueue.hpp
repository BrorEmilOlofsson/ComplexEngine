#pragma once
#include <vector>
#include <utility>

namespace CLX
{

    template<typename T>
    class buffer_queue;

    template<typename T>
    class buffer_queue_iterator
    {
    public:

        using value_type = T;

        constexpr buffer_queue_iterator() = default;

        constexpr buffer_queue_iterator(buffer_queue<T>* queue, std::size_t index)
            : mQueue(queue)
            , mIndex(index)
        {}

        [[nodiscard]] constexpr value_type& operator*()
        {
            return (*mQueue)[mIndex];
        }

    private:

        buffer_queue<T>* mQueue = nullptr;
        std::size_t mIndex{ 0 };

    };

    struct empty_buffer_queue_exception : public std::exception
    {
        [[nodiscard]] const char* what() const noexcept override
        {
            return "Buffer queue is empty.";
        }
    };

    template<typename T>
    class buffer_queue
    {
    public:

        buffer_queue() = default;

        constexpr void push(const T& item)
        {
            if (mPushIndex == mPopIndex)
            {
                reallocate();
            }
            if (mPushIndex < mPopIndex)
            {
                mData[mPushIndex] = item;
                mPushIndex++;
            }
            else if (mPopIndex > 0)
            {
                mPushIndex = 0;
                mData[mPushIndex] = item;
                mPushIndex++;
            }
            else
            {
                mData.push_back(item);
                mPushIndex++;
            }
        }

        constexpr void push(T&& item)
        {
            if (mPushIndex < mPopIndex)
            {
                mData[mPushIndex] = std::move(item);
                mPushIndex++;
            }
            else if (mPushIndex < mData.size())
            {
                mData[mPushIndex] = std::move(item);
                mPushIndex++;
            }
            else if (mPopIndex > 1)
            {
                mPushIndex = 0;
                mData[mPushIndex] = std::move(item);
                mPushIndex++;
            }
            else
            {
                mData.push_back(std::move(item));       
                mPushIndex++;
            }

            if (mPushIndex + 1 == mPopIndex)
            {
                reallocate();
            }
        }

        [[nodiscard]] constexpr T& front()
        {
            return mData[mPopIndex];
        }

        [[nodiscard]] constexpr const T& front() const
        {
            return mData[mPopIndex];
        }

        constexpr void pop()
        {
            if (is_empty())
            {
                throw empty_buffer_queue_exception();
            }
            mData[mPopIndex] = T{};
            if (mPopIndex + 1 == mPushIndex)
            {
                mPopIndex = 0;
                mPushIndex = 0;
            }
            else
            {
                mPopIndex = (mPopIndex + 1) % (mData.size());
            }
        }

        constexpr void clear()
        {
            mData.clear();
            mPushIndex = 0;
            mPopIndex = 0;
        }

        [[nodiscard]] constexpr std::size_t size() const
        {
            if (mPushIndex < mPopIndex || (mPushIndex == mPopIndex && mPopIndex != 0))
            {
                return mPushIndex + mData.size() - mPopIndex;
            }
            else
            {
                return mPushIndex - mPopIndex;
            }
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return size() == 0;
        }

        [[nodiscard]] constexpr buffer_queue_iterator<T> begin()
        {
            return buffer_queue_iterator<T>(this, mPopIndex);
        }

        [[nodiscard]] constexpr buffer_queue_iterator<T> end()
        {
            return buffer_queue_iterator<T>(this, mPushIndex);
        }
    private:

        void reallocate()
        {
            const std::size_t oldSize = mData.size();
            const std::size_t newSize = mData.size() * 2;
            mData.resize(newSize);
            const std::size_t newPopIndex = newSize - (oldSize - mPopIndex);
            for (std::size_t i = 0; i < oldSize - mPopIndex; i++)
            {
                mData[newPopIndex + i] = std::move(mData[mPopIndex + i]);
            }
            mPopIndex = newPopIndex;
        }

    private:

        std::vector<T> mData;
        std::size_t mPushIndex{ 0 };
        std::size_t mPopIndex{ 0 };
    };
}