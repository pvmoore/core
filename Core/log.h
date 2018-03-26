#pragma once

#include <cstdlib>
#include <cstdarg>
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
	/// Write a list of values to the log.
	template <typename... ARGS>
	static void write(ARGS... rest) {
		std::string value;
		if constexpr (sizeof...(ARGS) > 0) {
			value += writeArgs(rest...);
		} 
		writeChars(value.c_str(), value.size());
	}
	/// Write a formatted string to the log.
	static void format(const char* fmt, ...) {
		char text[512];
		va_list	ap;
		va_start(ap, fmt);
		int count = _vscprintf(fmt, ap) + 1;
		if(count > 1) {
			/// Only write if we have more than just "\0"
			char* buf = (count < _countof(text)) ? text : (char*)malloc(count);
			vsprintf_s(buf, count, fmt, ap);

			writeChars(buf, count-1);

			if(buf != text) free(buf);
		}
		va_end(ap);
	}
	/// Output formatted debug messages
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
private:
	static void writeChars(const char* chars, ulong count) {
		if(!fp) {
			if(0 != _wfopen_s(&fp, L"log.log", L"w")) return;
		}
		fwrite(chars, 1, count, fp);
		fwrite("\n", 1, 1, fp);
		fflush(fp);
	}
	///==============================================================================
	/// Templates for implementing _write_
	///==============================================================================
    static std::string parseArg(const char* str) {
        /// Handle null case
    	return str ? str : std::string();
    }
    static std::string parseArg(const std::string& str) {
        return str;
    }
    static std::string parseArg(int value) {
    	return String::format("%d", value);
    }
    static std::string parseArg(uint value) {
    	return String::format("%u", value);
    }
    static std::string parseArg(ulong value) {
    	return String::format("%llu", value);
    }
    static std::string parseArg(slong value) {
    	return String::format("%lld", value);
    }
    static std::string parseArg(double value) {
        auto s = String::format("%f", value);
        /// Remove extra trailing zeroes.
        while(s.size()>3 && s[s.size() - 1] == '0' && s[s.size() - 2] != '.') {
        	s.erase(s.size() - 1);
        }
        return std::move(s);
    }

	template <typename T>
	static std::string writeArgs(T t) {
		return parseArg(std::forward<T>(t));
	}
	template <typename T, typename... ARGS>
	static std::string writeArgs(T t, ARGS... rest) {
		return parseArg(std::forward<T>(t)) + writeArgs(rest...);
	}
};

} /// core