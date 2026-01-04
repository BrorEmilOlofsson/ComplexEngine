#pragma once
#include "../FlyDefines.hpp"
#include "../SystemTypes/FlyVec2.hpp"
#include "../Pin/FlyPin.hpp"
#include <vector>

namespace FLY_NAMESPACE
{

	class Node final
	{
	public:

		Node(const NodeTypeID typeID, const auto& inputPins, const auto& outputPins, const void* inputTuplePtr)
			: mTypeID(typeID)
			, mInputPins(begin(inputPins), end(inputPins))
			, mOutputPins(begin(outputPins), end(outputPins))
			, mSplitInputPins(mInputPins)
			, mSplitOutputPins(mOutputPins)
			, mInputTuplePtr(inputTuplePtr)
		{
		}

		Node(const NodeTypeID typeID, const std::vector<PinID>& inputPins, const std::vector<PinID>& outputPins, const void* inputTuplePtr);

		[[nodiscard]] constexpr NodeTypeID GetTypeID() const;
		[[nodiscard]] constexpr std::vector<PinID>& GetInputPins();
		[[nodiscard]] constexpr const std::vector<PinID>& GetInputPins() const;
		[[nodiscard]] constexpr std::vector<PinID>& GetOutputPins();
		[[nodiscard]] constexpr const std::vector<PinID>& GetOutputPins() const;
		[[nodiscard]] constexpr std::vector<PinID>& GetSplitInputPins();
		[[nodiscard]] constexpr const std::vector<PinID>& GetSplitInputPins() const;
		[[nodiscard]] constexpr std::vector<PinID>& GetSplitOutputPins();
		[[nodiscard]] constexpr const std::vector<PinID>& GetSplitOutputPins() const;
		[[nodiscard]] constexpr const void* GetInputTuplePtr() const;
		[[nodiscard]] Vec2 GetPosition() const;
		[[nodiscard]] bool IsDestroyed() const;

		void SetPosition(Vec2 position);
		void SetIsDestroyed(bool isDestroyed);

	private:

		NodeTypeID mTypeID;
		std::vector<PinID> mInputPins;
		std::vector<PinID> mOutputPins;
		std::vector<PinID> mSplitInputPins;
		std::vector<PinID> mSplitOutputPins;
		const void* mInputTuplePtr = nullptr;
		Vec2 mPosition;
		bool mIsDestroyed = false;
	};

	constexpr NodeTypeID Node::GetTypeID() const
	{
		return mTypeID;
	}

	constexpr std::vector<PinID>& Node::GetInputPins()
	{
		return mInputPins;
	}

	constexpr const std::vector<PinID>& Node::GetInputPins() const
	{
		return mInputPins;
	}

	constexpr std::vector<PinID>& Node::GetOutputPins()
	{
		return mOutputPins;
	}

	constexpr const std::vector<PinID>& Node::GetOutputPins() const
	{
		return mOutputPins;
	}

	constexpr std::vector<PinID>& Node::GetSplitInputPins()
	{
		return mSplitInputPins;
	}

	constexpr const std::vector<PinID>& Node::GetSplitInputPins() const
	{
		return mSplitInputPins;
	}

	constexpr std::vector<PinID>& Node::GetSplitOutputPins()
	{
		return mSplitOutputPins;
	}

	constexpr const std::vector<PinID>& Node::GetSplitOutputPins() const
	{
		return mSplitOutputPins;
	}

	constexpr const void* Node::GetInputTuplePtr() const
	{
		return mInputTuplePtr;
    }

}
