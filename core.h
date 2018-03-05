#pragma once

namespace core {

	typedef signed char         sbyte;
	typedef unsigned char       ubyte;
	typedef signed short        sshort;
	typedef unsigned short      ushort;
	typedef signed int          sint;
	typedef unsigned int        uint;
	typedef signed __int64      slong;
	typedef unsigned __int64    ulong; 

	constexpr sint MAX_SINT	    = 0x7fffffff;
	constexpr sint MIN_SINT	    = 0x80000000;
	constexpr uint MAX_UINT		= 0xffffffff;
	constexpr uint MIN_UINT		= 0x00000000;
	constexpr slong MAX_SLONG   = 0x7fffffffffffffff;
	constexpr slong MIN_SLONG   = 0x8000000000000000;
	constexpr ulong MAX_ULONG	= 0xffffffffffffffff;
	constexpr ulong MIN_ULONG	= 0x0000000000000000;

	/// 
	extern "C" {
		void core_memcpy(char* const dest, const char* const src, uint length);
		void core_memset(char* const dest, ubyte value, uint length);
		int core_indexof(const char* const haystack, const char* const needle, uint haystackLength, uint needleLength);
	}

	//__thread const char* errorMessage;
}

#pragma warning(disable : 4101) /// unused variable

#include "log.h"
#include "math.h" 
#include "string.h"
#include "wstring.h"
#include "CharBuf.h"
#include "file.h"
#include "mutex.h"
#include "semaphore.h"
#include "threads.h"
#include "map.h"
#include "queue.h"
