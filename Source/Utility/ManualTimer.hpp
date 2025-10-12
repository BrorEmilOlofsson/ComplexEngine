#pragma once
#include <chrono>

namespace Simple
{

	template<typename T>
	class ManualTimer final
	{
	public:

		constexpr ManualTimer() = default;

		constexpr explicit ManualTimer(const std::chrono::duration<T> duration, bool autoStart = true)
			: mDuration(duration)
			, mIsRunning(autoStart)
		{
		}

		constexpr void Start()
		{
			mIsRunning = true;
		}

		constexpr void Stop()
		{
			mIsRunning = false;
		}

		constexpr void Reset()
		{
			mElapsedTime = 0.0f;
		}

		void SetDuration(const std::chrono::duration<T> duration)
		{
			mDuration = duration;
		}

		constexpr bool Update(const std::chrono::duration<T> deltaTime)
		{
			mHasBeenTriggered = false;
			if (mIsRunning)
			{
				mElapsedTime += deltaTime;

				if (mElapsedTime >= mDuration)
				{
					mElapsedTime = std::chrono::duration<T>::zero();
					mHasBeenTriggered = true;
				}
			}

			return mHasBeenTriggered;
		}

		[[nodiscard]] constexpr bool HasBeenTriggered() const
		{
			return mHasBeenTriggered;
		}

		[[nodiscard]] constexpr std::chrono::duration<T> GetElapsedTime() const
		{
			return mElapsedTime;
		}

	private:

		std::chrono::duration<T> mElapsedTime = std::chrono::duration<T>::zero();
		std::chrono::duration<T> mDuration = std::chrono::duration<T>::zero();
		bool mIsRunning = true;
		bool mHasBeenTriggered = false;
	};

	using ManualTimerf = ManualTimer<float>;
	using ManualTimerd = ManualTimer<double>;
}