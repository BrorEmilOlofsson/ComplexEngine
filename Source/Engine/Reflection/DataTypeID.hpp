#pragma once
#include <limits>

namespace CLX
{
	struct DataTypeID final
	{
		std::size_t myID = std::numeric_limits<std::size_t>::max();
	};

	constexpr DataTypeID InvalidDataTypeID = DataTypeID{};

	constexpr bool operator==(DataTypeID a, DataTypeID b)
	{
		return a.myID == b.myID;
	}

	constexpr bool operator<(DataTypeID a, DataTypeID b)
	{
		return a.myID < b.myID;
	}

	[[nodiscard]] inline DataTypeID GetDataTypeID(const std::type_info& typeInfo)
	{
		return DataTypeID{ typeInfo.hash_code() };
	}

	template<typename T>
	[[nodiscard]] inline DataTypeID GetDataTypeID()
	{
        return GetDataTypeID(typeid(T));
	}
}

namespace std
{
	template<>
	struct hash<CLX::DataTypeID>
	{
		std::size_t operator()(CLX::DataTypeID a) const
		{
			return a.myID;
		}
	};
}