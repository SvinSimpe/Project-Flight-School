#pragma once

#define WIN32_LEAN_AND_MEAN

#define NOMINMAX
#include <windows.h>
#include <windowsx.h>

#include <crtdbg.h>

#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <memory>
#include "templates.h"
using std::shared_ptr;

#include <functional>
using std::function;


// Game Code Complete - Chapter 12, page 446-447
#if defined(_DEBUG)
#	define GCC_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define GCC_NEW new
#endif