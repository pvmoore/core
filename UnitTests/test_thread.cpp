#include "_pch.h"
#include "../core.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace core;
using std::string;

namespace UnitTests {

DWORD staticFunc(void* args) {
	uint* p = (uint*)args;
	uint arg = p[0];
	return 7;
}

TEST_CLASS(thread) {
public:
	TEST_METHOD(create_and_start) {
		const auto lambdaFunc = [](void* args)->DWORD {
			uint* p = (uint*)args;
			uint arg = p[0];
			Logger::WriteMessage(String::format("[%u] I am the new thread. Arg is %u...",
				   Thread::currentThreadId(), arg).c_str());
			Logger::WriteMessage(String::format("[%u] Thread finishing", Thread::currentThreadId()).c_str());
			return 13;
		};
		uint arg = 1;
		uint arg2 = 2;

		Thread t1("One", lambdaFunc, &arg);
		Thread t2("Two", staticFunc, &arg2);

		Assert::AreEqual(t1.name.c_str(), "One");
		Assert::AreEqual(t2.name.c_str(), "Two");
		Assert::IsTrue(t1.getStatus()==Thread::Status::NOT_STARTED);
		Assert::IsTrue(t2.getStatus() == Thread::Status::NOT_STARTED);

		t1.start();
		t2.start();

		t1.sleep(10);
		t1.yield();
		/// Wait for them to finish
		t1.join();
		t2.join();
		Assert::IsTrue(t1.isFinished());
		Assert::IsTrue(t1.getExitCode()==13);

		Assert::IsTrue(t2.isFinished());
		Assert::IsTrue(t2.getExitCode() == 7);

		Logger::WriteMessage(String::format("Thread cycles used: %llu", t1.getCyclesUsed()).c_str());
		Logger::WriteMessage(String::format("Thread cycles used: %llu", t2.getCyclesUsed()).c_str());
	}
};

}