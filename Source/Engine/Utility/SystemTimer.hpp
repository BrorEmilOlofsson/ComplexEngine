#pragma once
#include <chrono>
#include <concepts>

namespace Simple
{

	template<std::floating_point T>
	class SystemTimer final
	{
	public:

		constexpr SystemTimer();

		constexpr void Start();
		constexpr void Update();

		[[nodiscard]] constexpr std::chrono::duration<T> GetDeltaTime() const noexcept;
		[[nodiscard]] constexpr std::chrono::duration<T> GetTimeSinceLastMeasurement() const;
		[[nodiscard]] constexpr std::chrono::high_resolution_clock::time_point GetLastTimepoint() const noexcept;

	private:

		std::chrono::high_resolution_clock::time_point mLastTimePoint;
		std::chrono::duration<T> mDeltaTime = std::chrono::duration<T>::zero();
	};

	using SystemTimerf = SystemTimer<float>;
	using SystemTimerd = SystemTimer<double>;

	template<std::floating_point T>
	constexpr SystemTimer<T>::SystemTimer()
	{
		Start();
	}

	template<std::floating_point T>
	constexpr void SystemTimer<T>::Start()
	{
		mDeltaTime = std::chrono::duration<float>::zero();
		mLastTimePoint = std::chrono::high_resolution_clock::now();
	}

	template<std::floating_point T>
	constexpr void SystemTimer<T>::Update()
	{
		const auto now = std::chrono::high_resolution_clock::now();
		mDeltaTime = now - mLastTimePoint;
		mLastTimePoint = now;
	}

	template<std::floating_point T>
	constexpr std::chrono::duration<T> SystemTimer<T>::GetDeltaTime() const noexcept
	{
		return mDeltaTime;
	}

	template<std::floating_point T>
	constexpr std::chrono::high_resolution_clock::time_point SystemTimer<T>::GetLastTimepoint() const noexcept
	{
		return mLastTimePoint;
	}

	template<std::floating_point T>
	constexpr std::chrono::duration<T> SystemTimer<T>::GetTimeSinceLastMeasurement() const
	{
		return std::chrono::high_resolution_clock::now() - mLastTimePoint;
	}
}