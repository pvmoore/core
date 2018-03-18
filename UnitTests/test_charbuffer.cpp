#include "_pch.h"
#include "../Core/core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;

//Logger::WriteMessage("In Method1");

namespace UnitTests {

TEST_CLASS(charbuffer) {
public:
	TEST_METHOD(Constructors) {
		CharBuffer c;
		Assert::AreEqual(c.length(), 0u);
		Assert::AreEqual(c.capacity(), 0u);
		Assert::IsTrue(c.empty());

		CharBuffer c2{"Hello", 5};
		Assert::AreEqual(c2.length(), 5u);
		Assert::IsTrue(c2.capacity() >= 5);
		Assert::IsFalse(c2.empty());
	}
	TEST_METHOD(c_str) {
		CharBuffer c{"a string"};
		Assert::AreEqual(c.c_str(), "a string");
		Assert::AreEqual(CharBuffer{""}.c_str(), "");
		Assert::AreEqual(CharBuffer{"1"}.c_str(), "1");
	}
	TEST_METHOD(clear) {
		CharBuffer c{"some text"};
		Assert::IsFalse(c.empty());
		c.clear();
		Assert::IsTrue(c.empty() && c.length() == 0 && c.capacity()>0);
	}
	TEST_METHOD(pack) {
		CharBuffer c{"some text"};
		c.clear();
		c.pack();
		Assert::IsTrue(c.empty() && c.length() == 0 && c.capacity() == 1); // space for /0
	}
	TEST_METHOD(operator_equals_char_ptr) {
		CharBuffer c{"hello"};
		Assert::IsTrue(c == "hello");
		Assert::IsTrue(CharBuffer{} == "");
		Assert::IsTrue(c != "Hello");
		Assert::IsTrue(c != "");
		Assert::IsTrue(c != nullptr);
		Assert::IsTrue("hello" == c); // friend
		Assert::IsTrue("hi" != c);	// friend
	}
	TEST_METHOD(operator_equals_string) {
		CharBuffer c{"hello"};
		Assert::IsTrue(c == string("hello"));
		Assert::IsTrue(CharBuffer{} == string(""));
		Assert::IsTrue(c != string("Hello"));
		Assert::IsTrue(c != string(""));
		Assert::IsTrue(string("hello") == c); // friend
		Assert::IsTrue(string("hi") != c);	// friend
	}
	TEST_METHOD(operator_index) {
		CharBuffer c{"hello"};
		Assert::IsTrue(c[0] == 'h');
	}
    TEST_METHOD(append_char) {
        CharBuffer c{};
        c.append('a');
        Assert::IsTrue(!c.empty() && c.length()==1 && c[0]=='a');
        Assert::IsTrue(c[1] == 0);
    }
	TEST_METHOD(append_string) {
		CharBuffer c{};
		c.append(string("hello"))
		 .append(string(" there"));

		Assert::IsTrue(c.length() == 11 && c == "hello there");
	}
	TEST_METHOD(append_char_ptr) {
		CharBuffer c{};
		c.append("hello")
		 .append(" there");

		Assert::IsTrue(c.length() == 11 && c == "hello there");
	}
	TEST_METHOD(appendFmt) {
		CharBuffer c{};
		c.appendFmt("%s", "hello")
		 .appendFmt("%d,", 5)
		 .appendFmt("%.2f", 3.143f);

		Assert::IsTrue(c.length() == 11 && c == "hello5,3.14");
	}
	TEST_METHOD(contains_char_ptr) {
		CharBuffer c{"hello"};
		Assert::IsTrue(c.contains("ll"));
		Assert::IsTrue(c.contains("hello"));
		Assert::IsTrue(c.contains("ello"));
		Assert::IsTrue(c.contains("llo"));
		Assert::IsTrue(c.contains("lo"));
		Assert::IsTrue(c.contains("o"));
		Assert::IsTrue(!c.contains(""));
		Assert::IsTrue(!c.contains("la"));
	}
	TEST_METHOD(contains_string) {
		CharBuffer c{"hello"};
		Assert::IsTrue(c.contains(string("ll")));
		Assert::IsTrue(c.contains(string("hello")));
		Assert::IsTrue(c.contains(string("ello")));
		Assert::IsTrue(c.contains(string("llo")));
		Assert::IsTrue(c.contains(string("lo")));
		Assert::IsTrue(c.contains(string("o")));
		Assert::IsTrue(!c.contains(string("")));
		Assert::IsTrue(!c.contains(string("la")));
	}
	TEST_METHOD(indexOf_char_ptr_int) {
		CharBuffer c{"hello"};
		Assert::IsTrue(c.indexOf("hello") == 0);
		Assert::IsTrue(c.indexOf("ello") == 1);
		Assert::IsTrue(c.indexOf("llo") == 2);
		Assert::IsTrue(c.indexOf("ll") == 2);
		Assert::IsTrue(c.indexOf(nullptr) == -1);
		Assert::IsTrue(c.indexOf("") == -1);
		Assert::IsTrue(c.indexOf("hello", 1) == -1);
	}
	TEST_METHOD(indexOf_string_int) {
		CharBuffer c{"hello"};
		Assert::IsTrue(c.indexOf(string("hello")) == 0);
		Assert::IsTrue(c.indexOf(string("")) == -1);
	}
	TEST_METHOD(indexOf_char_int) {
		CharBuffer c{"hello"};
		Assert::IsTrue(c.indexOf('h') == 0);
		Assert::IsTrue(c.indexOf('e') == 1);
		Assert::IsTrue(c.indexOf('l') == 2);
		Assert::IsTrue(c.indexOf('o') == 4);
		Assert::IsTrue(c.indexOf('z') == -1);
		Assert::IsTrue(c.indexOf((char)0) == -1);
		Assert::IsTrue(c.indexOf('h', 1) == -1);
	}
};

} 