#include "_pch.h"
#include "../Core/core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;
using std::wstring;

namespace UnitTests {

TEST_CLASS(filewriter) {
public:
	static wstring tmpFile;
	TEST_CLASS_INITIALIZE(ClassInitialize) {
		tmpFile = File::createTemp();
		Logger::WriteMessage((L"Creating tmp file '%s'" + tmpFile).c_str());
	}
	TEST_CLASS_CLEANUP(ClassCleanup) {
		Logger::WriteMessage((L"removing " + tmpFile).c_str());
		File::remove(tmpFile);
	}

	TEST_METHOD(writeFmt) {
		{
			FileWriter w{tmpFile, false};
			printf("writeFmt(const char*,...)\n");
			w.writeFmt("%s %d", "Hello", 3);
			Assert::IsTrue(w.bytesWritten == 7);
		}
		Assert::IsTrue(File::size(tmpFile) == 7);
		Assert::IsTrue(File::readText(tmpFile) == "Hello 3");
	}
	TEST_METHOD(append_rather_than_create_new_file) {
		{
			FileWriter w{tmpFile, false};
			w.writeFmt("%d", 7);
			Assert::IsTrue(w.bytesWritten == 1);
		}
		{
			// append set to true
			FileWriter w{tmpFile, true};
			w.writeFmt("%d", 7);
			Assert::IsTrue(w.bytesWritten == 1);
		}
		Assert::IsTrue(File::size(tmpFile) == 2);
		Assert::IsTrue(File::readText(tmpFile) == "77");
	}
	TEST_METHOD(write_ubyte) {
		{
			FileWriter w{tmpFile, false};
			w.write((ubyte)127);
			Assert::IsTrue(w.bytesWritten == 1);
		}
		Assert::IsTrue(File::size(tmpFile) == 1);
		ubyte data[1];
		File::readBinary(tmpFile, data);
		Assert::IsTrue(data[0] == 127);
	}
	TEST_METHOD(write_ushort) {
		{
			FileWriter w{tmpFile, false};
			w.write((ushort)0xf7a0);
			Assert::IsTrue(w.bytesWritten == 2);
		}
		Assert::IsTrue(File::size(tmpFile) == 2);
		ushort data[1];
		File::readBinary(tmpFile, data);
		Assert::IsTrue(data[0] == 0xf7a0);
	}
	TEST_METHOD(write_uint) {
		{
			FileWriter w{tmpFile, false};
			w.write(0x80010203);
			Assert::IsTrue(w.bytesWritten == 4);
		}
		Assert::IsTrue(File::size(tmpFile) == 4);
		uint data3[1];
		File::readBinary(tmpFile, data3);
		Assert::IsTrue(data3[0] == 0x80010203);
	}
	TEST_METHOD(write_ulong) {
		{
			FileWriter w{tmpFile, false};
			w.write(0x80010203'fa00ee44);
			Assert::IsTrue(w.bytesWritten == 8);
		}
		Assert::IsTrue(File::size(tmpFile) == 8);
		ulong data4[1];
		File::readBinary(tmpFile, data4);
		Assert::IsTrue(data4[0] == 0x80010203'fa00ee44);
	}
	TEST_METHOD(write_float) {
		{
			FileWriter w{tmpFile, false};
			w.write(3.3f);
			Assert::IsTrue(w.bytesWritten == 4);
		}
		Assert::IsTrue(File::size(tmpFile) == 4);
		float data5[1];
		File::readBinary(tmpFile, data5);
		Assert::IsTrue(data5[0] == 3.3f);
	}
	TEST_METHOD(write_double) {
		{
			FileWriter w{tmpFile, false};
			w.write(3.7);
			Assert::IsTrue(w.bytesWritten == 8);
		}
		Assert::IsTrue(File::size(tmpFile) == 8);
		double data6[1];
		File::readBinary(tmpFile, data6);
		Assert::IsTrue(data6[0] == 3.7);
	}
	TEST_METHOD(write_char_ptr) {
		{
			FileWriter w{tmpFile, false};
			w.write("hello");
			Assert::IsTrue(w.bytesWritten == 5);
		}
		Assert::IsTrue(File::size(tmpFile) == 5);
		Assert::IsTrue(File::readText(tmpFile) == "hello");
	}
	TEST_METHOD(write_string) {
		{
			FileWriter w{tmpFile, false};
			w.write(string("hello"));
			Assert::IsTrue(w.bytesWritten == 5);
		}
		Assert::IsTrue(File::size(tmpFile) == 5);
		Assert::IsTrue(File::readText(tmpFile) == "hello");
	}
};

wstring filewriter::tmpFile;

}