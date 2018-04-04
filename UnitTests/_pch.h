// pre-compiled header for rarely-changing includes
#pragma once

static_assert(_WIN64 == 1, "Expecting to be in 64-bit mode");

/// Target Windows 7 and above  
#define WINVER		 _WIN32_WINNT_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <SDKDDKVer.h>

#define NOMINMAX	
#define NOMENUS
#define NODRAWTEXT
#define NOCTLMGR
#define NOCLIPBOARD
#define NODRAWTEXT
#define NOTEXTMETRIC
#define NOGDI
#define NOBITMAP
#define NOMCX	
#define NOSERVICE	
#define NOHELP	
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <crtdbg.h>
#include <process.h>

#define _ALLOW_RTCc_IN_STL
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cassert>

/// std namespace files
#include <memory>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <algorithm>
#include <functional>
