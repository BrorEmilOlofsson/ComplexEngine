#pragma once
#include <algorithm>
#include <array>
#include <cstring>
#include <stdexcept>
#include <string>
#include <string_view>

namespace CLX
{
    template<std::size_t N>
    class FixedString final
    {
    public:

        constexpr FixedString(std::array<char, N + 1> data, size_t size)
            : mData(data)
            , mSize(size)
        {
        }

    public:

        constexpr FixedString() = default;

        template<std::size_t M> requires (M > 0 && (M - 1) <= N)
        consteval FixedString(const char(&str)[M])
            : mSize(M - 1)
        {
            std::copy_n(str, M, mData.data()); // includes '\0'
        }

        [[nodiscard]] static constexpr FixedString FromStringView(const std::string_view sv)
        {
            if (sv.size() > N) throw std::length_error("FixedString: input too long");
            std::array<char, N + 1> data{};
            std::copy_n(sv.begin(), sv.size(), data.data());
            data[sv.size()] = '\0';
            return FixedString<N>(data, sv.size());
        }

        [[nodiscard]] static constexpr FixedString FromString(const std::string& str)
        {
            std::array<char, N + 1> data{};
            size_t size = std::min(str.size(), N);
            std::copy_n(str.data(), size, data.data());
            return FixedString<N>(data, size);
        }

        [[nodiscard]] static constexpr FixedString FromCharArray(const char(&str)[N + 1])
        {
            std::array<char, N + 1> data{};
            std::copy_n(str, N + 1, data.data());
            return FixedString<N>(data, N);
        }

        [[nodiscard]] static constexpr FixedString FromCString(const char* str)
        {
            std::size_t len = std::strlen(str);
            return FixedString<N>::FromCString(str, len);
        }
        
        [[nodiscard]] static constexpr FixedString FromCString(const char* str, std::size_t length)
        {
            if (length > N) throw std::length_error("FixedString: input too long");
            std::array<char, N + 1> data{};
            std::copy_n(str, length, data.data());
            data[length] = '\0';
            return FixedString<N>(data, length);
        }

        constexpr std::size_t size() const noexcept { return mSize; }
        constexpr std::size_t capacity() const noexcept { return N; }

        [[nodiscard]] constexpr const char* c_str() const noexcept { return mData.data(); }
        [[nodiscard]] constexpr char* data() noexcept { return mData.data(); }

        [[nodiscard]] constexpr const char& operator[](std::size_t i) const noexcept { return mData[i]; }
        [[nodiscard]] constexpr char& operator[](std::size_t i) noexcept { return mData[i]; }

        [[nodiscard]] constexpr std::string_view view() const noexcept { return std::string_view(mData.data(), mSize); }

        constexpr auto operator<=>(const FixedString&) const noexcept = default;

        [[nodiscard]] constexpr auto begin()
        {
            return mData.begin();
        }

        [[nodiscard]] constexpr auto end()
        {
            return mData.begin() + mSize;
        }

        [[nodiscard]] constexpr auto begin() const
        {
            return mData.begin();
        }

        [[nodiscard]] constexpr auto end() const
        {
            return mData.begin() + mSize;
        }

    public:

        std::array<char, N + 1> mData{};
        std::size_t mSize = 0;
    };

	template<std::size_t Size>
	FixedString(const char(&)[Size]) -> FixedString<Size - 1>;

}