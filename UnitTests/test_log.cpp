#include "_pch.h"
#include "../Core/core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;
using std::wstring;

namespace UnitTests {

TEST_CLASS(log) {
public:
	TEST_METHOD(format) {
		Log::close();
		wstring filename = L"log.log";
		Log::format("%s %d", "hello", 7);
		Log::close();
		Assert::IsTrue(File::size(filename) == 9);
		Assert::IsTrue(File::readText(filename) == "hello 7\r\n");
	}
	TEST_METHOD(write) {
		Log::close();
		wstring filename = L"log.log";
		Log::write("The num is ", 
                   13,                          // int
                   (uint)14,                    // uint
                   " + ",                       // const char*
                   2.0f,                        // float
                   3.14,                        // double
                   (const char*)nullptr,        // nullptr
                   string("oo"),                // std::string
                   (unsigned char)3,            // ubyte
                   (short)70,                   // signed short
                   (char*)"...",                // char*
                   true                         // bool
        );
		Log::close();
		Assert::IsTrue(File::size(filename) == 39);
		Assert::AreEqual(File::readText(filename).c_str(), "The num is 1314 + 2.03.14oo370...true\r\n");
	}
	TEST_METHOD(dbg) {
		Log::dbg("hello");
		// This should appear in the output window
	}
};
}