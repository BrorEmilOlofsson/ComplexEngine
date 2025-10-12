#pragma once
#include <print>

#define CHECK(condition) \
	if (!(condition)) { \
		std::fprintf(stderr, "Check failed: %s, in file %s, line %d\n", #condition, __FILE__, __LINE__); \
		std::abort(); \
	}