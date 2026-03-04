#pragma once
#include "Engine/Utility/Input/InputKey.hpp"
#include "Engine/Utility/Input/InputState.hpp"

namespace Simple
{

	struct InputOperations
	{
		template<typename T>
		InputOperations(std::type_identity<T>)
			: captureMouseFunc([](void* inputManager) { ::CaptureMouse(*static_cast<T*>(inputManager)); })
			, releaseMouseFunc([](void* inputManager) { ::ReleaseMouse(*static_cast<T*>(inputManager)); })
			, showMouseFunc([](void* inputManager) { ::ShowMouse(*static_cast<T*>(inputManager)); })
			, hideMouseFunc([](void* inputManager) { ::HideMouse(*static_cast<T*>(inputManager)); })
		{
		}

		void (*captureMouseFunc)(void*) = nullptr;
		void (*releaseMouseFunc)(void*) = nullptr;
		void (*hideMouseFunc)(void*) = nullptr;
		void (*showMouseFunc)(void*) = nullptr;
	};

	class InputView final
	{
		template<typename T>
		explicit InputView(T& inputManager, const InputState* inputState)
			: mInputManager(&inputManager)
			, mInputState(inputState)
		{
		}

		[[nodiscard]] bool IsKeyPressed(const eInputKey aKey) const
		{
			return mInputState->IsKeyPressed(aKey);
		}

		[[nodiscard]] bool IsKeyHeld(const eInputKey aKey) const
		{
			return mInputState->IsKeyHeld(aKey);
		}

		[[nodiscard]] bool IsKeyReleased(const eInputKey aKey) const
		{
			return mInputState->IsKeyReleased(aKey);
		}

		[[nodiscard]] bool IsKeyDown(const eInputKey aKey) const
		{
			return mInputState->IsKeyDown(aKey);
		}

		void CaptureMouse() const
		{
			mOperations.captureMouseFunc(mInputManager);
		}

		void ReleaseMouse() const
		{
			mOperations.releaseMouseFunc(mInputManager);
		}

		void ShowMouse() const
		{
			mOperations.showMouseFunc(mInputManager);
		}

		void HideMouse() const
		{
			mOperations.hideMouseFunc(mInputManager);
		}

	private:

		void* mInputManager = nullptr;
		InputOperations mOperations;
		const InputState* mInputState = nullptr;
	};
}