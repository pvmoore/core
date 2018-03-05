#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

namespace core {

class Log {
	const char* fn;
	FILE* fp;
public:
	Log(const char* filename) : fn(filename), fp(NULL) {}
	~Log() { if(fp) fclose(fp); }

	void write(const char* fmt, ...) {
		static const int MAX_CHARS = 256;
		static char text[MAX_CHARS];							
		va_list	ap;									

		va_start(ap, fmt);														
		vsprintf_s(text, MAX_CHARS, fmt, ap);
		va_end(ap);		

		if(!fp) fp = fopen(fn, "w");

		fwrite(text, 1, strlen(text), fp);
		fwrite("\r\n", 1, 2, fp);
		fflush(fp);
	}
};


} // namespace core