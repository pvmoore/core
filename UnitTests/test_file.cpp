#include "_pch.h"
#include "../Core/core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;

namespace UnitTests {

TEST_CLASS(file) {
public:
	TEST_METHOD(exists) {
		Assert::IsTrue(File::exists("../../LICENSE"));
	}
	TEST_METHOD(size) {
		Assert::IsTrue(File::size("../../LICENSE") == 1089);
	}
	TEST_METHOD(createTemp) {
		string filename = File::createTemp("_temp");
		Assert::IsTrue(File::exists(filename));
		Logger::WriteMessage(String::format("File::createTemp -> Created temp file '%s'\n", filename.c_str()).c_str());
		File::remove(filename);
	}
	TEST_METHOD(rename) {
		string filename = File::createTemp("_temp");
		Assert::IsTrue(File::exists(filename));
		string filename2 = filename + "2";
		File::rename(filename, filename2);
		Assert::IsFalse(File::exists(filename));
		Assert::IsTrue(File::exists(filename2));
		File::remove(filename2);
	}
	TEST_METHOD(remove) {
		string filename = File::createTemp("_temp");
		Assert::IsTrue(File::exists(filename));
		File::remove(filename);
		Assert::IsFalse(File::exists(filename));
	}
	TEST_METHOD(isDirectory) {
		Assert::IsTrue(File::isDirectory("../Debug"));
		Assert::IsTrue(File::isDirectory("."));
		Assert::IsTrue(File::isDirectory(".."));

		Assert::IsFalse(File::isDirectory("_pch.h"));
		Assert::IsFalse(File::isDirectory(""));
	}
	TEST_METHOD(isFile) {
		Assert::IsTrue(File::isFile("../../LICENSE"));

		Assert::IsFalse(File::isFile("../Debug"));
		Assert::IsFalse(File::isFile("."));
	}
	TEST_METHOD(readText) {
		auto text = File::readText("../../LICENSE");
		Assert::IsTrue(text.size() == 1089);
		Assert::IsTrue(text.find("MIT License") == 0);
		Assert::IsTrue(text.find("SOFTWARE.") == 1089 - 11); // assumes it ends in \r\n
	}
	TEST_METHOD(readBinary) {
		char array[1089];
		auto num = File::readBinary("../../LICENSE", array);
		Assert::IsTrue(num == 1089);
		Assert::IsTrue(array[0] == 'M');
		Assert::IsTrue(array[1] == 'I');
		Assert::IsTrue(array[2] == 'T');

		char array2[16];
		auto num2 = File::readBinary("../../LICENSE", array2);
		Assert::IsTrue(num2 == 16);
		Assert::IsTrue(array2[0] == 'M');
		Assert::IsTrue(array2[1] == 'I');
		Assert::IsTrue(array2[2] == 'T');
	}
	TEST_METHOD(writeText) {
		auto tmpFilename = File::createTemp();
		string text = "I am some text";
		File::writeText(tmpFilename, text);
		Assert::IsTrue(File::size(tmpFilename) == text.size());
		Assert::IsTrue(File::readText(tmpFilename) == text);
		File::remove(tmpFilename);
	}
	TEST_METHOD(writeBinary) {
		auto tmpFilename = File::createTemp();
		uint data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
		File::writeBinary(tmpFilename, data);
		Assert::IsTrue(File::size(tmpFilename) == _countof(data) * sizeof(uint));
		uint data2[8];
		File::readBinary(tmpFilename, data2);
		for(int i = 0; i < _countof(data); i++) {
			Assert::IsTrue(data2[i] == data[i]);
		}
		File::remove(tmpFilename);
	}
	TEST_METHOD(currentDirectory) {
		auto dirw = File::currentDirectory();
		auto dir  = WString::toString(dirw);
		Assert::IsTrue(String::endsWith(dir, "Debug"));
	}
};

}