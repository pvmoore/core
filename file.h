#pragma once

namespace core {

inline void throwOnError(errno_t err) {
	if(err == 0) return;
	char buf[256];
	strerror_s(buf, err);
	throw std::runtime_error("File error: " + std::string(buf));
}
//==================================================================================== FileReader
template<int BUFFER_SIZE>
class FileReader {
	FILE* fp    = nullptr;
	char buffer[BUFFER_SIZE];
	int bufferPos = 0;
	slong filePos = 0;
	static_assert(BUFFER_SIZE>=1024);
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
		return size<=0 || filePos >= size;
	}
	std::string readLine() const {
		if(eof()) return ""; 

		string line;

		//for(int i = bufferPos; i < BUFFER_SIZE; i++) {

		//}
	
		// todo

		

		return "";
	}
private:
	void fillBuffer() {
		slong len = Math::min<slong>(BUFFER_SIZE, size - filePos);
		printf("len=%lld\n", len);

		auto num = fread((void*)buffer, 1, len, fp);
		bufferPos = 0;
		filePos  += num;
	}
};
//==================================================================================== FileWriter
class FileWriter {
	FILE* fp = nullptr;
public:
	void writeconst(const std::string& fmt, ...) {
		//uint num = (int)fwrite(data, unitSize, numUnits, fp);
	}
	void flush() {
		fflush(fp);
	}
};
//==================================================================================== File
class File {
public:
	/// Returns true if the file exists.
	static bool exists(const std::string& path) {
		FILE* f;
		if(0 == fopen_s(&f, path.c_str(), "rb")) {
			fclose(f);
			return true;
		}
		return false;
	}
	/// Returns file size or -1 if it cannot be opened.
	static slong size(const std::string& path) {
		struct _stat64 s;
		if(0 == _stat64(path.c_str(), &s)) {
			return s.st_size;
		}
		return -1;
	}
	/// Creates a new file in the user's tmp directory with the given prefix.
	/// Throws an exception if the file cannot be created.
	static std::string createTemp(const std::string& prefix) {
		auto filename = _tempnam(".", prefix.c_str());
		FILE* f;
		throwOnError(fopen_s(&f, filename, "a+"));
		fclose(f);
		auto str = std::string(filename);
		free(filename);
		return str;
	}
	/// Deletes the specified file.
	static bool remove(const std::string& path) {
		return ::remove(path.c_str()) == 0;
	}
	/// Renames the specified file.
	static bool rename(const std::string& path, const std::string& newpath) {
		return ::rename(path.c_str(), newpath.c_str()) == 0;
	}
	/// Returns true if the specified path is a directory.
	static bool isDirectory(const std::string& path) {
		struct _stat64 s;
		if(0 == _stat64(path.c_str(), &s)) {
			return (s.st_mode & _S_IFDIR) != 0;
		}
		return false;
	}
	/// Returns true if the specified path is a regular file.
	static bool isFile(const std::string& path) {
		struct _stat64 s;
		if(0 == _stat64(path.c_str(), &s)) {
			return (s.st_mode & _S_IFREG) != 0;
		}
		return false;
	}
};

} /// namespace core