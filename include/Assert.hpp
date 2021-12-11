#pragma once


#ifdef _DEBUG
#include <assert.h>
#define ASSERT(expression) assert(expression);
#else
#define ASSERT(expression) 
#endif

