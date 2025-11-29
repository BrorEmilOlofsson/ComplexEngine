#pragma once 
#include <cassert>

#define ASSERT(expression) ((void)(                                                       \
            (!!(expression)) ||                                                               \
            (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)) \
        )