#pragma once

namespace core {

inline void throwOnError(errno_t err) {
	if(err == 0) return;
	char buf[256];
	strerror_s(buf, err);
	throw std::runtime_error("File error: " + std::string(buf));
}

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
		return std::move(str);
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
	/// Reads an entire text file.
	template<uint BUFFER_SIZE=4096>
	static std::string readText(const std::string& filename) {
		FILE* f;
		throwOnError(fopen_s(&f, filename.c_str(), "rb"));
		
		auto length = File::size(filename);
		char buf[BUFFER_SIZE+1];
		std::string result;
		result.reserve(length);

		while(length>0) {
			ulong bytes = Math::min<ulong>(length, BUFFER_SIZE);
			ulong num   = fread_s(buf, BUFFER_SIZE, 1, bytes, f);
			length -= num;

			buf[num] = 0;
			result += buf;
		}
		fclose(f);
		return std::move(result) ;
	}
	/// Reads an entire binary file into _array_.
	/// Returns the number of bytes read.
	/// Assumes you already know the file size or only want a part of the file.
	template<typename T, uint N>
	static ulong readBinary(const std::string& filename, const T(&array)[N]) {
		FILE* f;
		throwOnError(fopen_s(&f, filename.c_str(), "rb"));

		auto length       = File::size(filename);
		ulong bytesToRead = Math::min<ulong>(length, N);
		ulong num	      = fread_s((void*)array, N, 1, bytesToRead, f);

		fclose(f);
		return num;
	}
	static void writeText(const std::string& filename, const std::string& text) {
		// todo
	}
	template<typename T, uint N>
	static void writeBinary(const std::string& filename, const T(&array)[N]) {
		// todo
	}
};

} /// core