#pragma once

namespace Simple
{

	template<typename T>
	class ValuePtr final
	{
	public:

		ValuePtr()
			: mPtr(new T())
		{
		}

		ValuePtr(const T& pValue)
			: mPtr(new T(pValue))
		{
		}

		ValuePtr(const ValuePtr& pOther)
			: mPtr(new T(*pOther.mPtr))
		{
		}

		ValuePtr(ValuePtr&& pOther) noexcept
			: mPtr(pOther.mPtr)
		{
			pOther.mPtr = nullptr;
		}

		ValuePtr& operator=(const ValuePtr& pOther)
		{
			delete mPtr;
			mPtr = new T(*pOther.mPtr);

			return *this;
		}

		ValuePtr& operator=(ValuePtr&& pOther) noexcept
		{
			delete mPtr;
			mPtr = pOther.mPtr;
			pOther.mPtr = nullptr;

			return *this;
		}

		~ValuePtr()
		{
			delete mPtr;
		}

		[[nodiscard]] constexpr T& Get()
		{
			return *mPtr;
		}

		[[nodiscard]] constexpr const T& Get() const
		{
			return *mPtr;
		}

		[[nodiscard]] constexpr T* operator->()
		{
			return mPtr;
		}

		[[nodiscard]] constexpr const T* operator->() const
		{
			return mPtr;
		}

		[[nodiscard]] constexpr T& operator*()
		{
			return Get();
		}

		[[nodiscard]] constexpr const T& operator*() const
		{
			return Get();
		}

	private:

		T* mPtr = nullptr;
	};
}