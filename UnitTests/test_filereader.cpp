#include "_pch.h"
#include "../Core/core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;
using std::vector;

namespace UnitTests {

TEST_CLASS(filereader) {
public:
	TEST_METHOD(Constructor) {
		FileReader<1024> reader{L"../../LICENSE"};
		Assert::IsFalse(reader.eof());
		Assert::IsTrue(reader.size == 1089);
	}
	TEST_METHOD(readLine) {
		FileReader<1024> reader{L"../../LICENSE"};
		vector<string> lines;
		while(!reader.eof()) {
			lines.push_back(reader.readLine());
		}
		Assert::IsTrue(lines.size() == 21);
		Assert::IsTrue(lines.front() == "MIT License");
		Assert::IsTrue(lines[9] == "furnished to do so, subject to the following conditions:");
		Assert::IsTrue(lines.back() == "SOFTWARE.");
	}
};

}