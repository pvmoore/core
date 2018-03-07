#pragma once

namespace core {

class CharBuffer {
	char* buf      = nullptr;
	uint strLength = 0;
	uint bufLength = 0;

	inline void inflate(int addLength) {
		if(strLength + 1 + addLength > bufLength) {
			uint newLength = (strLength + 1 + addLength) * 2;
			buf = (char*)realloc(buf, newLength);
			bufLength = newLength;
		}
	}
public:
	CharBuffer() {};
	CharBuffer(const char* str) {
		append(std::string(str));
	}
	CharBuffer(const char* str, uint len) { 
		append(std::string(str, len)); 
	}
	~CharBuffer() { if(buf) free(buf); buf = nullptr; strLength = bufLength = 0; }
	
	uint length() const { return strLength; }
	uint capacity() const { return bufLength; }
	bool empty() const { return length()==0; }

	const char* c_str() const { if(buf) return buf; else return ""; }
	void clear() { if(buf) buf[0] = 0; strLength = 0; }
	void pack() { 
		if(!buf) return;
		bufLength = strLength+1;
		buf = (char*)realloc(buf, bufLength);
	}

	bool operator==(const char* rhs) const {
		if(!rhs) return false;
		if(strLength != (uint)strlen(rhs)) return false;
		for(uint i=0; i<strLength; i++) {
			if(buf[i] != rhs[i]) return false;
		}
		return true;
	}
	bool operator==(const std::string& str) const {
		if(strLength!=str.size()) return false;
		if(strLength==0) return true;
		return str==buf;
	}
	friend bool operator==(const char* lhs, const CharBuffer& rhs) {
		return rhs == lhs;
	}
	friend bool operator==(const std::string& lhs, const CharBuffer& rhs) {
		return rhs == lhs;
	}

	bool operator!=(const char* rhs) const {
		return ! operator==(rhs);
	}
	bool operator!=(const std::string& str) const {
		return ! operator==(str);
	}
	friend bool operator!=(const char* lhs, const CharBuffer& rhs) {
		return ! rhs.operator==(lhs);
	}
	friend bool operator!=(const std::string& lhs, const CharBuffer& rhs) {
		return !rhs.operator==(lhs);
	}

	/// Read-only access to chars
	const char operator[](uint index) const { return buf[index]; }

	CharBuffer& append(const std::string& str) {
		if(!str.empty()) {
			uint len = (uint)str.size();
			inflate(len);
			memcpy(buf+strLength, str.c_str(), len);
			strLength += len;
			buf[strLength] = 0;
		}
		return *this;
	}
	CharBuffer& append(const char* str) {
		if(!str) return *this;
		uint len = (uint)strlen(str);
		inflate(len);
		memcpy(buf + strLength, str, len);
		strLength += len;
		buf[strLength] = 0;
		return *this;
	}
	CharBuffer& appendFmt(const char* fmt, ...) {
		char text[1024];							
		va_list	ap;		
		va_start(ap, fmt);	
		/// Count number of characters required
		int count  = _vscprintf(fmt, ap) + 1;
		char* temp = (count < _countof(text)) ? text : (char*)malloc(count);
		vsprintf_s(temp, count, fmt, ap);
		va_end(ap);

		if(count > 1) {
			// we have something worth appending
			uint len = count-1;
			inflate(len);
			memcpy(buf + strLength, temp, len);
			strLength += len;
			buf[strLength] = 0;
		}
		if(temp != text) free(temp);
		return *this;
	}
	bool contains(const char* str) const {
		return indexOf(str) != -1;
	}
	bool contains(const std::string& str) const {
		return contains(str.c_str());
	}
	
	int indexOf(const char* str, uint start=0) const {
		if(!str || start>=strLength) return -1;
		uint len = (uint)strlen(str);
		if(len==0) return -1;
		if(len>strLength-start) return -1;

		char* endP = buf + (strLength-len) + 1;
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
	int indexOf(const std::string& str, uint start = 0) const {
		return indexOf(str.c_str(), start);
	}
	int indexOf(char val, uint start=0) const {
		uint end = strLength;
		char* p = (char*)memchr(buf+start, val, end-start);
		return (int) (p ? p-buf-start : -1);
	}
};


} // namespace core