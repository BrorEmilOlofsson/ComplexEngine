#pragma once

namespace Simple
{
    template<std::size_t N>
    class FixedString final
    {
    public:

        constexpr FixedString() = default;

        constexpr FixedString(const char(&str)[N + 1])
            : mSize(N)
        {
            std::copy_n(str, N + 1, mData);
        }

        constexpr FixedString(const std::string_view sv)
        {
            assign(sv);
        }

        constexpr FixedString(auto aStart, auto aEnd)
        {
            const std::size_t distance = std::distance(aStart, aEnd);
            if (distance >= N) throw std::length_error("Too long");
            std::copy_n(&*aStart, distance, mData);
            mSize = distance;
        }

        constexpr void assign(const char* str)
        {
            std::size_t len = std::strlen(str);
            if (len > N) throw std::length_error("FixedString: input too long");
            std::copy_n(str, len, mData);
            mData[len] = '\0';
            mSize = len;
        }

        constexpr void assign(const std::string_view sv)
        {
            if (sv.size() > N) throw std::length_error("FixedString: input too long");
            std::copy_n(sv.begin(), sv.size(), mData);
            mData[sv.size()] = '\0';
            mSize = sv.size();
        }

        constexpr std::size_t size() const noexcept { return mSize; }
        constexpr std::size_t capacity() const noexcept { return N; }

        constexpr const char* c_str() const noexcept { return mData; }
        constexpr char* data() noexcept { return mData; }

        constexpr const char& operator[](std::size_t i) const noexcept { return mData[i]; }
        constexpr char& operator[](std::size_t i) noexcept { return mData[i]; }

        constexpr std::string_view view() const noexcept { return std::string_view(mData, mSize); }

        constexpr auto operator<=>(const FixedString&) const noexcept = default;

    public:

        char mData[N + 1]{};
        std::size_t mSize = 0;
    };

	template<std::size_t Size>
	FixedString(const char(&)[Size]) -> FixedString<Size - 1>;
}