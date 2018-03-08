#pragma once

#include <string>
#include <vector>
#include <cstdarg>

namespace core {

class String {
public:
	inline static bool startsWith(const std::string& s, const std::string& needle) {
		return !needle.empty() && s.size() >= needle.size() &&
			0 == strncmp(s.data(), needle.data(), needle.size());
	}
	inline static bool endsWith(const std::string& s, const std::string& needle) {
		return !needle.empty() && s.size() >= needle.size() &&
			0 == strncmp(&s[s.size() - needle.size()], needle.data(), needle.size());
	}
	inline static bool contains(const std::string& s, const std::string& needle) {
		return !needle.empty() && s.size() >= needle.size() &&
			s.find(needle) != std::string::npos;
	}
	/// Removes whitespace from end of string (Copy-on-write)
	static std::string trimRight(const std::string& s) {
		if(s.empty()) return s;
		slong e = s.size() - 1;
		while(e >= 0 && s[e]<33) { e--; }
		if(e<0) return "";
		return e == s.size() - 1 ? s : std::string(s.begin(), s.begin() + e + 1);
	}
	/// Removes whitespace from start of string (Copy-on-write)
	static std::string trimLeft(const std::string& s) {
		if(s.empty()) return s;
		int e = 0;
		while(e<s.size() && s[e]<33) { e++; }
		if(e == 0) return s;
		return e == s.size() - 1 ? s : std::string(s.begin() + e, s.end());
	}
	/// Removes whitespace from both sides of string (Copy-on-write)
	static std::string trimBoth(const std::string& s) {
		if(s.empty()) return s;
		if(s[0]<33) {
			auto s2 = trimLeft(s);
			return std::move(trimRight(s2));
		}
		if(s[s.size() - 1]<33) return std::move(trimRight(s));
		return s;
	}
	/// Splits a string using whitespace as separator.
	static std::vector<std::string> split(const std::string& s) {
		std::vector<std::string> array;
		if(s.empty()) return array;
		int pos = 0;
		int start = -1;
		while(pos<s.size()) {
			start = -1;
			/// skip separator
			while(pos < s.size() && s[pos] < 33) pos++;
			if(pos == s.size()) break;

			/// start collecting the next token here
			start = pos;

			while(pos<s.size() && s[pos]>32) pos++;
			if(pos == s.size()) break;

			array.push_back(std::string(s.begin() + start, s.begin() + pos));
		}
		if(start>-1 && pos>start) array.push_back(std::string(s.begin() + start, s.begin() + pos));
		return array;
	}
	/// splits a string using sep as separator.
	static std::vector<std::string> split(const std::string& s, char sep) {
		std::vector<std::string> array;
		if(s.empty()) return array;
		int pos = 0;
		int start = 0;
		while(pos<s.size()) {
			start = pos;
			while(pos<s.size() && s[pos] != sep) pos++;

			array.push_back(std::string(s.begin() + start, s.begin() + pos));
			pos++;
		}
		if(pos > 0 && s[pos - 1] == sep) {
			/// There is a separator at the end of the string
			array.push_back("");
		}
		return array;
	}
	/// Concatenate strings into a single comma separated string
	static std::string toString(const std::vector<std::string>& array, std::string delim = ", ") {
		std::string s;
		for(int i = 0; i<array.size(); i++) {
			if(i>0) s += delim;
			s += array[i];
		}
		return std::move(s);
	}
	inline static int toInt(const std::string& s) {
		return atoi(s.c_str());
	}
	inline static float toFloat(const std::string& s) {
		return (float)atof(s.c_str());
	}
	static std::string format(const char* fmt, ...) {
		char text[256];
		va_list	ap;
		std::string str;
		va_start(ap, fmt);
		int count = _vscprintf(fmt, ap) + 1;
		if(count > 1) {
			char* buf = (count < _countof(text)) ? text : (char*)malloc(count);
			vsprintf_s(buf, count, fmt, ap);
			str = buf;
			if(buf != text) free(buf);
		}
		va_end(ap);
		return std::move(str);
	}
};

} /// core