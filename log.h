#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <Windows.h>

namespace core {

class Log {
	static FILE* fp;
public:
	~Log() {
		close();
	}
	static void close() {
		if(fp) fclose(fp);
		fp = nullptr;
	}
	static void write(const char* fmt, ...) {
		if(!fp) {
			if(0 != fopen_s(&fp, "log.log", "w")) return;
		}

		/// Use this for small strings
		char text[512];
		va_list	ap;
		va_start(ap, fmt);
		/// Count number of characters required
		int count = _vscprintf(fmt, ap) + 1;
		if(count > 1) {
			// Only write if we have more than just "\0"
			char* buf = (count < _countof(text)) ? text : (char*)malloc(count);
			vsprintf_s(buf, count, fmt, ap);

			fwrite(buf, 1, count - 1, fp);
			fwrite("\n", 1, 1, fp);
			fflush(fp);

			if(buf != text) free(buf);
		}
		va_end(ap);
	}
	/// Output debug messages
	static void dbg(const char* fmt, ...) {
#ifdef _DEBUG
		/// Use this for small strings
		char text[512];
		va_list	ap;

		va_start(ap, fmt);
		int count = _vscprintf(fmt, ap) + 1;
		if(count > 1) {
			// Only write if we have more than just "\0"
			char* buf = (count < _countof(text)) ? text : (char*)malloc(count);
			vsprintf_s(buf, count, fmt, ap);

			OutputDebugStringA(buf);
			OutputDebugStringA("\n");

			if(buf != text) free(buf);
		}
		va_end(ap);
#endif
	}
};

} /// namespace core