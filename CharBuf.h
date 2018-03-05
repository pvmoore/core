#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

namespace core {

/// TODO - handle non-ascii data
class CharBuf {
	const static uint GROWTH = 8;
	char* buf;
	uint strLength;
	uint bufLength;

	inline void inflate(int addLength) {
		if(strLength + 1 + addLength > bufLength) {
			uint newLength = strLength + 1 + addLength + GROWTH;
			buf = (char*)realloc(buf, newLength);
			bufLength = newLength;
		}
	}
public:
	CharBuf()							  : buf(nullptr), strLength(0), bufLength(0) {}
	CharBuf(const char* str, uint len=-1) : buf(nullptr), strLength(0), bufLength(0) { append(str, len); }
	CharBuf(CharBuf& src)				  : buf(nullptr), strLength(0), bufLength(0) { append(src.cStr(), src.length()); }

	~CharBuf() { if(buf) free(buf); buf = nullptr; strLength = bufLength = 0; }
	
	uint length() const { return strLength; }
	const char* cStr() const { if(buf) return buf; else return ""; }
	void clear() { if(buf) buf[0] = 0; strLength = 0; }
	void pack() { 
		if(!buf) return;
		bufLength = strLength+1;
		buf = (char*)realloc(buf, bufLength);
	}

	bool operator==(const char* rhs) const {
		uint len = (uint)strlen(rhs);
		if(strLength != len) return false;
		for(uint i=0; i<strLength; i++) {
			if(buf[i] != rhs[i]) return false;
		}
		return true;
	}

	bool operator==(CharBuf& rhs) const {
		if(strLength != rhs.bufLength) return false;
		for(uint i=0; i<strLength; i++) {
			if(buf[i] != rhs.buf[i]) return false;
		}
		return true;
	}

	char& operator[](int index) { return buf[index]; }

	CharBuf& operator+=(const char* str) { return append(str, (int)strlen(str)); }
	CharBuf& operator+=(int i) { return append(i); }
	CharBuf& operator+=(uint i) { return append(i); }
	CharBuf& operator+=(slong i) { return append(i); }
	CharBuf& operator+=(ulong i) { return append(i); }
	CharBuf& operator+=(double d) { return append(d); }
	CharBuf& append(const CharBuf& b) { return append(b.cStr(), b.length()); }
	CharBuf& append(const char* str, int len=-1) {
		if(len==-1) len = (int)strlen(str);
		if(len>0) {
			inflate(len);
			memcpy(buf+strLength, str, len);
			strLength += len;
			buf[strLength] = 0;
		}
		return *this;
	}
	CharBuf& append(int i) {
		static char temp[20];
		int len = sprintf(temp, "%i", i);
		return append(temp, len);
	}
	CharBuf& append(uint i) {
		static char temp[20];
		int len = sprintf(temp, "%u", i);
		return append(temp, len);
	}
	CharBuf& append(slong i) {
		if(i > MAX_SINT) assert(0 && "unsupported");
		return append((int)i);
	}
	CharBuf& append(ulong i) {
		if(i > 0xffffffff) assert(0 && "unsupported");
		return append((uint)i);
	}
	CharBuf& append(double d, uint dp=-1) {
		static char temp[30];
		int len;
		if(dp==-1) {
			len = sprintf(temp, "%f", d);
		} else {
			len = sprintf(temp, "%.*f", dp, d);
		}
		return append(temp, len);
	}
	CharBuf& appendFmt(const char* fmt, ...) {
		static const int MAX_CHARS = 1024;
		static char text[MAX_CHARS];							
		va_list	ap;									
		va_start(ap, fmt);														
		int len = vsprintf_s(text, MAX_CHARS, fmt, ap);
		va_end(ap);	
		return append(text, len);
	}

	bool contains(CharBuf& str) const {
		return indexOf(str) != -1;
	}
	bool contains(const char* str) const {
		return indexOf(str) != -1;
	}
	
	int indexOf(CharBuf& b, int start=0, int end=-1) const { return indexOf(b.cStr(), start, end); }
	int indexOf(const char* str, int start=0, int end=-1) const {
		if(end==-1) end = strLength;
		uint len   = (uint)strlen(str);
		char* endP = buf + end - len + 1;
		char* p	   = buf+start;
		char ch1   = str[0];

		while(p != endP) {
			if(*p == ch1) {
				if(memcmp(p, str, len)==0) return (int) (p-buf-start);
			}
			p++;
		}
		return -1;
	}

	int indexOf(char val, int start=0, int end=-1) const {
		if(end==-1) end = strLength;
		char* p = (char*)memchr(buf+start, val, end-start);
		return (int) (p ? p-buf-start : -1);
	}

	/*uint getTokens(LinkedList<const char*> list) const {
		if(!buf) return 0;
		uint num = 0;

		for(uint i = 0; i<bufLength; i++) {
			
		}

		return num;
	}*/

	friend bool operator==(const char* lhs, CharBuf& rhs) {
		return rhs==lhs;
	}
};


} // namespace core