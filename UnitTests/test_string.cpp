#include "_pch.h"
#include "../core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;

namespace UnitTests {

TEST_CLASS(string) {
public:
	TEST_METHOD(startswith) {
		Assert::IsTrue(String::startsWith("abcd", "abc"));
		Assert::IsTrue(!String::startsWith("abcd", "aabc"));
	}
	TEST_METHOD(endswith) {
		Assert::IsTrue(String::endsWith("abcd", std::string("bcd")));
		Assert::IsTrue(!String::endsWith("abcd", std::string("aabc")));
	}
	TEST_METHOD(contains) {
		Assert::IsTrue(String::contains("abcde", "cd"));
		Assert::IsTrue(!String::contains("abcde", "Cd"));
	}
	TEST_METHOD(trimRight) {
		Assert::IsTrue(String::trimRight("abc  ") == "abc");
	}
	TEST_METHOD(trimLeft) {
		Assert::IsTrue(String::trimLeft("\t abc") == "abc");
	}
	TEST_METHOD(trimBoth) {
		Assert::IsTrue(String::trimBoth("  \n123 \t ") == "123");
	}
	TEST_METHOD(split_string) {
		auto vec = String::split(" 0  1\n2 \t3 ");
		Assert::IsTrue(vec.size() == 4);
		Assert::IsTrue(vec[0] == "0" && vec[1] == "1" && vec[2] == "2" && vec[3] == "3");
	}
	TEST_METHOD(split_string_char) {
		auto vec = String::split("0,1,2,3", ',');
		Assert::IsTrue(vec.size() == 4);
		Assert::IsTrue(vec[0] == "0" && vec[1] == "1" && vec[2] == "2" && vec[3] == "3");
	}
	TEST_METHOD(toString_vector_string_string) {
		std::vector<std::string> vec{"0", "1"};
		auto s = String::toString(vec, ", ");
		Assert::IsTrue(s == "0, 1");
	}
	TEST_METHOD(toInt) {
		Assert::IsTrue(String::toInt("436") == 436);
	}
	TEST_METHOD(toFloat) {
		Assert::IsTrue(String::toFloat("3.14") == 3.14f);
	}
	TEST_METHOD(format) {
		auto s = String::format("%s %d", "hello", 40);
		Assert::AreEqual(s.c_str(), "hello 40");
	}
};

}