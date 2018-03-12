#pragma once

namespace core {

inline void throwOnFileError(errno_t err) {
	if(err == 0) return;
	char buf[256];
	strerror_s(buf, err);
	throw std::runtime_error("File error: " + std::string(buf));
}

class File {
public:
	/// Returns true if the file exists.
	static bool exists(const std::wstring& path) {
		FILE* f;
		if(0 == _wfopen_s(&f, path.c_str(), L"rb")) {
			fclose(f);
			return true;
		}
		return false;
	}
	/// Returns file size or -1 if it cannot be opened.
	static slong size(const std::wstring& path) {
		struct _stat64 s;
		if(0 == _wstat64(path.c_str(), &s)) {
			return s.st_size;
		}
		return -1;
	}
	/// Creates a new file in the user's tmp directory with the given prefix.
	/// Throws an exception if the file cannot be created.
	static std::wstring createTemp(const std::wstring& prefix=L"_tmp") {
		auto filename = _wtempnam(L".", prefix.c_str());
		FILE* f;
		throwOnFileError(_wfopen_s(&f, filename, L"a+"));
		fclose(f);
		auto str = std::wstring(filename);
		free(filename);
		return std::move(str);
	}
	/// Deletes the specified file.
	static bool remove(const std::wstring& path) {
		return _wremove(path.c_str()) == 0;
	}
	/// Renames the specified file.
	static bool rename(const std::wstring& path, const std::wstring& newpath) {
		return ::_wrename(path.c_str(), newpath.c_str()) == 0;
	}
	/// Returns true if the specified path is a directory.
	static bool isDirectory(const std::wstring& path) {
		struct _stat64 s;
		if(0 == _wstat64(path.c_str(), &s)) {
			return (s.st_mode & _S_IFDIR) != 0;
		}
		return false;
	}
	/// Returns true if the specified path is a regular file.
	static bool isFile(const std::wstring& path) {
		struct _stat64 s;
		if(0 == _wstat64(path.c_str(), &s)) {
			return (s.st_mode & _S_IFREG) != 0;
		}
		return false;
	}
	static std::wstring currentDirectory() {
		wchar_t path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, path);
		return std::wstring(path);
	}
	/// Reads an entire text file.
	template<uint BUFFER_SIZE=4096>
	static std::string readText(const std::wstring& filename) {
		FILE* f;
		throwOnFileError(_wfopen_s(&f, filename.c_str(), L"rb"));
		
		auto length = File::size(filename);
		char buf[BUFFER_SIZE+1];
		std::string result;
		result.reserve(length);

		while(length>0) {
			ulong bytes = std::min<ulong>(length, BUFFER_SIZE);
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
	static ulong readBinary(const std::wstring& filename, const T(&array)[N]) {
		FILE* fp;
		throwOnFileError(_wfopen_s(&fp, filename.c_str(), L"rb"));

		auto length        = File::size(filename);
		auto elementSize   = sizeof(T);
		auto bufferSize    = N * elementSize;
		auto elementCount  = std::min<ulong>(length / elementSize, N);
		auto num	       = fread_s((void*)array, bufferSize, elementSize, elementCount, fp);

		fclose(fp);
		return num;
	}
	static void writeText(const std::wstring& filename, const std::string& text) {
		FILE* fp;
		throwOnFileError(_wfopen_s(&fp, filename.c_str(), L"wb"));

		auto len = text.size();
		if(len > 0) {
			fwrite(text.data(), 1, len, fp);
		}
		fclose(fp);
	}
	template<typename T, uint N>
	static void writeBinary(const std::wstring& filename, const T(&array)[N]) {
		FILE* fp;
		throwOnFileError(_wfopen_s(&fp, filename.c_str(), L"wb"));

		fwrite(array, sizeof(N), N, fp);

		fclose(fp);
	}
};

} /// core