#pragma once
#include <filesystem>

class SimpleNodeScript final
{
public:
	SimpleNodeScript();
	~SimpleNodeScript();

	void Init();

	static inline std::filesystem::path ASSET_FILE_PATH = std::filesystem::path(SIMPLE_DIR_ASSETS) / "FlyClasses";

};
