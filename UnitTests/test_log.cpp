#include "_pch.h"
#include "../Core/core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;

namespace UnitTests {

TEST_CLASS(log) {
public:
	TEST_METHOD(format) {
		Log::close();
		string filename = "log.log";
		Log::format("%s %d", "hello", 7);
		Log::close();
		Assert::IsTrue(File::size(filename) == 9);
		Assert::IsTrue(File::readText(filename) == "hello 7\r\n");
	}
	TEST_METHOD(write) {
		Log::close();
		string filename = "log.log";
		Log::write("The num is ", 13, " + ", 2.0f);
		Log::close();
		Assert::IsTrue(File::size(filename) == 21);
		Assert::AreEqual(File::readText(filename).c_str(), "The num is 13 + 2.0\r\n");
	}
	TEST_METHOD(dbg) {
		Log::dbg("hello");
		// This should appear in the output window
	}
};
}