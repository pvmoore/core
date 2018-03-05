// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

static_assert(_WIN64==1, "Expecting to be in 64-bit mode");

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

#include <process.h>
//#include <new.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <memory>
#include <atomic>

#include "core.h"
