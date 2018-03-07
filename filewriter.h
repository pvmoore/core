#pragma once

namespace core {

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


} /// core