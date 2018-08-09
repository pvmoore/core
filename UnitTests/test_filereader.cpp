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
    TEST_METHOD(read) {
        FileReader<16> reader{L"../../LICENSE"};
        char temp[32];
        char expect[] = {
            0x4D, 0x49, 0x54, 0x20, 0x4C, 0x69, 0x63, 0x65,
            0x6E, 0x73, 0x65, 0x0D, 0x0A, 0x0D, 0x0A, 0x43,
            0x6F, 0x70, 0x79, 0x72, 0x69, 0x67, 0x68, 0x74,
            0x20, 0x28, 0x63, 0x29, 0x20, 0x32, 0x30, 0x31
        };

        reader.read(temp+0, 15); 
        Assert::IsTrue(reader.pos()==15);
        Assert::IsTrue(0==memcmp(temp, expect, 15));

        reader.read(temp+15, 8);
        Assert::IsTrue(reader.pos() == 23);
        Assert::IsTrue(0 == memcmp(temp, expect, 23));

        reader.read(temp+23, 5);
        Assert::IsTrue(reader.pos() == 28);
        Assert::IsTrue(0 == memcmp(temp, expect, 28));

        reader.read(temp+28, 4);
        Assert::IsTrue(reader.pos() == 32);
        Assert::IsTrue(0 == memcmp(temp, expect, 32));
    }
    TEST_METHOD(readDouble) {
       
    }
};

}