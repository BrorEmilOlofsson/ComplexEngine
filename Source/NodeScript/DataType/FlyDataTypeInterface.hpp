#pragma once
#include "../FlyDefines.hpp"
#include <External/nlohmann/json.hpp>
#include <functional>

namespace FLY_NAMESPACE
{
	using InplaceAllocateF = FunctionWrapper<void(void*, const void*), struct InplaceAllocateParam>;
	using ReleaseF = FunctionWrapper<void(void*), struct ReleaseParam>;;
	using CopyF = FunctionWrapper<void(void*, const void*), struct CopyParam>;
	using SwapF = FunctionWrapper<void(void*, void*), struct SwapParam>;
	using EqualsF = FunctionWrapper<bool(const void*, const void*), struct EqualsParam>;

	using InplaceAllocateInterface = void(*)(void* dataPtr, const void* defaultValuePtr);
	using ReleaseInterface = void(*)(void* dataPtr);
	using CopyInterface = void(*)(void* aDestination, const void* source);
	using SwapInterface = void(*)(void* dataPtr1, void* dataPtr2);
	using EqualsInterface = bool(*)(const void* dataPtr1, const void* dataPtr2);

	struct FundamentalInterface final
	{
		InplaceAllocateF mInplaceAllocateF;
		ReleaseF mReleaseF;
		CopyF mCopyF;
		SwapF mSwapF;
		EqualsF mEqualsF;
	};

	using EditorTextFunction = std::function<void(const std::string&)>;

	using ViewAndEditF = FunctionWrapper<ViewAndEditResult(void* dataPtr, EditorTextFunction nullptrFunction), struct ViewAndEditParam>;
	using ViewF = FunctionWrapper<void(const void* dataPtr, EditorTextFunction nullptrFunction), struct ViewParam>;
	using SaveF = FunctionWrapper<void(const void* dataPtr, nlohmann::json& saveObject), struct SaveParam>;
	using LoadF = FunctionWrapper<void(void* dataPtr, const nlohmann::json& loadObject), struct LoadParam>;

	struct FunctionInterface final
	{
		ViewAndEditF mViewAndEditF;
		ViewF mViewF;
		SaveF mSaveF;
		LoadF mLoadF;
	};

	struct ExecutionInterface final
	{
		SetPinValueF mSetInputPinValueF;
		SetPinValueF mSetOutputPinValueF;
		SetPinValueFromPinF mSetInputPinValueFromPinF;
		SetPinValueFromPinF mSetOutputPinValueFromPinF;
	};

	class DataTypeInterface final
	{
	public:

		DataTypeInterface() = default;
		DataTypeInterface(FundamentalInterface fundamentalInterface, FunctionInterface functionInterface, ExecutionInterface executionInterface);

		[[nodiscard]] InplaceAllocateF GetInplaceAllocateFunction() const;
		[[nodiscard]] ReleaseF GetReleaseFunction() const;
		[[nodiscard]] CopyF GetCopyFunction() const;
		[[nodiscard]] SwapF GetSwapFunction() const;
		[[nodiscard]] EqualsF GetEqualsFunction() const;

		[[nodiscard]] ViewAndEditF GetViewAndEditFunction() const;
		[[nodiscard]] ViewF GetViewFunction() const;
		[[nodiscard]] SaveF GetSaveFunction() const;
		[[nodiscard]] LoadF GetLoadFunction() const;

		[[nodiscard]] SetPinValueF GetSetInputPinValueFunction() const;
		[[nodiscard]] SetPinValueF GetSetOutputPinValueFunction() const;
		[[nodiscard]] SetPinValueFromPinF GetSetInputPinValueFromPinFunction() const;
		[[nodiscard]] SetPinValueFromPinF GetSetOutputPinValueFromPinFunction() const;

	private:

		FundamentalInterface mFundamental;
		FunctionInterface mFunction;
		ExecutionInterface mExecution;
	};

	inline InplaceAllocateF DataTypeInterface::GetInplaceAllocateFunction() const
	{
		return mFundamental.mInplaceAllocateF;
	}

	inline ReleaseF DataTypeInterface::GetReleaseFunction() const
	{
		return mFundamental.mReleaseF;
	}

	inline CopyF DataTypeInterface::GetCopyFunction() const
	{
		return mFundamental.mCopyF;
	}

	inline SwapF DataTypeInterface::GetSwapFunction() const
	{
		return mFundamental.mSwapF;
	}

	inline EqualsF DataTypeInterface::GetEqualsFunction() const
	{
		return mFundamental.mEqualsF;
	}

	inline ViewAndEditF DataTypeInterface::GetViewAndEditFunction() const
	{
		return mFunction.mViewAndEditF;
	}

	inline ViewF DataTypeInterface::GetViewFunction() const
	{
		return mFunction.mViewF;
	}

	inline SaveF DataTypeInterface::GetSaveFunction() const
	{
		return mFunction.mSaveF;
	}

	inline LoadF DataTypeInterface::GetLoadFunction() const
	{
		return mFunction.mLoadF;
	}

	inline SetPinValueF DataTypeInterface::GetSetInputPinValueFunction() const
	{
		return mExecution.mSetInputPinValueF;
	}

	inline SetPinValueF DataTypeInterface::GetSetOutputPinValueFunction() const
	{
		return mExecution.mSetOutputPinValueF;
	}

	inline SetPinValueFromPinF DataTypeInterface::GetSetInputPinValueFromPinFunction() const
	{
		return mExecution.mSetInputPinValueFromPinF;
	}

	inline SetPinValueFromPinF DataTypeInterface::GetSetOutputPinValueFromPinFunction() const
	{
		return mExecution.mSetOutputPinValueFromPinF;
	}
}