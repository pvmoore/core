#pragma once

#include "_defines.h"

namespace core {
	/// 
	extern "C" {
		void core_memcpy(char* const dest, const char* const src, uint length);
		void core_memset(char* const dest, ubyte value, uint length);
		int core_indexof(const char* const haystack, const char* const needle, uint haystackLength, uint needleLength);
	}
	//__thread const char* errorMessage;
}

#include "log.h"
#include "math.h" 
#include "string.h"
#include "wstring.h"
#include "CharBuf.h"
#include "file.h"
#include "mutex.h"
#include "semaphore.h"
#include "thread.h"
#include "map.h"
#include "queue.h"
