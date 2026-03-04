#pragma once
#define FLY_NAMESPACE Fly
#if defined _DEBUG
#define FLY_DEBUG
#endif
#include <limits>
#include <type_traits>
#include <algorithm>
#include <string_view>
#include <string>
#include <variant>
#include <xhash>
#include "SystemTypes/FlyNone.hpp"

#define FLY_CONCATENATE_DETAIL(x, y) x##y
#define FLY_CONCATENATE(x, y) FLY_CONCATENATE_DETAIL(x, y)

#define FLY_UNIQUE_NAME(base) FLY_CONCATENATE(base, __COUNTER__)

namespace FLY_NAMESPACE
{
	using MemoryPoolID = size_t;
	using ObjectTypeID = size_t;

	template<std::integral IDType>
	constexpr IDType InvalidID()
	{
		return std::numeric_limits<IDType>::max();
	}

	template<std::integral IDType>
	constexpr IDType InvalidIndex()
	{
		return std::numeric_limits<IDType>::max();
	}

	template<typename IDType>
	struct IDWrapper
	{
		using value_type = IDType;

		constexpr friend bool operator==(const IDWrapper& a, const IDWrapper& b)
		{
			return a.mID == b.mID;
		}
		constexpr operator value_type() const
		{
			return mID;
		}

		IDWrapper operator++(int)
		{
			mID++;
			return *this;
		}

		IDWrapper& operator++()
		{
			++mID;
			return *this;
		}

	private:

		static constexpr IDType GetInitialized()
		{
			if constexpr (std::is_fundamental_v<IDType>)
			{
				return InvalidID<IDType>();
			}
			else
			{
				return IDType{};
			}
		}

	public:
		IDType mID = GetInitialized();
	};

	template<typename IDWrapperType> requires std::is_base_of_v<IDWrapper<typename IDWrapperType::value_type>, IDWrapperType>
	constexpr IDWrapperType InvalidID()
	{
		return IDWrapperType{ InvalidID<typename IDWrapperType::value_type>() };
	}

	struct PinID final : IDWrapper<int> {};
	struct LinkID final : IDWrapper<int> {};
	struct NodeID final : IDWrapper<int> {};
	struct DataTypeID final : IDWrapper<size_t> {};
	struct StructID final : IDWrapper<size_t> {};
	struct ClassID final : IDWrapper<size_t> {};
	struct PinTypeID final : IDWrapper<size_t> {};
	struct NodeTypeID final : IDWrapper<size_t> {};
	struct VarID final : IDWrapper<size_t> {};
	struct FunctionID final : IDWrapper<size_t> {};
	struct EventID final : IDWrapper<size_t> {};
    struct GenericDataTypeID final : IDWrapper<std::variant<DataTypeID, ClassID>> {};
	struct TraitID final : IDWrapper<size_t> {};

	template<>
	constexpr GenericDataTypeID InvalidID()
	{
		return GenericDataTypeID{ DataTypeID{} };
	}

	constexpr const char* TypeIdentifierStr = "#T";

	template<typename T>
	constexpr DataTypeID GetDataTypeID()
	{
		return DataTypeID{ typeid(T).hash_code() };
	}

	template<typename T>
	class OwningPtr final
	{
	public:

		constexpr OwningPtr() = default;
		constexpr OwningPtr(T* const ptr)
			: mPtr(ptr)
		{
		}

		constexpr T* Get() noexcept
		{
			return mPtr;
		}

		constexpr const T* Get() const noexcept
		{
			return mPtr;
		}

		constexpr operator T* () noexcept
		{
			return Get();
		}

		constexpr operator const T* () const noexcept
		{
			return Get();
		}

	private:
		T* mPtr = nullptr;
	};

	template<typename T>
	class NonOwningPtr final
	{
	public:

		constexpr NonOwningPtr() = default;
		constexpr NonOwningPtr(T* ptr)
			: mPtr(ptr)
		{
		}

		constexpr T* Get() const noexcept
		{
			return mPtr;
		}

		constexpr explicit operator T* () const noexcept
		{
			return Get();
		}

	private:

		T* mPtr = nullptr;
	};

	// Values between 0 and 1
	struct Color final
	{

		constexpr Color() = default;
		constexpr Color(float r, float g, float b, float a = 1.f)
			: r(r)
			, g(g)
			, b(b)
			, a(a)
		{

		}
		float r = 0, g = 0, b = 0, a = 1;

		constexpr void Clamp()
		{
			r = std::clamp(r, 0.f, 1.f);
			g = std::clamp(g, 0.f, 1.f);
			b = std::clamp(b, 0.f, 1.f);
			a = std::clamp(a, 0.f, 1.f);
		}

	};

	constexpr Color operator+(const Color& lhs, const Color& rhs)
	{
		Color c = { lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a };
		c.Clamp();
		return c;
	}

	constexpr Color operator-(const Color& lhs, const Color& rhs)
	{
		Color c = { lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a };
		c.Clamp();
		return c;
	}

	namespace Colors
	{
		constexpr Color Black = Color();
		constexpr Color White = Color(1.f, 1.f, 1.f);
		constexpr Color Red = Color(1.f, 0.f, 0.f);
		constexpr Color Blue = Color(0.f, 0.f, 1.f);
		constexpr Color Green = Color(0.f, 1.f, 0.f);
		constexpr Color Yellow = Color(1.f, 1.f, 0.f);
		constexpr Color Orange = Color(1.f, 0.65f, 0.f);
		constexpr Color Pink = Color(1.f, 0.57f, 0.69f);
		constexpr Color Purple = Color(0.6f, 0.f, 0.1f);
		constexpr Color Gray = Color(0.5f, 0.5f, 0.5f);
	}

	struct ViewAndEditResult final
	{
		bool mIsItemActive = false;
	};

}

namespace std
{

	template<typename IDType>
	struct hash<Fly::IDWrapper<IDType>>
	{
		IDType operator()(const Fly::IDWrapper<IDType>& aIDWrapper) const
		{
			return aIDWrapper.mID;
		}
	};

	template<>
	struct hash<Fly::DataTypeID> : hash<Fly::IDWrapper<Fly::DataTypeID::value_type>> {};

	template<>
	struct hash<Fly::ClassID> : hash<Fly::IDWrapper<Fly::ClassID::value_type>> {};

	template<>
	struct hash<Fly::NodeID> : hash<Fly::IDWrapper<Fly::NodeID::value_type>> {};

	template<>
	struct hash<Fly::NodeTypeID> : hash<Fly::IDWrapper<Fly::NodeTypeID::value_type>> {};

	template<>
	struct hash<Fly::EventID> : hash<Fly::IDWrapper<Fly::EventID::value_type>> {};

	template<>
	struct hash<Fly::TraitID> : hash<Fly::IDWrapper<Fly::TraitID::value_type>> {};
}