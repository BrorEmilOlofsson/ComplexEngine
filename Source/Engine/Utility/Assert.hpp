#pragma once 
#include <cassert>

#ifdef _DEBUG

#define ASSERT(expression) ((void)(                                                       \
            (!!(expression)) ||                                                               \
            (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)) \
        )

#define ASSERT_NEW(expression, message) ((void)(                                                       \
            (!!(expression)) ||                                                               \
            (_wassert(_CRT_WIDE(message), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)) \
        )

#else 

#define ASSERT(expression) ((void)0)
#define ASSERT_NEW(expression, message) ((void)0)

#endif