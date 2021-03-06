#include "_pch.h"
#include "../Core/core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;
using std::wstring;

namespace UnitTests {

TEST_CLASS(file) {
public:
	TEST_METHOD(exists) {
		Assert::IsTrue(File::exists(L"../../LICENSE"));
	}
	TEST_METHOD(size) {
		Assert::IsTrue(File::size(L"../../LICENSE") == 1089);
	}
	TEST_METHOD(createTemp) {
		wstring filename = File::createTemp(L"_temp");
		Assert::IsTrue(File::exists(filename));
		Logger::WriteMessage(String::format("File::createTemp -> Created temp file '%s'\n", filename.c_str()).c_str());
		File::remove(filename);
	}
	TEST_METHOD(rename) {
		wstring filename = File::createTemp(L"_temp");
		Assert::IsTrue(File::exists(filename));
		wstring filename2 = filename + L"2";
		File::rename(filename, filename2);
		Assert::IsFalse(File::exists(filename));
		Assert::IsTrue(File::exists(filename2));
		File::remove(filename2);
	}
	TEST_METHOD(remove) {
		wstring filename = File::createTemp(L"_temp");
		Assert::IsTrue(File::exists(filename));
		File::remove(filename);
		Assert::IsFalse(File::exists(filename));
	}
	TEST_METHOD(isDirectory) {
		Assert::IsTrue(File::isDirectory(L"../Debug"));
		Assert::IsTrue(File::isDirectory(L"."));
		Assert::IsTrue(File::isDirectory(L".."));

		Assert::IsFalse(File::isDirectory(L"_pch.h"));
		Assert::IsFalse(File::isDirectory(L""));
	}
	TEST_METHOD(isFile) {
		Assert::IsTrue(File::isFile(L"../../LICENSE"));

		Assert::IsFalse(File::isFile(L"../Debug"));
		Assert::IsFalse(File::isFile(L"."));
	}
	TEST_METHOD(readText) {
		auto text = File::readText(L"../../LICENSE");
		Assert::IsTrue(text.size() == 1089);
		Assert::IsTrue(text.find("MIT License") == 0);
		Assert::IsTrue(text.find("SOFTWARE.") == 1089 - 11); // assumes it ends in \r\n
	}
	TEST_METHOD(writeText) {
		auto tmpFilename = File::createTemp();
		string text = "I am some text";
		File::writeText(tmpFilename, text);
		Assert::IsTrue(File::size(tmpFilename) == text.size());
		Assert::IsTrue(File::readText(tmpFilename) == text);
		File::remove(tmpFilename);
	}
    TEST_METHOD(readBinary_static_array) {
        char array[1089];
        auto num = File::readBinary(L"../../LICENSE", array);
        Assert::IsTrue(num == 1089);
        Assert::IsTrue(array[0] == 'M');
        Assert::IsTrue(array[1] == 'I');
        Assert::IsTrue(array[2] == 'T');

        char array2[16];
        auto num2 = File::readBinary(L"../../LICENSE", array2);
        Assert::IsTrue(num2 == 16);
        Assert::IsTrue(array2[0] == 'M');
        Assert::IsTrue(array2[1] == 'I');
        Assert::IsTrue(array2[2] == 'T');
    }
	TEST_METHOD(writeBinary_static_array) {
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
    TEST_METHOD(read_write_binary_dynamic_array) {
        auto tmpFilename = File::createTemp();
        const uint N = 10;
        uint* data = new uint[N];
        for(int i=0; i<N; i++) { data[i] = i; }

        /// writeBinary (dynamic)
        File::writeBinary(tmpFilename, data, N);
        Assert::IsTrue(File::size(tmpFilename) == N * sizeof(uint));

        /// readBinary (dynamic)
        uint* data2 = new uint[N];
        File::readBinary(tmpFilename, data2, N);
        for(int i = 0; i < N; i++) {
            Assert::IsTrue(data2[i] == data[i]);
        }
        delete[] data;
        delete[] data2;
        File::remove(tmpFilename);
    }
	TEST_METHOD(currentDirectory) {
		auto dirw = File::currentDirectory();
		auto dir  = WString::toString(dirw);
		Assert::IsTrue(String::endsWith(dir, "Debug"));
	}
};

}