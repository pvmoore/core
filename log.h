#pragma once

namespace core {

class Log {
	static FILE* fp;
public:
	~Log() {
		if(fp) fclose(fp);
		fp = nullptr;
	}
	static void write(const char* fmt, ...) {
		if(!fp) {
			if(0 != fopen_s(&fp, "log.log", "w, ccs=UTF-8")) return;
		}

		/// Use this for small strings
		char text[512];
		va_list	ap;
		va_start(ap, fmt);
		/// Count number of characters required
		int count = _vscprintf(fmt, ap) + 1;
		char* buf = (count < _countof(text)) ? text : (char*)malloc(count);
		vsprintf_s(buf, count, fmt, ap);
		va_end(ap);

		fwrite(buf, 1, count, fp);
		fwrite("\n", 1, 1, fp);
		fflush(fp);

		if(buf != text) free(buf);
	}
	/// Output debug messages
	static void dbg(const char* fmt, ...) {
#ifdef _DEBUG
		/// Use this for small strings
		char text[512];
		va_list	ap;

		va_start(ap, fmt);
		int count = _vscprintf(fmt, ap) + 1;
		char* buf = (count < _countof(text)) ? text : (char*)malloc(count);
		vsprintf_s(buf, count, fmt, ap);
		va_end(ap);

		OutputDebugStringA(buf);
		OutputDebugStringA("\n");

		if(buf != text) free(buf);
#endif
	}
};

} /// namespace core