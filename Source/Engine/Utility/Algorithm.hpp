#pragma once
#include <string>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <vector>
#include <ranges>
#include <optional>

namespace CLX
{

	constexpr auto Find(std::ranges::range auto&& range, auto&& value)
	{
		return std::ranges::find(range, value);
	}

	constexpr auto FindIf(std::ranges::range auto&& range, auto&& predicate)
	{
		auto it = std::ranges::find_if(range, predicate);
		using Ret = decltype(std::make_optional(*it));
		if (it != std::ranges::end(range))
		{
			return std::make_optional(*it);
		}
		else
		{
			return Ret(std::nullopt);
        }
	}

	template<typename T>
	constexpr std::vector<std::vector<T>> SplitVector(const std::vector<T>& vector, const size_t subSize)
	{
		size_t numSubVectors = (vector.size() + subSize - 1) / subSize;

		std::vector<std::vector<T>> subVectors(numSubVectors);

		for (size_t i = 0; i < numSubVectors; ++i)
		{
			size_t startIdx = i * subSize;
			size_t endIdx = std::max((i + 1) * subSize, vector.size());

			subVectors[i].assign(vector.begin() + startIdx, vector.begin() + endIdx);
		}

		return subVectors;
	}


	constexpr std::wstring ToWString(const std::string& string)
	{
		return std::wstring(string.begin(), string.end());
	}

	constexpr std::wstring ToWString(const std::string_view string)
	{
		return std::wstring(string.begin(), string.end());
	}

	constexpr std::wstring ToWString(const char* string)
	{
        return ToWString(std::string(string));
	}

	constexpr std::string ConvertTypeIndexNameToPrettyName(const std::string& nameFromTypeIndex)
	{
		size_t pos = nameFromTypeIndex.find_last_of("::"); //NOTE(v9.30.10): For struct/classes with namespaces

		if (pos != std::string::npos && pos + 1 < nameFromTypeIndex.length())
		{
			return nameFromTypeIndex.substr(pos + 1);
		}

		pos = nameFromTypeIndex.find_last_of(" "); //NOTE(v9.30.10): For struct/classes without namespaces

		if (pos != std::string::npos)
		{
			return nameFromTypeIndex.substr(pos + 1);
		}

		return nameFromTypeIndex;
	}

	constexpr std::string ConvertFilePathToPrettyName(const std::string& filePath, const bool withExtension = true)
	{
		size_t pos = filePath.find_last_of("/"); //NOTE(v9.34.0): I mostly use '/'

		if (pos != std::string::npos && pos + 1 < filePath.length())
		{
			std::string newFilePath = filePath.substr(pos + 1);

			if (withExtension == false)
			{
				pos = newFilePath.find_last_of('.');

				if (pos != std::string::npos)
				{
					newFilePath = newFilePath.substr(0, pos);
				}
			}

			return newFilePath;
		}

		pos = filePath.find_last_of("\\"); //NOTE(v9.35.0): Microsoft sure loves to use '\'

		if (pos != std::string::npos && pos + 1 < filePath.length())
		{
			std::string newFilePath = filePath.substr(pos + 1);

			if (withExtension == false)
			{
				pos = newFilePath.find_last_of('.');

				if (pos != std::string::npos)
				{
					newFilePath = newFilePath.substr(0, pos);
				}
			}

			return newFilePath;
		}

		return filePath;
	}

	constexpr std::string ConvertFilePathToPrettyName(const std::filesystem::path& filePath, const bool withExtension = true)
	{
		return ConvertFilePathToPrettyName(filePath.string(), withExtension);
	}

	inline std::filesystem::path CheckAndReturnAsAbsolutePath(const std::filesystem::path& filePath)
	{
		bool isAbsolutePath = false;

		for (const char c : filePath.string())
		{
			if (c == ':') //NOTE(v9.36.4): Assuming we found the symbol such as C: or D: as the ':' is special symbol
			{
				isAbsolutePath = true;
				break;
			}
		}

		if (isAbsolutePath == true)
		{
			return filePath;
		}
		else
		{
			return std::filesystem::absolute(filePath);
		}
	}

	constexpr std::string ToLower(const std::string& str)
	{
		std::string result = str;
		std::ranges::transform(result, result.begin(), [](unsigned char c) { return std::tolower(c); });
		return result;
	}

	// Case insensitive (doesn't matter upper or lower-case
	constexpr bool FindSuffix(const std::string& name, const std::string& suffixToSearch)
	{
		for (size_t i = name.length(); i > 0; i--)
		{
			if (name[i] == '.')
			{
				std::string suffix = name.substr(i - 2, 2);

				if (ToLower(suffix) == ToLower(suffixToSearch))
				{
					return true;
				}

				break;
			}
		}

		return false;
	}

	inline std::filesystem::path AppendCounterIfAlreadyExist(const std::filesystem::path& fileName)
	{
		const std::string fileNameOG = fileName.string();
		std::string fileNameTest = fileName.string();
		const size_t dotPosition = fileName.string().find_last_of('.');
		const std::string extension = fileName.string().substr(dotPosition);
		int index = 0;

		do
		{
			fileNameTest = fileNameOG.substr(0, dotPosition) + "(" + std::to_string(index++) + ")" + extension;

		} while (std::filesystem::exists(fileNameTest));

		return std::filesystem::path(fileNameTest);
	}

	constexpr void AppendStringsInFront(const std::string& stringToAdd, std::vector<std::string>& originalStrings)
	{
		for (size_t i = 0; i < originalStrings.size(); ++i)
		{
			originalStrings[i] = stringToAdd + originalStrings[i];
		}
	}

	constexpr std::string AppendStringBeforeDot(const std::string& stringToAdd, const std::string& originalString)
	{
		const size_t dotPosition = originalString.rfind('.');

		if (dotPosition != std::string::npos)
		{
			return std::string(originalString.substr(0, dotPosition) + stringToAdd + originalString.substr(dotPosition, originalString.length()));
		}

		return originalString;
	}

	constexpr std::string RemoveSubStringIfExist(const std::string& originalString, const std::string& stringToRemove)
	{
		std::string modifiedString = ToLower(originalString);
		const size_t pos = modifiedString.find(ToLower(stringToRemove));

		if (pos != std::string::npos)
		{
			modifiedString = originalString;
			modifiedString.erase(pos, stringToRemove.length());
			return modifiedString;
		}

		return originalString;
	}

	constexpr std::string ConvertAndAddSpaceToSubStringWithUpperCase(const std::string& string)
    {
        if (string.empty())
        {
            return string;
        }

        std::string newString = string;

		for (char& c : newString)
		{
			if (c == '_')
			{
				c = ' ';
			}
		}

        for (size_t i = 1; i < newString.length(); ++i)
        {
            if (std::isupper(static_cast<unsigned char>(newString[i])))
            {
                newString.insert(i, " ");
                ++i;
            }
        }

        newString[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(newString[0])));

        return newString;
    }
}