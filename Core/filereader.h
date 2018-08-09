#pragma once

#include "_defines.h"
#include <algorithm>

namespace core {

template<int BUFFER_SIZE = 4096>
class FileReader final {
	FILE* fp = nullptr;
	char buffer[BUFFER_SIZE];
	int bufferPos = 0;
	slong currentPos = 0;
	slong filePos = 0;
	static_assert(BUFFER_SIZE>=16); // very small buffer but small buffer is useful for testing
public:
	const std::wstring path;
	slong size = -1;

	FileReader(const std::wstring& path) : path(path) {
		throwOnFileError(_wfopen_s(&fp, path.c_str(), L"rb"), path);
		size = File::size(path);
	}
	~FileReader() {
		if(fp) {
			fclose(fp);
			fp = nullptr;
		}
	}
	bool eof() const { return currentPos >= size; }
    ulong pos() const { return currentPos; }

	ubyte readByte() {
        ubyte v;
        read(&v, 1);
        return v;
	}
	short readShort() {
        short v;
        read(&v, 2);
        return v;
	}
	int readInt() {
        int v;
        read(&v, 4);
        return v;
	}
	slong readLong() {
        slong v;
        read(&v, 8);
        return v;
	}
	float readFloat() {
		float f;
        read(&f, 4);
        return f;
	}
	double readDouble() {
		double d;
        read(&d, 8);
        return d;
	}
    void read(char* dest, ulong numBytes) {
        assert(dest);
        if(numBytes > 0) {
            if(currentPos == 0) {
                fillBuffer();
            }
            while(numBytes > 0 && !eof()) {
                if(numBytes > BUFFER_SIZE - bufferPos) {
                    /// Partial copy
                    auto num = BUFFER_SIZE - bufferPos;
                    memcpy(dest, buffer + bufferPos, num);
                    currentPos += num;
                    bufferPos += num;
                    numBytes -= num;
                    dest += num;
                    fillBuffer();
                } else {
                    /// Copy remainder
                    memcpy(dest, buffer+bufferPos, numBytes);
                    currentPos += numBytes;
                    bufferPos += numBytes;
                    numBytes = 0;
                } 
            }
        }
    }
	/// Assumes /n (UNIX) or /r/n (Windows) line endings.
	std::string readLine() {
		if(eof()) return "";

		std::string line;
		int start = bufferPos;

		/// Pre-fill our buffer
		if(currentPos == 0) {
			fillBuffer();
		}

		const auto appendToLine = [&](int end) {
			/// remove any /r from start and end
			if(start>0 && buffer[start - 1] == 13) start++;
			if(end>0 && buffer[end - 1] == 13) end--;

			int count = (end - start);
			if(count > 0) {
				line += std::string(buffer, start, count);
			}
		};

		while(true) {
			if(currentPos >= size) {
				/// Add the last bit
				appendToLine(bufferPos);
				break;
			}
			if(bufferPos == BUFFER_SIZE) {
				appendToLine(bufferPos);
				start = 0;
				fillBuffer();
			}

			char ch = buffer[bufferPos];
			bufferPos++;
			currentPos++;

			if(ch == 10) {
				appendToLine(bufferPos-1);
				break;
			}
		}
		return std::move(line);
	}
private:
	void fillBuffer() {
		slong len = std::min<slong>(BUFFER_SIZE, size - filePos);
		auto num  = fread((void*)buffer, 1, len, fp);
		bufferPos = 0;
		filePos += num;
	}
};

} /// core