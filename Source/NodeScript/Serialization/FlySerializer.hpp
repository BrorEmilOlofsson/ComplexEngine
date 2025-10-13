#pragma once
#include "../FlyDefines.hpp"
#include <string>
#include <External/nlohmann/json.hpp>

namespace FLY_NAMESPACE
{
	class Class;
	class Struct;
	class DataType;

	namespace Internal
	{
		void SaveDataType(const DataType& dataType, const std::filesystem::path& filePath);
		void LoadDataType(const nlohmann::json& jsonData);
		void LoadDataType(const nlohmann::json& jsonData, DataType& dataType);
		//void SaveStruct(const Struct& aStruct, std::string_view filePath);
		//void LoadStruct(const nlohmann::json& jsonData);
		//void LoadStruct(const nlohmann::json& jsonData, Struct& aStruct);
		void SaveClass(const Class& c, const std::filesystem::path& filePath);
		void LoadClass(const nlohmann::json& jsonData);
		void LoadClass(const nlohmann::json& jsonData, Class& c);

		void LoadAllFlyFiles(const std::filesystem::path& filePath);

		void CreateCopyOfClass(const Class& c, const std::filesystem::path& filePath, std::string_view copyName);

		void SaveCustomEvents(const std::filesystem::path& filePath);
		void LoadCustomEvents(const std::filesystem::path& filePath);
	}


}