#pragma once

namespace core {


class Stream {
public:
	Stream() {}
	virtual ~Stream() {}
};

class InputStream : public Stream {
public:
	InputStream() {}
	virtual ~InputStream() {}
	virtual int read() = 0;
};

class OutputStream : public Stream {
public:
	OutputStream() {}
	virtual ~OutputStream() {}
	virtual int write(int byte) = 0;
};

class FileOutputStream : public OutputStream {
public:
	FileOutputStream(File& file, bool append) {}
	virtual ~FileOutputStream() {}
	virtual int write(int byte) {
		return -1;
	}
};

} // namespace code