#pragma once
#include "../FlyDefines.hpp"
#include <string>

namespace FLY_NAMESPACE
{
	class Variable final
	{
	public:

		Variable() = default;
		Variable(GenericDataTypeID dataTypeID, std::string name, std::size_t byteOffset);

		void SetDataTypeID(GenericDataTypeID dataTypeID);
		[[nodiscard]] GenericDataTypeID GetDataTypeID() const;
		void SetDefaultValueDataPtr(void* dataPtr);
		[[nodiscard]] OwningPtr<void> GetDefaultValueDataPtr() const;
		[[nodiscard]] std::string& Name();
		[[nodiscard]] const std::string& Name() const;

		[[nodiscard]] std::size_t GetByteOffset() const;
		void SetIsDestroyed(bool isDestroyed);
		[[nodiscard]] bool IsDestroyed() const;

	private:

		GenericDataTypeID mDataTypeID;
		OwningPtr<void> mDefaultValueDataPtr;
		std::string mName = "Var";

		std::size_t mByteOffset = 0;
		bool mIsDestroyed = false;
	};

	inline GenericDataTypeID Variable::GetDataTypeID() const
	{
		return mDataTypeID;
	}

	inline void Variable::SetDefaultValueDataPtr(void* dataPtr)
	{
		mDefaultValueDataPtr = dataPtr;
	}

	inline OwningPtr<void> Variable::GetDefaultValueDataPtr() const
	{
		return mDefaultValueDataPtr;
	}

	inline std::size_t Variable::GetByteOffset() const
	{
		return mByteOffset;
	}
}