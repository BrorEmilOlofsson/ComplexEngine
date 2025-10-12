#pragma once
#include <vector>
#include <utility>
#include <optional>

template<typename T>
concept Equalizable = requires(const T & a, const T & b)
{
	a == b;
};

template<typename T>
concept ValidKeyType = Equalizable<T>;

template<typename KeyType>
constexpr std::size_t RobinHoodHash(const KeyType& pKey, const std::size_t pBucketSize)
{
	return std::hash<KeyType>{}(pKey) % pBucketSize;
}

template<ValidKeyType KeyType, typename ValueType>
class RobinHoodMap
{
	static constexpr std::size_t DefaultBucketSize = 16;

	class Slot final
	{
	public:
		KeyType mKey;
		ValueType mValue;
	};
public:

	RobinHoodMap()
		: mTable(DefaultBucketSize)
	{
	}


	[[nodiscard]] ValueType& operator[](const KeyType& pKey)
	{
		if (mUsedSlots == mTable.size())
		{
			Resize();
		}

		const std::size_t index = RobinHoodHash(pKey, mTable.size());
		if (!mTable[index])
		{
			mTable[index] = Slot{ pKey, ValueType{} };
			mUsedSlots++;
		}
		return mTable[index]->mValue;
	}


	const ValueType& operator[](const KeyType& pKey) const
	{
		const std::size_t index = RobinHoodHash(pKey, mTable.size());
		return mTable[index]->mValue;
	}


private:


	void Resize()
	{
		const std::size_t oldSize = mTable.size();
		const std::size_t newSize = oldSize * 2;
		const std::vector<std::optional<Slot>> oldTable = std::move(mTable);
		mTable.resize(newSize);
		
		for (size_t i = 0; i < oldSize; i++)
		{
			if (oldTable[i])
			{
				const std::size_t newIndex = RobinHoodHash(oldTable[i]->mKey, newSize);
				mTable[newIndex] = oldTable[i].value();
			}
		}
	}

private:

	std::vector<std::optional<Slot>> mTable;
	std::size_t mUsedSlots = 0;
};