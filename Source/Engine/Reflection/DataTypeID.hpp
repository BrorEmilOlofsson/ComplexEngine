#pragma once
#include <typeindex>

namespace CLX
{
	struct DataTypeID final
	{
		std::type_index typeIndex = typeid(void);
	};

	inline static DataTypeID InvalidDataTypeID = DataTypeID{};

	[[nodiscard]] inline bool operator==(DataTypeID a, DataTypeID b)
	{
		return a.typeIndex == b.typeIndex;
	}

	[[nodiscard]] inline bool operator<(DataTypeID a, DataTypeID b)
	{
		return a.typeIndex < b.typeIndex;
	}

	[[nodiscard]] inline DataTypeID GetDataTypeID(const std::type_info& typeInfo)
	{
		return DataTypeID{ std::type_index{ typeInfo } };
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
			return a.typeIndex.hash_code();
		}
	};
}