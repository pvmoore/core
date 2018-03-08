#include "_pch.h"
#include "../Core/core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;

namespace UnitTests {

TEST_CLASS(log) {
public:
	TEST_METHOD(write_char_ptr_ellipsis) {
		string filename = "log.log";
		Log::write("%s %d", "hello", 7);
		Log::close();
		Assert::IsTrue(File::size(filename) == 9);
		Assert::IsTrue(File::readText(filename) == "hello 7\r\n");
	}
	TEST_METHOD(dbg) {
		Log::dbg("hello");
		// This should appear in the output window
	}
};
}