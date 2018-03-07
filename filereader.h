#pragma once

namespace core {

template<int BUFFER_SIZE = 4096>
class FileReader {
	FILE* fp = nullptr;
	char buffer[BUFFER_SIZE];
	int bufferPos = 0;
	slong currentPos = 0;
	slong filePos = 0;
	//static_assert(BUFFER_SIZE>=1024);
public:
	const std::string path;
	slong size = -1;

	FileReader(const std::string& path) : path(path) {
		throwOnError(fopen_s(&fp, path.c_str(), "rb"));
		size = File::size(path);
	}
	~FileReader() {
		if(fp) {
			fclose(fp);
			fp = nullptr;
		}
	}
	bool eof() const {
		return currentPos >= size;
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
				line += string(buffer, start, count);
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
		slong len = Math::min<slong>(BUFFER_SIZE, size - filePos);
		//printf("fillBuffer()\n");
		//printf("\treading %lld bytes into buffer\n", len);

		auto num = fread((void*)buffer, 1, len, fp);
		bufferPos = 0;
		filePos += num;
		//printf("\tfread %llu bytes\n", num);
		//printf("\tfilePos is now %lld\n", filePos);
	}
};


} /// core