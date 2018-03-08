#pragma once

#include "_defines.h"

namespace core {
	/// 
	extern "C" {
		/// None of these are  currently used
		void core_memcpy(char* const dest, const char* const src, uint length);
		void core_memset(char* const dest, ubyte value, uint length);
		int core_indexof(const char* const haystack, const char* const needle, uint haystackLength, uint needleLength);
	}
	//__thread const char* errorMessage;
}

#include "types.h"
#include "math.h" 
#include "string.h"
#include "wstring.h"
#include "log.h"
#include "charbuffer.h"
#include "file.h"
#include "filereader.h"
#include "filewriter.h"
#include "mutex.h"
#include "semaphore.h"
#include "thread.h"
#include "map.h"
#include "queue.h"
