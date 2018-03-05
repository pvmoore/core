#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

namespace core {

class Log {
	const char* fn = nullptr;
	FILE* fp;
public:
	Log(const char* filename) : fn(filename) {}
	~Log() { if(fp) fclose(fp); fp = nullptr; }

	void write(const char* fmt, ...) {
		if(!fp) {
			if(0 != fopen_s(&fp, fn, "w")) return;
		}

		char text[256];							
		va_list	ap;									

		va_start(ap, fmt);														
		vsprintf_s(text, fmt, ap);
		va_end(ap);		

		fwrite(text, 1, strlen(text), fp);
		fwrite("\r\n", 1, 2, fp);
		fflush(fp);
	}
};


} // namespace core