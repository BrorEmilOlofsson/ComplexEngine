#pragma once
#include "Utility/Math/Matrix4x4.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>

namespace Simple
{
	class Animation final
	{
	public:
		struct Frame
		{
			std::unordered_map<std::string, Matrix4x4f> jointNameToModelSpaceMatrix;
		private:
			const char padding[48] = {};
		};

		std::vector<Frame> frames;
		std::string animationName;
		std::filesystem::path relativePath;

		float duration = 0.0f;
		float framesPerSecond = 0.0f;

		unsigned int length = 0;
	};
}