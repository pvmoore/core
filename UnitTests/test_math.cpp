#include "_pch.h"
#include "../Core/core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;
using std::vector;

namespace UnitTests {

TEST_CLASS(math) {
public:
	TEST_METHOD(factorsOf) {
		Assert::IsTrue(Math::factorsOf(0).size()==0);

		vector<uint> v = {1};
		Assert::IsTrue(Math::factorsOf(1) == v);

		v = {2,1};
		Assert::IsTrue(Math::factorsOf(2) == v);

		v = {4, 2, 1};
		Assert::IsTrue(Math::factorsOf(4) == v);

		v = {6, 3, 2, 1};
		Assert::IsTrue(Math::factorsOf(6) == v);

		v = {12, 6, 4, 3, 2, 1};
		Assert::IsTrue(Math::factorsOf(12) == v);
	}
};

}