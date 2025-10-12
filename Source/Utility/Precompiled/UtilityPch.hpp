#pragma once

#ifdef _WIN32
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#define NOMINMAX
#endif

#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <External/profiler.h>
