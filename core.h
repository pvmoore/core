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

	const sint MAX_SINT	    = 0x7fffffff;
	const sint MIN_SINT	    = 0x80000000;
	const uint MAX_UINT		= 0xffffffff;
	const uint MIN_UINT		= 0x00000000;
	const slong MAX_SLONG   = 0x7fffffffffffffff;
	const slong MIN_SLONG   = 0x8000000000000000;
	const ulong MAX_ULONG	= 0xffffffffffffffff;
	const ulong MIN_ULONG	= 0x0000000000000000;

	extern "C" {
		//uint core_parse_uint(char*, uint);
		//uint core_div_10(uint);
		//void core_div_mod_10(uint*, uint*, uint);
		//bool core_strcmp(const char* str1, const char* str2, uint strlen);
		//void core_memcpy(char* const dest, const char* const src, uint length);

		uint core_div_10(uint value);
		void core_memcpy(char* const dest, const char* const src, uint length);
		void core_memset(char* const dest, ubyte value, uint length);
		int core_indexof(const char* const haystack, const char* const needle, uint haystackLength, uint needleLength);
	}

	//__thread const char* errorMessage;
}

#include "log.h"
#include "maths.h" 
#include "String.h"
#include "CharBuf.h"
#include "LinkedList.h"
#include "File.h"
#include "Stream.h"
#include "mutex.h"
#include "semaphore.h"
#include "threads.h"
