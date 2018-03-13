#include "_pch.h"
#include "../Core/core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;
using std::wstring;
using std::vector;

namespace UnitTests {

TEST_CLASS(fun) {
public:
	TEST_METHOD(forEach) {

		vector<int> v = {1,2,3};
		::forEach(v, [](int i)->void{ Logger::WriteMessage(String::format("hello %d\n", i).c_str()); });

	}
	TEST_METHOD(map) {
		vector<int> v = {1,2,3};

		auto v2 = ::map(v, [](int i)->float { return (float)i+1; } );
		Logger::WriteMessage(String::format("v2 = %f", v2[0]).c_str());

	}
};

}