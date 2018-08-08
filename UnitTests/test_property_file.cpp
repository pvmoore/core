#include "_pch.h"
#include "../Core/core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;
using std::wstring;

namespace UnitTests {

TEST_CLASS(propertyfile) {
public:
    static wstring tmpFile;
    TEST_CLASS_INITIALIZE(ClassInitialize) {
        tmpFile = File::createTemp();
        Logger::WriteMessage((L"Creating propertyfile tmp file '%s'" + tmpFile).c_str());

        File::writeText(tmpFile, 
            "prop1 = hello\n"
            "prop2 = 10\n"
            "prop3 = 3.14\n"
        );
    }
    TEST_CLASS_CLEANUP(ClassCleanup) {
        Logger::WriteMessage((L"removing propertyfile " + tmpFile).c_str());
        File::remove(tmpFile);
    }
    TEST_METHOD(size) {
        PropertyFile props{tmpFile};
        Assert::IsTrue(props.size() == 3);
        Assert::IsFalse(props.isModified());
    }
    TEST_METHOD(read) {
        PropertyFile props{tmpFile};
        Assert::IsTrue(props.size() == 3);
        Assert::IsTrue(props.get("prop1", "there") == string("hello"));
        Assert::IsTrue(props.get("prop2", (slong)1) == 10);
        Assert::IsTrue(props.get("prop3", (double)3) == 3.14);
        Assert::IsTrue(props.get("notexist", (double)5) == 5);
        Assert::IsFalse(props.isModified());
    }
    TEST_METHOD(update) {
        PropertyFile props{tmpFile};
        props.set("prop4", "there");
        props.set("prop5", (slong)4);
        props.set("prop6", 7.7);
        props.set("prop1", "hello2");
        Assert::IsTrue(props.size() == 6);
        Assert::IsTrue(props.isModified());
        Assert::IsTrue(props.get("prop1", "") == "hello2");
        Assert::IsTrue(props.get("prop4", "") == "there");
    }
};

wstring propertyfile::tmpFile;
}