#pragma once

#include <stdio.h>
#include <string.h>
#include <windows.h>

namespace core {

class File {
	enum Mode { NONE, READ, WRITE, READWRITE };
	const char* path;
	FILE* fp;
	Mode mode;
	const char* flags;

	bool open(const char* desiredFlags, Mode desiredMode) {
		if(mode==desiredMode && strcmp(flags, desiredFlags) == 0) return true;
		if(fp) close();

		fp = fopen(path, desiredFlags);
		if(fp) {
			mode = desiredMode;
			return true;
		} 
		mode = NONE;
		return false;
	}
public:
	File(const char* Path) : path(Path), fp(nullptr), mode(NONE), flags(nullptr) {}
	~File() { close(); }

	bool openRead() {
		return open("rb", READ);
	}
	bool openWrite(bool append=true) {
		return open(append ? "ab" : "wb", WRITE);
	}
	bool openReadWrite(bool append=true) {
		return open(append ? "ab+" : "wb+", READWRITE);
	}
	void close() { 
		if(fp) { 
			fclose(fp); 
			fp = nullptr; 
		}
		mode = NONE; 
		flags = nullptr;
	}
	ulong length() const {
		FILE* f = fopen(path, "r");
		if(f==nullptr) return 0;
		fseek(f, 0, SEEK_END);
		int len = ftell(f);
		fclose(f);
		return len;
	}
	bool exists() const {
		FILE* f = fopen(path, "r");
		if(f==nullptr) return false;
		fclose(f);
		return true;
	}
	bool deleteFile() const {
		return remove(path)!=-1; 
	}
	bool moveTo(const char* newpath) const {
		return rename(path, newpath) == 0;
	}
	bool isDirectory() const {
		const uint dwAttr = GetFileAttributesA(path);
		if(dwAttr !=  INVALID_FILE_ATTRIBUTES) {
			String p = path;
			if((dwAttr & FILE_ATTRIBUTE_DIRECTORY & dwAttr) && p!="." && p!="..") {
				return true;
			}
		}
		return false;
	}
	bool isFile() const {
		return !isDirectory();
	}

	uint read(void* dest, uint unitSize, uint numUnits) {
		if(!openRead()) return 0;

		uint num = (int)fread(dest, unitSize, numUnits, fp);
		return num;
	}
	uint read(CharBuf& buf) {
		if(!openRead()) return 0;

		uint len = (uint)length();
		char* dest = new char[len+1];
		uint num = (uint)fread(dest, 1, len, fp);
		dest[len] = 0;
		buf.append(dest);
		delete[] dest;
		return num;
	}
	uint write(const void* data, uint unitSize, uint numUnits) {
		if(!openWrite(true)) return 0;

		uint num = (int)fwrite(data, unitSize, numUnits, fp);
		fflush(fp);
		return num;
	}
	uint write(const char c) {
		return write(&c, 1, 1);
	}
	uint write(const char* data) {
		return write(data, 1, (uint)strlen(data));
	}
	uint write(CharBuf& str) {
		return write(str.cStr(), 1, str.length());
	}
	uint write(int i) {
		char buf[32];
		sprintf(buf, "%d", i);
		return write(buf);
	}
};

} // namespace core