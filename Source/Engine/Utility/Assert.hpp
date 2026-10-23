#pragma once 
#include <cassert>

#define ASSERT(expression, message) ((void)(                                                       \
            (!!(expression)) ||                                                               \
            (_wassert(_CRT_WIDE(message), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)) \
        )

#ifdef _DEBUG



#define DEBUG_ASSERT ASSERT

#else 

#define DEBUG_ASSERT(expression, message) ((void)0)

#endif